# Learning Algos
This repository contains explanations of common algorithms with examples in C. This guide mostly follows along _[Grokking Algorithms](https://www.manning.com/books/grokking-algorithms)_ chapters. 

## 1. Binary Search
An efficient algorithm with O(log(n)) time complexity. Given an array of sorted values (i.e. numerically, alphabetically) and a target, a program will calculate the middle index of the array, check the value at that index, and continue searching by splitting the array and following the same process for the new values. 

```
arr = [1, 4, 6, 8, 10, 20, 21]
target = 20

1. Calculate middle index, 4
2. Check value at middle index, 8
3. Since 8 < 20, find middle index of upper array
4. Check value at new middle index, and repeat process
```
[Example in C](./examples/binary_search.c)
