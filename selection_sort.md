# Selection Sort

## Arrays vs linked lists
Arrays hold contiguous elements in memory, while linked lists contain elements which contain the address in memory of the next item.

|          | Arrays | Lists |
|----------|----------|----------|
| Reading    | O(1)   | O(n)   |
| Insertion    | O(n)  | O(1)   |
| Deletion    | O(n)   | O(1)   |


## Selection Sort
Suppose you want to sort an array from highest number to smallest number.
```
[1, 33, 444, 4, 5]
```
A straightforward solution is to iterate through the array and find the highest number. Then add that number to a new list and remove it from the current array. Then iterate over the array again, repeating the same process until there are no more elements.

```
[1, 33, 4, 5]

New array
[444]

Repeat
[1, 4, 5]
[444, 33]
And so on
```

This works fine, but is expensive. It has `O(n^2)` time complexity because for every number of items in the array `n` you have to perform the operation `n` times. 

[Example of selection sort, using implemented linked list in C](./examples/selection_sort.c)

