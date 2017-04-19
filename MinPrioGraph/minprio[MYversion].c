#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minprio.h"

/* Invariants (with names so I can refer to them in my code)
 * SizeOk: numElts <= arrsize and the array has length arrsize+1
 * IC: there are numElts current elements, in arr[1:numElts+1] 
 * IA: for every i<=numElts, arr[i]->pos == i
 * IH: Heap property (Levitin's parental dominance, but reversed for min-queue)
 */

struct minprio {
    int arrsize; 
    int numElts;
    Handle* arr; 
    Comparator comp;
};

MinPrio makeQueue(Comparator comp, int maxsize) {
    MinPrio qp = malloc(sizeof(struct minprio));
    qp->arrsize = maxsize + 1;
    qp->numElts = 0;
    qp->arr = malloc(qp->arrsize *  sizeof(struct handle));
    qp->comp = comp;
    return qp;
}

/* designed for dequeueMin function
 * takes a MinPrio queue and two indeces
 * returns index of min element
 */
int min(MinPrio qp, int left, int right) {
    if(right > qp->numElts && left > qp->numElts) return -1;
    if(left > qp->numElts) return right;
    if(right > qp->numElts) return left;
    void* lefty = qp->arr[left]->content;
    void* righty = qp->arr[right]->content;
    if(qp->comp(lefty, righty) == 0) return left;
    if(qp->comp(lefty, righty) < 0) return left;
    else return right;
}

void printQueue(MinPrio qp) {
    for(int i = 1; i <= qp->numElts;) {
        for(int j = 0; j < i; j++) {
            if(i + j <= qp->numElts) {
                printf("%d ", (int) qp->arr[i+j]->content);
            }
        } 
        printf("\n");
        i = i + i;
    }
    printf("\n\n");
}

void disposeQueue(MinPrio qp) {
    for(int i = 1; i <= qp->numElts; i++) {
        free(qp->arr[i]);
    } 
    free(qp->arr);
    free(qp);
}

void swap(MinPrio qp, int parent, int child) {
    Handle temp = qp->arr[parent];
    int tempPos = qp->arr[child]->pos;
    qp->arr[parent] = qp->arr[child];
    qp->arr[parent]->pos = temp->pos;
    qp->arr[child] = temp;
    qp->arr[child]->pos = tempPos;
}

Handle enqueue(MinPrio qp, void* item) {
    Handle handle = malloc(sizeof(struct handle));
    handle->content = item;
    handle->pos = qp->numElts+1;
    qp->arr[handle->pos] = handle;
    qp->numElts++;
    int parent = handle->pos/2;
    while(parent != 0 && qp->comp(handle->content, qp->arr[parent]->content) < 0) {
        swap(qp, handle->pos, parent); 
        parent = handle->pos/2;
    }
    return handle;
}

int nonempty(MinPrio qp) {
    if(qp->numElts != 0) return 1;
    else return 0;
}

void* dequeueMin(MinPrio qp) {
    void* ret = qp->arr[1]->content;
    swap(qp, 1, qp->numElts);
    free(qp->arr[qp->numElts]);
    qp->numElts--;
    int child = min(qp, 2, 3);
    Handle hand = qp->arr[1];
    while(child != -1 && qp->comp(hand->content, qp->arr[child]->content) > 0) {
        swap(qp, hand->pos, child);
        hand = qp->arr[child];
        child = min(qp, 2*child, 2*child+1); 
    }
    return ret;
}

void decreasedKey(MinPrio qp, Handle hand) {
    int parent = hand->pos/2;
    while(parent != 0 && qp->comp(hand->content, qp->arr[parent]->content) < 0) {
        swap(qp, hand->pos, parent); 
        parent = hand->pos/2;
    }
}

int intComp(int x, int y) {
    if(x == y) return 0;
    if(x > y) return 1;
    else return -1;
}

/*
int main() {
    MinPrio qp = makeQueue((Comparator)intComp, 10);
    enqueue(qp, 5);
    printQueue(qp);
    enqueue(qp, 8);
    printQueue(qp);
    enqueue(qp, 3);
    printQueue(qp);
    enqueue(qp, 1);
    printQueue(qp);     
    enqueue(qp, 2);
    printQueue(qp);
    enqueue(qp, 9);
    printQueue(qp);
    enqueue(qp, 0);
    printQueue(qp);
    enqueue(qp, 4);
    printQueue(qp);
    enqueue(qp, 6);
    printQueue(qp);
    enqueue(qp, 7);
    printQueue(qp);
    
    printf("%d\n\n\n", (int) dequeueMin(qp));
    printQueue(qp);
    qp->arr[9]->content = 0;
    decreasedKey(qp, qp->arr[9]);
    printQueue(qp);
 
    disposeQueue(qp);
} */
