#include <stdio.h>
#include <stdlib.h>

int binary_search(int arr[], int arr_len, int target) {
  int left = 0; // lowest index
  int right = arr_len - 1;

  while (left <= right) {
    // calculate mid point
    int mid = left + (right - left) / 2;
    // check value at mid point
    if (arr[mid] == target) {
      // return index if target found
      return mid;
    }

    if (arr[mid] < target) {
      // if the value at mid point is less than target
      // remove bottom half of array
      left = mid + 1;
    } else if (arr[mid] > target) {
      // if the value at mid point is greater than the target
      // remove top half of array
      right = mid - 1;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Too few arguments.\n");
    exit(1);
  }

  int target = atoi(argv[1]);
  int arr[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 72, 91};
  int size = sizeof(arr) / sizeof(arr[0]);
  int found = binary_search(arr, size, target);

  if (found < 0) {
    printf("Not found\n");
  } else {
    printf("Target %d found at index %d\n", target, found);
  }

  return 0;
}
