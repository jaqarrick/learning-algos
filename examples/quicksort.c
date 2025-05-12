#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high){
    int pivot = arr[high]; // random partition already swapped
    int i = (low - 1);

     for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i+1], &arr[high]);
    return (i + 1); // return index of the partition
}

int randomPartition(int arr[], int low, int high){
    srand(time(NULL));
    int random = low + rand() % (high - low);
    // Swap arr[random] with arr[high]
    swap(&arr[random], &arr[high]);
    return partition(arr, low, high);

}

void quicksort(int arr[], int low, int high){
    if(low < high){
        int partition_index = partition(arr, low, high);
        quicksort(arr, low, partition_index - 1);
        quicksort(arr, partition_index + 1, high);
    }
}
int main() {
    int arr[] = { 33, 1, 4, 3, 100, 30, 2, 3, 1111};
    int size = sizeof(arr) / sizeof(arr[0]);
    printf("Original Array:\n");
    printArray(arr, size);
    quicksort(arr, 0, size - 1);
    printf("Sorted Array\n");
    printArray(arr, size);
    return 0;

}