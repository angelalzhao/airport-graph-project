# performs pagerank on the graph represented by airports_small.dat and routes_small.dat using the traditional matrix implementation
# outputs used in last pagerank test case to verify that adjacency list implementation has the same result as matrix implementation
import numpy as np
# JFK 0
# ORD 1
# LAX 2
# CLT 3
# ATL 4
# adjacency matrix with columns of zeros replaced with all ones
A = np.array([[1, 0, 0, 0, 1], [1, 0, 0, 0, 1], [1, 1, 0, 0, 1], [1, 1, 0, 0, 1], [1, 0, 1, 1, 1]])
# normalizing so that each column adds up to 1
M = A / A.sum(axis = 0)
# applying a decay of 0.85
G = 0.85 * M + 0.15 * np.ones((5, 5)) / 5
x = np.array([0.2, 0.2, 0.2, 0.2, 0.2])
while True:
    prev = x
    x = G@x
    if np.amax(np.abs(prev - x)) < 0.000005:
        break
print(x)