# cache-oblivious-DP

This repository implements cache-oblivious dynamic programming algorithms, including Longest Common Subsequence, Floyd-Warshall, and Gaussian elimination. This repository also implements a framework to implement further cache-oblivious dynamic programming algorithms.

## Cache-Oblivious Dynamic Programming 

Cache-oblivious algorithms can achieve asymptotically optimal bounds on the number of I/Os performed. Because cache misses are often as expensive as several instructions, cache-oblivious algorithms can often be faster than other implementations by exploiting spatial and temporal locality, and reducing the number of cache misses made. This is especially true for large inputs that do not fit in cache.

## GEP Framework 

This respository implements the Gaussian elimination paradigm as described in [Cache-Oblivious Dynamic Programming](http://www.cs.utexas.edu/~vlr/papers/soda06.pdf) by Chowdhury and Ramachandran. This framework can be used for general O(n^3) dynamic programming algorithms that have updates of the form 

```
 X[i][j] = f(X[i][j], X[i][k], X[k][j], X[k][k])
```

In order to use this framework, you can implement the UPDATE_F and UPDATE_EXISTS_F functions as defined in `gep.c`. See `floyd_warshall/floyd_warshall_with_gep.c` as an example.





