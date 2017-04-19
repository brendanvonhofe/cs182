#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "sorting.c"

/* Demonstration of getting timing measurements for sorting sizeable arrays. 
 * Uses test files from Project Gutenberg http://www.gutenberg.org/
 */


/* assuming i,j in range, swaps the values, in place */
/*
void swap(char* arr[], int i, int j) {
    char* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}*/


/* Muddle with arr[l:r] in place, to kill time.
 * Precondition: 0 <= l <= r <= len(arr).
 * Known bugs: Doesn't crash but does nothing useful, slowly.  Is that a bug?
 */ 
void quacksort(char* arr[], int l, int r) {
    for (int silly = l; silly < r; silly++) 
        for (int sillier = silly; sillier < r; sillier++)
            if (strcmp(arr[silly], arr[sillier]) < 0) 
                swap(arr, silly, sillier);
}

void testSortedNess(char* arr[], int len) {
    int sorted = 1;
    for(int i = 0; i < len-1; i++) {
        if(strcmp(arr[i], arr[i+1]) > 0) {
            sorted = 0;
            printf("%s, %s\n", arr[i], arr[i+1]);
            return;
        }
    }
    if(sorted == 1) printf("It's actually sorted yo!");
    else printf(":(\n");
}


#define NUM_REPEAT 20 /* number of repetitions of a sorting test */


/* Repeatedly run quacksort on the given file, assuming it exists and contains 
 * at most numwords of words, with each word of length at most wordlen. 
 * (A word is just a sequence of characters, delimited by white space.)
 * Print the approximate execution time, including time for copying the array
 * but not the time for loading the file.
 */
void testsort(char* filename, int numwords, int wordlen) {
    char* big[numwords];
    char* copy[numwords];
    char buf[wordlen+1]; // allow for null

    /* load the file into big array (with no error checking!) */

    FILE* file = fopen(filename, "r");
    int actualwords = 0;
    while( fscanf(file, "%s", buf) != EOF ) {
        int buflen = strlen(buf);
        char* buf2 = (char*) malloc((buflen+1) * sizeof(char));
	strcpy(buf2, buf);
        big[actualwords++] = buf2;
    }
    fclose(file);

    printf("testing file  %s ...\n", filename);
    
    /* save start time, repeatedly make a (shallow) copy of the array and sort it */ 

    clock_t begin, end;
    double time_spent;
    begin = clock();

    for (int j = 0; j < NUM_REPEAT; j++) {
        for (int i = 0; i < actualwords; i++)
            copy[i] = big[i];
        quicksort(copy, 0, actualwords);
    }

    /* stop timing and print duration */

    end = clock();
    time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;
    printf("file is %s; time spent on %i sorts is %f \n", 
           filename, NUM_REPEAT, time_spent);

    testSortedNess(copy, actualwords);
    printArray(copy, actualwords);
}        


/* Run some tests.  ALERT: I've hard coded the data sizes. 
*/
int main(int argc, char* argv[]) {

    testsort("statesContig.txt", 267, 3);  
    testsort("3esl.txt", 23427, 21);  
   // testsort("chromosome4.txt", 170617, 61); 
//    testsort("war+peace.txt", 564264, 32); /* May not work in linux-lab */
}





