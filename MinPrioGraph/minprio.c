#include "minprio.h"
#include <stdlib.h>
#include <assert.h>

/* An implementation of minprio.h */
struct minprio {
    int maxsize;
    int numElts;
    Handle* arr;
    Comparator comp;
};





/* Invariants
 * SizeOk: 0 <= numElts <= arrsize and the array has length arrsize+1
 * IC: there are numElts current elements, in arr[1:numElts+1]
 * IA: for every i<=numElts, arr[i]->pos == i
 * IH: Heap property (Levitin's parental dominance, but reversed for min-queue)
 */


/* API function, see minprio.h */
MinPrio makeQueue(Comparator comp, int maxsize) {
    MinPrio mq = (MinPrio) malloc(sizeof(struct minprio));
    mq->maxsize = maxsize;
    mq->numElts = 0;
    mq->comp = comp;
    mq->arr = (Handle*) malloc((maxsize + 1) * sizeof(Handle)); /* entry 0 unused */
    return mq;
}

/* swap items, maintaining IA */
void swap(MinPrio q, int i, int j) {
    Handle tmp = q->arr[i];
    q->arr[i] = q->arr[j];
    q->arr[i]->pos = i;
    q->arr[j] = tmp;
    q->arr[j]->pos = j;
}

/* API function, see minprio.h */
Handle enqueue(MinPrio q, void* item) {
    assert(q!=NULL); assert(item!=NULL);
    assert(q->numElts < q->maxsize);

    Handle h = (Handle) malloc(sizeof(struct handle));
    h->content = item;

    /* put new item in last position */
    q->numElts++;
    int pos = q->numElts;
    q->arr[pos] = h;
    h->pos = pos; // maintain IA

    /* sift upward, maintaining: IH except possibly at pos */
    while ((pos > 1)
           && (q->comp(q->arr[pos/2]->content,
                       q->arr[pos]->content) > 0)) {
        swap(q, pos, pos/2);
        pos = pos/2;
    }
    return h;
}


/* API function, see minprio.h */
int nonempty(MinPrio qp) {
    return qp->numElts > 0 ? 1 : 0;
}

/* dequeue and return minimum and free its handle */
void* dequeueMin(MinPrio q) {
    assert(q!=NULL);  assert(q->numElts > 0);

    /* get minimum and free its handle */
    Handle h = q->arr[1];
    void* minElt = h->content;
    h->content = NULL;
    free(h);

    /* decrease number of elts and return if none remain */
    q->numElts--;
    if (q->numElts == 0)
        return minElt;

    /* Some elements remain, so move last one to root
     * and repair invariants. */

    q->arr[1] = q->arr[q->numElts+1];
    q->arr[1]->pos = 1; // maintain IA

    int pos = 1;
    /* sift downward
     * invar: IH holds at all positions except possibly at pos */
    while (2*pos <= q->numElts) {
        /* set m to smallest child */
        int m;
        if ((2*pos + 1 > q->numElts) // pos only has left child?
            || (q->comp(q->arr[2*pos]->content,
                        q->arr[2*pos + 1]->content) <= 0)) // left child smaller?
            m = 2*pos;
        else
            m = 2*pos + 1;
        /* swap with smallest child if parent exceeds child */
        if (q->comp(q->arr[pos]->content,
                    q->arr[m]->content
                    ) > 0) { // parent exceeds child
            swap(q, pos, m);
            pos = m;
        }
        else /* IH holds at pos */
            break;
    }
    /* return the minimum */
    return minElt;
}


/* API function, see minprio.h */
void decreasedKey(MinPrio q, Handle h) {
    assert(q!=NULL); assert(h!=NULL);

    int pos = h->pos;
    while ((pos > 1) && (q->comp(q->arr[pos/2]->content,
                                 q->arr[pos]->content) > 0)) {
        swap(q, pos, pos/2);
        pos /= 2;
    }
}


/* API function, see minprio.h */
void disposeQueue(MinPrio qp) {
    /* free any remaining handles */
    for (int i = 1; i < qp->numElts; i++)
        free(qp->arr[i]);
    /* free the array */
    free(qp->arr);
    /* free the queue */
    free(qp);
}
