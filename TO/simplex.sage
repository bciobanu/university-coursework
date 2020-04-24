import numpy as np
from scipy.optimize import linprog


class InfeasibleError(Exception):
    ...


class IterationLimit(Exception):
    ...


ITER_LIMIT = 1000


def select_primal(T):
    # enter variable
    enter = np.argmin(T[-1, :-1])

    # stop criterion
    if T[-1, enter] >= 0:
        return -T[-1, -1]

    # leave variable
    leave = None
    for r in range(len(T) - 1):
        if T[r, enter] <= 0:
            continue

        if leave is None:
            leave = r
        elif T[r, enter] * abs(T[leave, -1]) > T[leave, enter] * abs(T[r, -1]):
            leave = r

    if leave is None:
        raise InfeasibleError
    return leave, enter


def select_dual(T):
    # enter variable
    enter = np.argmin(T[:-1, -1])

    # stop criterion
    if T[enter, -1] >= 0:
        return -T[-1, -1]
    
    T[enter] = -T[enter]
    # leave variable
    leave = None
    for c in range(len(T[enter]) - 1):
        if T[enter, c] <= 0:
            continue
        if leave is None:
            leave = c
        elif T[enter, c] * abs(T[-1, leave]) > T[enter, leave] * abs(T[-1, c]):
            leave = c

    if leave is None:
        raise InfeasibleError
    return enter, leave


def simplex(A, b, c, select):
    assert A.dtype == b.dtype and b.dtype == c.dtype

    num_variables = len(c)
    assert c.shape == (num_variables,)

    num_constraints = len(b)
    assert b.shape == (num_constraints,)

    assert A.shape == (num_constraints, num_variables)

    # build initial table
    T = np.zeros(
        (num_constraints + 1, num_variables + num_constraints + 1,), dtype=A.dtype
    )
    T[:num_constraints, :num_variables] = A
    for idx, bound in enumerate(b):
        T[idx, num_variables + idx] = 1
        T[idx, -1] = bound
    T[num_constraints, :num_variables] = c

    for _ in range(ITER_LIMIT):
        # select
        selection_result = select(T)
        if not isinstance(selection_result, tuple):
            return selection_result

        row, column = selection_result

        # pivot
        for r in range(len(T)):
            if r == row:
                continue
            alpha = T[r, column] / T[row, column]
            T[r] -= T[row] * alpha
    raise IterationLimit


NUM_TESTS = 100
N, M = 5, 3
for method in (select_dual, select_primal):
    for _ in range(NUM_TESTS):
        num_variables, num_constraints = N, M

        A = np.random.rand(num_constraints, num_variables)
        b = np.random.rand(num_constraints)
        c = np.random.rand(num_variables)

        if method is select_dual:
            A *= -1
            b *= -5
            c *= 5
        else:
            A *= 5
            b *= 5
            c *= -5

        try:
            r1 = simplex(A, b, c, method)
            r2 = linprog(c, A_ub=A, b_ub=b)
            if r2.success:
                print(abs(r1 - r2.fun))
        except InfeasibleError:
            print("Infeasible")
        except IterationLimit:
            print("IterationLimit")
