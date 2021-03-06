import sys
import os
import numpy as np
from random import randint

INF = 999999

def write_line(f, x):
    s = str(x) + '\n'
    f.write(s)

fname = sys.argv[1]
n = int(sys.argv[2])
m = int(sys.argv[3])

inc_min = len(sys.argv) > 4 and sys.argv[4] == "inc_min"

inp = open(fname + '.in', 'w')
out = open(fname + '.out', 'w')

write_line(inp, n)

arr = np.random.randint(0, INF, size=n, dtype=int)

for i in range(0, n):
    write_line(inp, "%d" % arr[i])

def mk_find(k):
    real_v = arr[k]
    write_line(inp, "find %d" % k)
    write_line(out, real_v)

def mk_increase(a, b, v):
    inc_map = np.full((b - a)+1, v)
    arr[a:b+1] += inc_map
    write_line(inp, "increase %d %d %d" % (a, b, v))
    write_line(out, "increased")

def mk_min(k):
    m = np.min(arr[0:k+1])
    write_line(inp, "min %d" % k)
    write_line(out, m)

write_line(inp, m)

for i in range(0, m):
    k = randint(0, n-1)
    #start = time.time()
    if inc_min:
        act = randint(0, 2)
        if act == 0:
            mk_find(k)
        elif act == 1:
            a = randint(0, k)
            v = randint(-10, 10)
            mk_increase(a, k, v)
        elif act == 2:
            mk_min(k)
    else:
        mk_find(k)

