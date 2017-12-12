# Longest Common Subsequence

This directory implements a cache-oblivious LCS algorithm as well as a naive algorithm, both in O(n<sup>2</sup>) space.

The algorithm is described in [the paper] (http://www.cs.utexas.edu/~vlr/papers/soda06.pdf) "Cache-Oblivious Dynamic Programming" by Rezaul Alam Chowdhury and Vijaya Ramachandran. 

The input is of the form
```
x_length
y_length
x_input
y_input
```

as seen in sample_lcs_input. Then you can run `./lcs -f sample_lcs_input` to print out the length and contents of a longest common subsequence.