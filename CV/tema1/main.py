#!/usr/bin/env python3
import random
from enum import Enum, auto
from math import sqrt
from pathlib import Path
from typing import Dict, Generator, List, NamedTuple, Optional, Sequence, Tuple

import click
import cv2 as cv
import numpy as np
from scipy.spatial import KDTree

random.seed(42)


class MosaicType(Enum):
    RANDOM = auto()
    GRID = auto()
    HEXAGON = auto()


def read_image(image: str, grayscale: bool) -> np.array:
    return cv.imread(image, cv.IMREAD_GRAYSCALE if grayscale else cv.IMREAD_COLOR)


def load_dataset(dataset: str, grayscale: bool) -> np.array:
    return np.array(
        [read_image(str(img), grayscale) for img in Path(dataset).glob("*.png")]
    )


def average_color(image: np.array) -> np.array:
    result = np.mean(image, axis=(0, 1))
    if isinstance(result, np.float64):
        result = [result]
    return result


def compute_vertical_cells(
    image_shape: Tuple[int, int], mosaic_shape: Tuple[int, int], horizontal_cells: int
) -> Tuple[int, Tuple[int, int]]:
    big_h, big_v = image_shape
    small_h, small_v = mosaic_shape
    horizontal_pixels = small_h * horizontal_cells
    vertical_cells = int((big_v * horizontal_pixels) / (big_h * small_v))
    vertical_pixels = small_v * vertical_cells
    return vertical_cells, (horizontal_pixels, vertical_pixels)


class PixelChunk(NamedTuple):
    h: int
    v: int
    mean: np.array
    neighbours: Sequence["PixelChunk"]  # type: ignore


def pixel_chunks(
    image: np.array,
    horizontal_size: int,
    vertical_size: int,
    horizontal_pixels: int,
    vertical_pixels: int,
    mosaic_type: MosaicType,
) -> Generator[PixelChunk, None, None]:
    if mosaic_type is MosaicType.GRID:

        def grid_generator() -> Generator[Tuple[int, int], None, None]:
            for i1 in range(horizontal_pixels):
                for i2 in range(vertical_pixels):
                    yield horizontal_size * i1, vertical_size * i2

        generator = grid_generator
    elif mosaic_type is MosaicType.RANDOM:

        def random_generator() -> Generator[Tuple[int, int], None, None]:
            num_steps = horizontal_pixels * vertical_pixels * 7
            for _ in range(num_steps):
                h = random.randint(0, horizontal_size * (horizontal_pixels - 1))
                v = random.randint(0, vertical_size * (vertical_pixels - 1))
                yield h, v

        generator = random_generator
    else:
        raise NotImplementedError()

    visited: Dict[Tuple[int, int], PixelChunk] = {}
    for h, v in generator():
        neighbours = []
        for dx, dy in (
            (-horizontal_size, 0),
            (horizontal_size, 0),
            (0, vertical_size),
            (0, -vertical_size),
        ):
            try:
                neighbours.append(visited[(h + dx, v + dy)])
            except KeyError:
                ...

        chunk = PixelChunk(
            h=h,
            v=v,
            mean=average_color(image[h : h + horizontal_size, v : v + vertical_size]),
            neighbours=neighbours,
        )
        visited[(h, v)] = chunk
        yield chunk


def generate_mosaic_image(
    image_size: Tuple[int, int, int],
    dataset: np.array,
    kd_tree: KDTree,
    distinct: bool,
    chunks_generator: Generator[PixelChunk, None, None],
) -> np.array:
    image = np.empty(image_size)
    horizontal_size, vertical_size = dataset.shape[1:3]
    index_used: Dict[int, int] = {}
    for chunk in chunks_generator:
        _, best_indices = kd_tree.query(chunk.mean, k=5)
        banned_indices = {
            index_used.get(id(neighbour), None)
            for neighbour in chunk.neighbours
            if distinct
        }
        while best_indices[0] in banned_indices:
            best_indices = best_indices[1:]
        index_used[id(chunk)] = best_indices[0]
        image[
            chunk.h : chunk.h + horizontal_size, chunk.v : chunk.v + vertical_size, ...
        ] = dataset[best_indices[0]]
    return image


@click.command()
@click.option("--image", type=click.Path(exists=True, dir_okay=False), required=True)
@click.option("--dataset", type=click.Path(exists=True, file_okay=False), required=True)
@click.option(
    "--mosaic_type",
    type=click.Choice(MosaicType.__members__),
    callback=lambda _, __, v: getattr(MosaicType, v) if v else None,
    required=True,
)
@click.option("--output", type=click.Path(dir_okay=False), required=True)
@click.option("--horizontal_cells", type=int, default=100)
@click.option("--distinct", is_flag=True)
@click.option("--grayscale", is_flag=True)
def main(
    image: str,
    dataset: str,
    mosaic_type: MosaicType,
    output: str,
    horizontal_cells: int,
    distinct: bool,
    grayscale: bool,
) -> None:
    loaded_image = read_image(image, grayscale)
    loaded_dataset = load_dataset(dataset, grayscale)
    dataset_image_size = loaded_dataset.shape[1:3]
    vertical_cells, resized_image_size = compute_vertical_cells(
        loaded_image.shape[:2], dataset_image_size, horizontal_cells
    )
    resized_image = cv.resize(loaded_image, resized_image_size[::-1])
    kd_tree = KDTree([average_color(img) for img in loaded_dataset])
    pixel_chunks_generator = pixel_chunks(  # type: ignore
        resized_image,
        *dataset_image_size,
        horizontal_cells,
        vertical_cells,
        mosaic_type
    )
    generated_image = generate_mosaic_image(
        resized_image.shape, loaded_dataset, kd_tree, distinct, pixel_chunks_generator
    )
    cv.imwrite(output, generated_image, [cv.IMWRITE_JPEG_QUALITY, 10])


if __name__ == "__main__":
    main()
