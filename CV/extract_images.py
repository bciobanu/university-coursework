#!/usr/bin/env python3
import pickle
import random
from typing import List

import click
import cv2 as cv
import numpy as np

SIZE = 32


def convert_format(cifar_image: np.array) -> np.array:
    result = np.empty((SIZE, SIZE, 3))
    idx = 0
    for i3 in range(3):
        for i1 in range(SIZE):
            for i2 in range(SIZE):
                result[i1, i2, i3] = cifar_image[idx]
                idx += 1
    return result


@click.command()
@click.option("--data", type=click.Path(exists=True, dir_okay=False), required=True)
def main(data: str):
    with open(data, "rb") as f:
        d = pickle.load(f, encoding="bytes")
    labels = d[b"labels"]
    to_extract: List[List[int]] = [[] for _ in range(5)]
    for i, l in enumerate(labels):
        if l >= 5:
            continue
        if len(to_extract[l]) > 500:
            continue
        to_extract[l].append(i)
    data = d[b"data"]
    for i1, s in enumerate(to_extract):
        for i2, l in enumerate(s):
            cv.imwrite(f"{i1}/{i2}.png", convert_format(data[l]))


if __name__ == "__main__":
    main()
