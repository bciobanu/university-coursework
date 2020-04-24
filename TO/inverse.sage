from collections import namedtuple
from itertools import cycle

import numpy as np

MatInv = namedtuple("MatInv", ["mat", "inv"])


class NotInversibleError(Exception):
    ...


def change_column(A, b, idx):
    c = np.dot(A.inv, b)
    if np.isclose(c[idx], 0):
        raise NotInversibleError

    E = np.eye(len(A.mat))

    idx_col = -c / c[idx]
    idx_col[idx] = 1 / c[idx]

    E[:, idx] = idx_col

    inv = np.matmul(E, A.inv)

    mat = A.mat
    mat[:, idx] = b

    return MatInv(mat=mat, inv=inv)


N = 10
NUM_MATRICES = 100
NUM_COLUMN_CHANGES = 10

for _ in range(NUM_MATRICES):
    # start with an inversible matrix
    while True:
        A = np.random.rand(N, N)
        try:
            invA = np.linalg.inv(A)
            break
        except np.linalg.LinAlgError:
            ...

    A = MatInv(mat=A, inv=invA)

    for c, _ in zip(cycle(range(N)), range(NUM_COLUMN_CHANGES)):
        new_column = np.random.rand(N)
        try:
            A = change_column(A, new_column, c)
            assert np.allclose(np.matmul(A.mat, A.inv), np.eye(N))
        except NotInversibleError:
            mat = np.copy(A.mat)
            mat[:, c] = new_column
            assert np.linalg.det(mat) == 0
