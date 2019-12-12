#!/usr/bin/env python3

import click
import numpy as np
import cv2 as cv
from enum import Enum, auto


NUM_BLOCKS = 2000
BLOCK_SIZE = 36
OVERLAP_INVERSE_RATIO = 6
OVERLAP = (BLOCK_SIZE + OVERLAP_INVERSE_RATIO - 1) // OVERLAP_INVERSE_RATIO

np.random.seed(42)


class SynthesisPolicy(Enum):
    RANDOM = auto()
    WEIGHTED = auto()
    FRONTIER = auto()


def distance_matrix(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    return np.sum((a.astype(np.int64) - b.astype(np.int64)) ** 2, axis=(2,))


def squared_distance(a: np.ndarray, b: np.ndarray) -> int:
    return np.sum(distance_matrix(a, b))


def min_column_path(matrix: np.ndarray) -> np.array:
    n1, n2 = matrix.shape
    min_cost = np.ndarray(matrix.shape, dtype=matrix.dtype)
    min_cost[0, :] = matrix[0, :].copy()
    for i in range(1, n1):
        min_cost[i] = matrix[i] + np.minimum(
            min_cost[i - 1],
            np.minimum(
                np.append(min_cost[i - 1][1:], [np.inf]),
                np.append([np.inf], min_cost[i - 1][:-1]),
            ),
        )

    def find_path() -> np.array:
        x2 = np.argmin(min_cost[n1 - 1])
        path = [x2]
        for x1 in range(n1 - 1, 0, -1):
            goal = int(min_cost[x1, x2]) - matrix[x1, x2]
            for nx2 in (x2 - 1, x2, x2 + 1):
                if nx2 != -1 and nx2 != n2 and min_cost[x1 - 1, nx2] == goal:
                    x2 = nx2
                    break
            path.append(x2)
        return np.array(path[::-1])

    return find_path()


def load(image: str) -> np.ndarray:
    return cv.imread(image, cv.IMREAD_COLOR)


def random_block(image: np.ndarray, block_size: int) -> np.ndarray:
    h, w = image.shape[:2]
    i1 = np.random.randint(low=0, high=h - block_size, size=1)[0]
    i2 = np.random.randint(low=0, high=w - block_size, size=1)[0]
    return image[i1 : i1 + block_size, i2 : i2 + block_size, ...]


def random_synthesis(image: np.ndarray) -> np.ndarray:
    h, w = image.shape[:2]
    blocks_h, blocks_w = (
        (h + BLOCK_SIZE - 1) // BLOCK_SIZE,
        (w + BLOCK_SIZE - 1) // BLOCK_SIZE,
    )
    random_blocks = [random_block(image, BLOCK_SIZE) for _ in range(NUM_BLOCKS)]
    result = np.zeros((blocks_h * BLOCK_SIZE, blocks_w * BLOCK_SIZE, 3))
    indices = np.random.randint(low=0, high=NUM_BLOCKS, size=blocks_h * blocks_w)
    for i1 in range(blocks_h):
        for i2 in range(blocks_w):
            result[
                i1 * BLOCK_SIZE : (i1 + 1) * BLOCK_SIZE,
                i2 * BLOCK_SIZE : (i2 + 1) * BLOCK_SIZE,
                ...,
            ] = random_blocks[indices[i1 * blocks_w + i2]]
    return result[: h + h, : w + w, ...]


def weighted_synthesis(image: np.ndarray, is_frontier: bool = False) -> np.ndarray:
    h, w = image.shape[:2]
    block_non_overlap = BLOCK_SIZE - OVERLAP
    blocks_h, blocks_w = (
        (h + block_non_overlap - 1) // block_non_overlap,
        (w + block_non_overlap - 1) // block_non_overlap,
    )
    random_blocks = [random_block(image, BLOCK_SIZE) for _ in range(NUM_BLOCKS)]

    result_h = (blocks_h - 1) * block_non_overlap + BLOCK_SIZE
    result_w = (blocks_w - 1) * block_non_overlap + BLOCK_SIZE
    result = np.zeros((result_h, result_w, 3))

    for i1 in range(blocks_h):
        for i2 in range(blocks_w):
            x, y = i1 * block_non_overlap, i2 * block_non_overlap
            best_cost = None
            for block in random_blocks:
                cost = 0
                if x != 0:
                    cost += squared_distance(
                        result[x : x + OVERLAP, y : y + BLOCK_SIZE, ...],
                        block[:OVERLAP, ...],
                    )
                if y != 0:
                    cost += squared_distance(
                        result[x : x + BLOCK_SIZE, y : y + OVERLAP, ...],
                        block[:, :OVERLAP, ...],
                    )
                if x != 0 and y != 0:
                    cost -= squared_distance(
                        result[x : x + OVERLAP, y : y + OVERLAP, ...],
                        block[:OVERLAP, :OVERLAP, ...],
                    )
                if best_cost is None or best_cost > cost:
                    best_cost, best_block = cost, block
            mask = np.ones((BLOCK_SIZE, BLOCK_SIZE, 3), dtype=bool)
            if is_frontier:
                if x != 0:
                    path = min_column_path(
                        distance_matrix(
                            result[x : x + OVERLAP, y : y + BLOCK_SIZE, ...],
                            block[:OVERLAP, ...],
                        )
                    )
                    for r, c in enumerate(path):
                        mask[r, :c] = False
                if y != 0:
                    path = min_column_path(
                        np.transpose(
                            distance_matrix(
                                result[x : x + BLOCK_SIZE, y : y + OVERLAP, ...],
                                block[:, :OVERLAP, ...],
                            )
                        )
                    )
                    for c, r in enumerate(path):
                        mask[:r, c] = False
            np.copyto(
                src=best_block,
                dst=result[x : x + BLOCK_SIZE, y : y + BLOCK_SIZE, ...],
                where=mask,
            )

    return result[: h + h, : w + w, ...]


@click.command()
@click.option(
    "-i", "--image", type=click.Path(exists=True, dir_okay=False), required=True
)
@click.option(
    "-p",
    "--synthesis_policy",
    type=click.Choice(SynthesisPolicy.__members__),
    callback=lambda _, __, v: getattr(SynthesisPolicy, v) if v else None,
    required=True,
)
@click.option("-o", "--output", type=click.Path(dir_okay=False), required=True)
def main(image: str, synthesis_policy: SynthesisPolicy, output: str) -> None:
    loaded_image = load(image)
    if synthesis_policy is SynthesisPolicy.RANDOM:
        result = random_synthesis(loaded_image)
    elif synthesis_policy is SynthesisPolicy.WEIGHTED:
        result = weighted_synthesis(loaded_image)
    elif synthesis_policy is SynthesisPolicy.FRONTIER:
        result = weighted_synthesis(loaded_image, is_frontier=True)
    else:
        raise NotImplementedError()
    cv.imwrite(output, result)


if __name__ == "__main__":
    main()
