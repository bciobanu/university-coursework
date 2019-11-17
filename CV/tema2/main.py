#!/usr/bin/env python3

from enum import Enum, auto
from functools import update_wrapper
from typing import Generator, List

import click
import cv2 as cv
import numpy as np

INF = 2 ** 50


class SeamPathPolicy(Enum):
    OPTIMAL = auto()
    GREEDY = auto()
    RANDOM = auto()


SEAM_PATH_POLICY = SeamPathPolicy.OPTIMAL


def transposed(image: np.array) -> np.array:
    return np.transpose(image, axes=(1, 0, 2))


def extract_edges(image: np.ndarray) -> np.ndarray:
    n1, n2, _ = image.shape
    image = cv.cvtColor(image.astype(np.uint8), cv.COLOR_BGR2GRAY).astype(np.float64)

    energy = np.zeros((n1, n2))
    m = np.zeros((n1, n2))

    u = np.roll(image, 1, axis=0)
    l = np.roll(image, 1, axis=1)
    r = np.roll(image, -1, axis=1)

    cu = np.abs(r - l)
    cl = np.abs(u - l) + cu
    cr = np.abs(u - r) + cu

    for i in range(1, n1):
        mu = m[i - 1]
        ml = np.roll(mu, 1)
        mr = np.roll(mu, -1)

        culr = np.array([cu[i], cl[i], cr[i]])
        mulr = np.array([mu, ml, mr]) + culr

        argmins = np.argmin(mulr, axis=0)
        m[i] = np.choose(argmins, mulr)
        energy[i] = np.choose(argmins, culr)
    return energy


def remove_column_path(image: np.ndarray, path: np.array) -> np.ndarray:
    assert len(image.shape) == 3
    n1, n2, c = image.shape
    assert len(path) == n1 and c == 3

    resulting_image = np.ndarray((n1, n2 - 1, c), dtype=image.dtype)
    for i1, i2 in enumerate(path):
        resulting_image[i1] = np.concatenate((image[i1, :i2], image[i1, i2 + 1 :]))

    return resulting_image


def min_column_path_random(matrix: np.ndarray) -> np.ndarray:
    n1, n2 = matrix.shape
    i2 = np.random.randint(n2)
    path = [i2]
    for i1 in range(1, n1):
        selection = [i2]
        if i2 != 0:
            selection.append(i2 - 1)
        if i2 != n2 - 1:
            selection.append(i2 + 1)
        i2 = np.random.choice(selection)
        path.append(i2)
    return np.array(path)


def min_column_path_greedy(matrix: np.ndarray) -> np.array:
    n1, n2 = matrix.shape
    i2 = np.argmin(matrix[0])
    path = [i2]
    for i1 in range(1, n1):
        best = i2
        for ni2 in (i2 - 1, i2 + 1):
            if ni2 != -1 and ni2 != n2 and matrix[i1, ni2] > matrix[i1, best]:
                best = ni2
        i2 = best
        path.append(i2)
    return np.array(path)


def min_column_path_optimal(matrix: np.ndarray) -> np.array:
    n1, n2 = matrix.shape
    min_cost = np.ndarray(matrix.shape, dtype=matrix.dtype)
    min_cost[0, :] = matrix[0, :].copy()
    for i in range(1, n1):
        min_cost[i] = matrix[i] + np.minimum(
            min_cost[i - 1],
            np.minimum(
                np.append(min_cost[i - 1][1:], [INF]),
                np.append([INF], min_cost[i - 1][:-1]),
            ),
        )

    def find_path() -> List[int]:
        x2 = np.argmin(min_cost[n1 - 1])
        path = [x2]
        for x1 in range(n1 - 1, 0, -1):
            goal = int(min_cost[x1, x2]) - matrix[x1, x2]
            for nx2 in (x2 - 1, x2, x2 + 1):
                if nx2 != -1 and nx2 != n2 and min_cost[x1 - 1, nx2] == goal:
                    x2 = nx2
                    break
            path.append(x2)
        return path[::-1]

    return np.array(find_path())


def min_column_path(matrix: np.ndarray) -> np.array:
    assert len(matrix.shape) == 2
    if SEAM_PATH_POLICY == SeamPathPolicy.OPTIMAL:
        return min_column_path_optimal(matrix)
    elif SEAM_PATH_POLICY == SeamPathPolicy.GREEDY:
        return min_column_path_greedy(matrix)
    elif SEAM_PATH_POLICY == SeamPathPolicy.RANDOM:
        return min_column_path_random(matrix)
    else:
        raise NotImplementedError


def decrease_width(image: np.array, times: int = 1) -> np.array:
    for _ in range(times):
        image = remove_column_path(image, min_column_path(extract_edges(image)))
    return image


def increase_width(image: np.array, times: int = 1) -> np.array:
    assert len(image.shape) == 3
    n1, n2, _ = image.shape
    assert times <= n2

    def fill_erased_columns() -> Generator[Generator[int, None, None], None, None]:
        erased_columns = [[-1 for _ in range(times)] for _ in range(n1)]
        image_iter = image
        for i in range(times):
            path = min_column_path(extract_edges(image_iter))
            for place, what in zip(erased_columns, path):
                place[i] = what
            image_iter = remove_column_path(image_iter, path)

        def shift_increasing(row: List[int]) -> Generator[int, None, None]:
            for idx, col in enumerate(sorted(row)):
                yield idx + col

        for row in erased_columns:
            yield shift_increasing(row)

    result = np.ndarray((n1, n2 + times, 3), dtype=image.dtype)
    for i, erased in enumerate(fill_erased_columns()):
        row = image[i].copy()
        for column in erased:
            pixels = np.ndarray((2, 3), dtype=image.dtype)
            for idx, neighbour_column in enumerate((column - 1, column + 1)):
                if neighbour_column != -1 and neighbour_column != len(row):
                    pixels[idx] = np.mean([row[neighbour_column], row[column]], axis=0)
                else:
                    pixels[idx] = row[column]
            row = np.concatenate((row[:column], pixels, row[column + 1 :]))
        result[i] = row
    return result


@click.group(chain=True)
def cli() -> None:
    ...


@cli.resultcallback()
def process_commands(processors) -> None:
    stream = None
    for processor in processors:
        stream = processor(stream)


def processor(f):
    def new_func(*args, **kwargs):
        def processor(stream):
            return f(*args, **kwargs) if stream is None else f(stream, *args, **kwargs)

        return processor

    return update_wrapper(new_func, f)


@cli.command()
@click.option(
    "-i", "--image", type=click.Path(exists=True, dir_okay=False), required=True,
)
@processor
def load(image: str) -> np.ndarray:
    return cv.imread(image, cv.IMREAD_COLOR)


@cli.command()
@click.option("-l", "--lines", type=int, default=0)
@click.option("-c", "--columns", type=int, default=0)
@click.option(
    "-p",
    "--policy",
    type=click.Choice(SeamPathPolicy.__members__),
    callback=lambda _, __, v: getattr(SeamPathPolicy, v) if v else None,
    default="OPTIMAL",
)
@processor
def resize(
    image: np.ndarray, lines: int, columns: int, policy: SeamPathPolicy
) -> np.ndarray:
    SEAM_PATH_POLICY = policy
    if columns < 0:
        image = decrease_width(image, -columns)
    elif columns > 0:
        image = increase_width(image, columns)

    image = transposed(image)
    if lines < 0:
        image = decrease_width(image, -lines)
    elif lines > 0:
        image = increase_width(image, lines)
    image = transposed(image)
    return image


@cli.command()
@click.option(
    "-p",
    "--policy",
    type=click.Choice(SeamPathPolicy.__members__),
    callback=lambda _, __, v: getattr(SeamPathPolicy, v) if v else None,
    default="OPTIMAL",
)
@processor
def cut(image: np.ndarray, policy: SeamPathPolicy) -> np.ndarray:
    SEAM_PATH_POLICY = policy
    y0, x0, y1, x1 = cv.selectROI("Cut", image)
    y1, x1 = y0 + y1 - 1, x0 + x1 - 1
    if x1 - x0 < y0 - y1:
        image = transposed(image)
        should_transpose = True
    else:
        should_transpose = False

    for c in range(y1, y0 - 1, -1):
        edges = extract_edges(image)
        edges[x0 : x1 + 1, :y0] = INF
        edges[x0 : x1 + 1, c + 1 :] = INF
        path = min_column_path(edges)
        assert y0 <= np.min(path[x0 : x1 + 1]) and np.max(path[x0 : x1 + 1]) <= y1
        image = remove_column_path(image, path)

    if should_transpose:
        image = transposed(image)
    return image


@cli.command()
@click.option(
    "-o", "--output", type=click.Path(exists=False, dir_okay=False), required=True
)
@processor
def save(image: np.ndarray, output: str) -> None:
    cv.imwrite(output, image)


if __name__ == "__main__":
    cli()
