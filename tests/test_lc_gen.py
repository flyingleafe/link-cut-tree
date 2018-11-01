import sys
import os
import numpy as np
from random import randint
from random import sample

INF = 999999

def write_line(f, x):
    s = str(x) + '\n'
    f.write(s)

fname = sys.argv[1]
n = int(sys.argv[2])
m = int(sys.argv[3])

inp = open(fname + '.in', 'w')
out = open(fname + '.out', 'w')

write_line(inp, n)

vals = np.random.randint(0, INF, size=n, dtype=int)

for i in range(0, n):
    write_line(inp, vals[i])

# tree node is a value + link to parent
forest = list(map(lambda v: [v, None], vals))

# maintain the set of forest roots
roots = set(range(0, n))

def ancestors(i):
    while i is not None:
        i = forest[i][1]
        yield i

def mk_get(k):
    real_v = forest[k][0]
    write_line(inp, "get %d" % k)
    write_line(out, real_v)

def mk_add(k, c):
    i = k
    while i is not None:
        forest[i][0] += c
        i = forest[i][1]
    write_line(inp, "add %d %d" % (k, c))
    write_line(out, "added")

def mk_min(k):
    i = k
    res = None
    while i is not None:
        res = forest[i][0] if res is None else min((forest[i][0], res))
        i = forest[i][1]

    if res is None:
        raise Exception("Wrong node idx passed")

    write_line(inp, "min %d" % k)
    write_line(out, res)

def mk_link(i, j):
    if forest[j][1] is not None:
        raise Exception("Trying to link non-root")

    forest[j][1] = i
    roots.remove(j)
    write_line(inp, "link %d %d" % (i, j))
    write_line(out, "linked")

def mk_cut(k):
    forest[k][1] = None
    roots.add(k)
    write_line(inp, "cut %d" % k)
    write_line(out, "cut")

def mk_lca(i, j):
    i_ancs = set(ancestors(i))
    _j = j
    lca = None
    while _j is not None:
        if _j in i_ancs:
            lca = _j
            break
        _j = forest[_j][1]

    write_line(inp, "lca %d %d" % (i, j))
    write_line(out, lca)

write_line(inp, m)

for i in range(0, m):
    k = randint(0, n-1)
    act = randint(0, 5)
    if act == 0:
        mk_get(k)
    elif act == 1:
        c = randint(-10, 10)
        mk_add(k, c)
    elif act == 2:
        mk_min(k)
    elif act == 3:
        [v] = sample(roots, 1)
        mk_link(k, v)
    elif act == 4:
        mk_cut(k)
    elif act == 5:
        v = randint(0, n-1)
        mk_lca(k, v)
