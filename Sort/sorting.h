/****************************************************************
Comparison of sorting algorithms.

For simplicity, only string arrays are considered,
although the sorting methods are not specific to strings.

******************************************************************/


/* Insertion sort
 *
 * Sort an array segment, in place:
 * Assume 0 <= l <= r and arr has length at least r.
 * Assume the elements of arr are null-terminated strings.
 * Permute the elements, in place, to put them in non-decreasing order.
 * 
 * Use the insertion sort algorithm, with binary search to find the insertion point.
 */
void insertionsort(char* arr[], int l, int r);


/* Quicksort: same specification, but use the recursive quicksort algorithm.
 * Use a two-sided partition algorithm, as discussed in class.
 * You can use the one in the textbook, or some variation.
 */
void quicksort(char* arr[], int l, int r);


/* QuicksortPlus: same specification, but use the following form of recursive quicksort:
 * 
 * If r - l <= 10, use insertionsort.
 * Otherwise, use quicksort, choosing the pivot element as the median value of 
 * arr[l], arr[r-1], and arr[(l+r)/2].
 */
void quicksortPlus(char* arr[], int l, int r);


/* Assume arr has length at least len.
 * For each of the three sort functions, run it 20 times on arr, and print the total time.
 */ 
void compareSorts(char* arr[], int len);




