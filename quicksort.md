# Quicksort
Quick sort is one of the fastest sorting algorithms and has, on average `O(n * log(n))` time complexity. This algorithm uses D&C (Divide and Conquer) technique, splitting and recursively sorting a single array into two separate arrays based on a _partition_ point. 

Assume an unsorted array of integers:
```
[3, 222, 1, 4, 9, 2, 44, 22]
```
From here, we'll pick a _partition_. There are several approaches to choosing this, but we can just pick a random index (this is on average the most efficient solution). So let's pick 4. 

For simplicity, we'll move 4 to the end of the list. 
```
[11, 222, 1, 9, 2, 44, 22, 4]
```
Next, we'll keep track of two indices `i` and `j`. We'll iterate over each element and check if it is less than the partition value. If it is not, we do nothing but increment `j` and move on. If it is less, we increment `i` and `j` and swap the value at index `j` with the value at index `i`. 
```
i = -1
j = 0
 _
[11, 222, 1, 9, 2, 44, 22, 4]
```
11 is not less than 4, so move on
```
i = -1
j = 1
     ___
[11, 222, 1, 9, 2, 44, 22, 4]
```
222 is not less than 4, so move on
```
i = -1
j = 2
          _
[11, 222, 1, 9, 2, 44, 22, 4]
```
1 *is* less than 4, so increment i and swap `arr[j]` with `arr[i]`
```
i = 0
j = 2
[1, 222, 11, 9, 2, 44, 22, 4]
```
Then increment `j` and continue

```
i = 0
j = 3
             _
[1, 222, 11, 9, 2, 44, 22, 4]
```

```
i = 0
j = 4
                _
[1, 222, 11, 9, 2, 44, 22, 4]
```

2 is less than 4, so increment `i` and swap:
```
i = 1
j = 4
                _
[1, 2, 11, 9, 222, 44, 22, 4]
```

When we get to the end of the list, we swap the partition point with the value at `i+1`
```
[1, 2, 4, 9, 222, 44, 22, 11]
```
This array is not sorted _but_ we know that the values of the array to the left are all less than 4 and values to the right are greater than 4. So, we can split these up into two separate arrays and perform the same technique:
```
[1, 2] 4 [9, 222, 44, 22, 11]
```
We can perform the same operation on these child arrays:
```
qs([1, 2]) 4 qs([9, 222, 44, 22, 11])
returns:
[1, 2] 4 [9, 11, 22, 44, 222]
 ```

[Full example with random partition index in C](./examples/quicksort.c)