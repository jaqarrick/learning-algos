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

### Running time
The binary search algorithm helps illuminate the concept of _running time_, or the amount of time an algorithm takes to run. With a _linear_ search, i.e. iterating through each number starting from 0, the time is _linear_. With the time complexity of `O(n)`. 

Binary search runs in _logarithmic_ time or `O(log(n))`. If we have 100 elements, the maximum amount of guesses for binary search is log₂(100), roughly 7, if we round up to the nearest integer.

We express run time in "Big O" notation. Where "O" denotes we are using Big O and "n" represents the number of operations. 

Binary search is an efficient algorithm because as the number of items to search increases, the running time does not increase linearly, but rather _decreases_ on average. 

### Bad algorithms: The traveling salesperson problem
Assume a salesperson who is traveling to 5 cities and wants to determine the minimum distance they can travel. Their solution is to look at each possible order, adds up the distance between each city, and choses the lowest distance. Sounds okay... but as the number of cities increases the number of calculations they must perform increases dramatically by the magnitude of `n!`. For example, 5 cities requires 120 calculations. 

This time complexity is _super_-exponential, with `O(n!)`. 
