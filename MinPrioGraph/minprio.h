#ifndef minprio_H
#define minprio_H

/* min-priority queues ("addressable")
 * Items in a given queue are struct pointers that should be
 * comparable by the comparator provided to makeQueue.
 *
 * Implement using an array heap as in Levitin 6.4 (but MIN).
 *
 * Items in the queue have type Enquable which enables the queue to
 * determine, from an item pointer, what is its current index.
 * Clients should not depend on this field.
 *
 */


/* the queue type */
struct minprio;
typedef struct minprio* MinPrio;


/* handles for efficient access to enqueued items */
struct handle {
    int pos; /* current pos in the queue; not for client use */
    void* content; /* the client's data */
};
typedef struct handle* Handle;


/* type and contract for comparison function
 * Assumes lhs and rhs non-null.
 * Returns <0, =0, >0 according to whether lhs<rhs, lhs==rhs, lhs>rhs */
typedef int (*Comparator)(void* lhs, void* rhs);


/* make an empty queue
 * Items will be compared using comp.
 *
 * It' the client's responsibility to ensure size is never exceeded.
 */
MinPrio makeQueue(Comparator comp, int maxsize);


/* dispose of memory owned by the queue
 * Namely: the queue object, the array, and the Handles.
 * The Handle contets are the responsibility of the client.
 */
void disposeQueue(MinPrio qp);


/* enqueue
 * Assumes queue currently contains less than maxsize elements.
 * Assumes qp and item non-null.
 * Returns a Handle containing the item, for use with decreaseKey.
 */
Handle enqueue(MinPrio qp, void* item);


/* 1 if queue has elements, else 0 (assuming qp non-null) */
int nonempty(MinPrio qp);


/* dequeue and return a minimum element according to the comparator.
 * Assumes qp non-null and queue nonempty */
void* dequeueMin(MinPrio qp);


/* decrease the item's key
 * Must be called whenever comparison value may have changed.
 * Must only decrease comparison value (i.e. raise priority).
 *
 * Assumes qp non-null and hand is in the queue.
 */
void decreasedKey(MinPrio qp, Handle hand);



#endif
