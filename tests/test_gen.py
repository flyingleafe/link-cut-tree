import sys
import os
import subprocess
import time
from random import randint

def write_line(f, x):
    s = str(x) + '\n'
    f.write(s)

fname = sys.argv[1]
n = int(sys.argv[2])
m = int(sys.argv[3])

inp = open(fname + '.in', 'w')
out = open(fname + '.out', 'w')

mp = {}

sum_add = 0

write_line(inp, n + m)
for i in range(0, n):
    k, v = (randint(0, n), randint(0, 100))
    mp[k] = v
    #start = time.time()
    write_line(inp, "add %d %d" % (k, v))
    write_line(out, "added")
    #end = time.time()
    #sum_add += end - start

#print("average time for addition is %f" % (sum_add / n))

#sum_find = 0

for i in range(0, m):
    k = randint(0, n)
    real_v = mp.get(k, None)
    #start = time.time()
    write_line(inp, "find %d" % k)
    write_line(out, real_v)
    #ans = out.readline()
    #end = time.time()
    #sum_find += end - start   
    
    #v = None if ans == "None" else int(ans)	
    #if v != real_v:
    #    print("ERROR: ", real_v, v)

#print("average time for finding an element is %f" & (sum_find / m))
