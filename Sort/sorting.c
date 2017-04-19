#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Takes a char array and two indeces.
 * Swaps the values at the indeces.
 */
void swap(char* arr[], int index1, int index2) {
    char* temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}

void printArray(char* arr[], int len) {
    int i = 0;
    printf("[");
    while(i < len) {
        printf("%s ", arr[i]);
        i++;
    }
    printf("]\n");
}

/* 
 * Takes an an array, a search key, and two indeces
 * Searches the array from [l, r]
 * Returns index where key should be places
 * PRECONDTION: 0 <= l < r < length of arr
 */
int binarySearch(char* arr[], int l, int r, char* key) {
    if(r <= l) return (strcmp(key, arr[l]) > 0)? l+1 : l;
    int m = (l + r) / 2;
    if(strcmp(key, arr[m]) == 0) return m+1;
    if(strcmp(key, arr[m]) > 0) return binarySearch(arr, m+1, r, key);
    return binarySearch(arr, l, m-1, key);
}

void insertionsort(char* arr[], int l, int r) {
    int sorted = l;
    for(int i = l+1; i < r; i++) {
        char* v = arr[i];
        int insPt = binarySearch(arr, 0, i, v);
        for(int j = i-1; j >= insPt; j--) {
            arr[j+1] = arr[j];
        }
        arr[insPt] = v;
    }
}

int partition(char* arr[], int l, int r) {
    char* str = arr[l];
    int i = l - 1;
    int j = r;
    do {
        do {
            i++;
        } while(strcmp(arr[i], str) < 0);
        do {
            j--;
        } while(strcmp(arr[j], str) > 0);
        swap(arr, i, j);
    } while(i < j);
    swap(arr, i, j);
    swap(arr, l, j);
    return j;
}

int partitionPlus(char* arr[], int l, int r, int p) {
    char* str = arr[p];
    int i = l - 1;
    int j = r;
    do {
        do {
            i++;
        } while(strcmp(arr[i], str) < 0);
        do {
            j--;
        } while(strcmp(arr[j], str) > 0);
        swap(arr, i, j);
    } while(i < j);
    swap(arr, i, j);
    swap(arr, l, j);
    return j;
}

int median(char* arr[], int str1, int str2, int str3) {
    if(strcmp(arr[str1], arr[str2]) > 0) {
        if(strcmp(arr[str1], arr[str3]) > 0) {
            if(strcmp(arr[str2], arr[str3]) > 0) return str2;
            return str3;
        } 
        else return str2;
    }
    else {
        if(strcmp(arr[str2], arr[str3]) > 0) {
            if(strcmp(arr[str1], arr[str3]) > 0) return str1; 
            else return str3; 
        }
        else return str2;
    }
}

void quicksort(char* arr[], int l, int r) {
    if(l < r) {
        int split = partition(arr, l, r);
        quicksort(arr, l, split-1);
        quicksort(arr, split+1, r);
    }
}

void quicksortPlus(char* arr[], int l, int r) {
    if(l < r) {
        if(r - l <= 10) insertionsort(arr, l, r);
        else {
            int split = partitionPlus(arr, l, r, median(arr, l, r-1, (l+r)/2));
            quicksort(arr, l, split-1);
            quicksort(arr, split+1, r);
        }
    }
}
/*
int main() {
    char* arr[] = {"gangrene", "caple", "pluto", "plato", "fruit", "apples"};
    quicksortPlus(arr, 0, 6);
    printArray(arr, 6); 
} */
