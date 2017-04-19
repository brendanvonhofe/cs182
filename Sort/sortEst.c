#include <stdio.h>
#include <stdlib.h>
#include "sorting.c"

int main() {
    char* arr[] = {"hi", "hello", "woah", "cool", "fun", "rad", "radical", "righteous"};
    insertionsort(arr, 0, 8);
    return 0;
}
