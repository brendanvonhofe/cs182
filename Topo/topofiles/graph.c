#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include <string.h>
#include <assert.h>
#include "graph.h"

/* PARTIAL IMPLEMENTATION OF graph.h *************************************** */


/* list node to represent an edge, for adjacency list */
struct eNode {
    float wt;
    int target; 
    struct eNode* next; 
};
typedef struct eNode* ENode;


/* The graph; see graph.h for type Graph. 
   Depending on the repType field, one of the fields adjM or aLists is not used.
   (We could use a union for this, but it's not worth the trouble.)
   Invariant: each adjacency list begins with a dummy node.
*/
struct graph {
    short repType; /* MATRIX or LIST representation */
    int numVerts;
    float* adjM; /* the adjacency matrix, size numVerts*numVerts */
    ENode* aList; /* array of adjacency lists; length numVerts */
};


/* Make an empty graph with n vertices, using either adjacency matrix 
   or adjacency lists depending on whether rep==MATRIX or rep==LIST.
   Precondition: n>=1 and rep is MATRIX or LIST.
*/
Graph makeGraph(int n, int rep) {
    Graph g = (Graph) malloc(sizeof(struct graph));
    g->numVerts = n;
    g->repType = rep;
    if (rep == MATRIX) {
	g->aList = NULL;
	g->adjM = (float*) malloc(n * n * sizeof(float));
	for (int src = 0; src < n; src++)
	    for (int trg = 0; trg < n; trg++)
		*(g->adjM + (n*src) + trg) = INFINITY; // (g->adjM)[src][trg] = INFINITY;
    } else { // repType is LIST
	g->adjM = NULL;
	g->aList = (ENode*) malloc(n * sizeof(ENode));
	// initialize with dummy nodes
	for (int src = 0; src < n; src++) {
	    ENode node = (ENode) malloc(sizeof(struct eNode));
	    node->wt = 0.0; 
	    node->target = 0; 
	    node->next = NULL;
	    *(g->aList + src) = node;
	}
    }
    return g;
}


/* make a copy of g, but using the representation
   specified by rep (which is assumed to be MATRIX or LIST) 
   Known bug: not implemented
*/
Graph cloneGraph(Graph g, int rep){ 
    printf("cloneGraph not supported.\n");
    exit(1); 
}


/* free the graph object and all its resources.
   Postcondition: g is no longer a valid pointer. 
   Alert: the caller should set their variable to null.
   Known bug: does not free nodes of adjacency lists.
*/
void disposeGraph(Graph g) {
    if (g->repType == MATRIX)
	free(g->adjM);
    else  // repType is LIST
	free(g->aList);
    free(g);
}


/* number of vertices */
int numVerts(Graph g){ 
    return g->numVerts;
}


/* add edge from source to target with weight w, and return
   OK, if source and target are valid vertex numbers and
   there was no edge from source to target initially.
   Otherwise, make no change and return ERROR. 
*/
int addEdge(Graph g, int source, int target, float w) {
    int n = g->numVerts;
    if (source < 0 || source >= n || target < 0 || target >= n)
	return ERROR;
    if (g->repType == MATRIX) {
	if ( *(g->adjM + (n*source) + target) != INFINITY )
	    return ERROR; // an edge was already present
	*(g->adjM + (n*source) + target) = w;
    } else { // rep is LIST; attach at end of list
	ENode p = *(g->aList + source);
	while (p->next != NULL && p->next->target != target) 
	    p = p->next;
	if (p->next != NULL )
	    return ERROR; // an edge was already present
	else {
	    ENode node = (ENode) malloc(sizeof(struct eNode));
	    p->next = node;
	    node->wt = w;
	    node->target = target;
	    node->next = NULL;
	}
    }
    return OK;
}    

/* delete edge from source to target, and return
   OK, if there was an edge from source.
   Otherwise, make no change and return ERROR. 
*/
int delEdge(Graph g, int source, int target) { 
    int n = g->numVerts;
    if (source < 0 || source >= n || target < 0 || target >= n)
	return ERROR;
    if (g->repType == MATRIX) {
	if ( *(g->adjM + (n*source) + target) == INFINITY )
	    return ERROR;
	*(g->adjM + (n*source) + target) = INFINITY;
    } else { // LIST
	ENode follow = *(g->aList + source);
	ENode p = follow->next; // skip dummy node
	while (p != NULL && p->target != target) {
	    follow = p;
	    p = p->next;
	}
	if (p == NULL)
	    return ERROR; 
	else {
	    follow->next = p->next;
	    free(p);
	}
    }
    return OK;
}


/* return weight of the edge from source to target,
   if there is one; otherwise return INFINITY.
   Return -1.0 if source or target are out of range.
*/
float edge(Graph g, int source, int target) {
    int n = g->numVerts;
    if (source < 0 || source >= n || target < 0 || target >= n)
	return -1.0; 
    if (g->repType == MATRIX) {
	return *(g->adjM + (n * source) + target);
    } else { // LIST
	ENode p = *(g->aList + source);
	p = p->next; // skip dummy node
	while (p != NULL && p->target != target)   
	    p = p->next;
	if (p == NULL)
	    return INFINITY; 
	else 
	    return p->wt;
    }
}


/* a freshly allocated array with the successor
   vertices of source, if any, followed by an entry with -1
   to indicate end of sequence.
   Precondition: source is in range.
*/
int* successors(Graph g, int source) {
    int n = g->numVerts;
    int succ[n]; // temporary array, on the stack, big enough for all possible successors 
    int nsucc = 0; // actual number of successors

    /* get precedessors into temporary array in the current stack frame */
    if (g->repType == MATRIX) {
	for (int trg = 0; trg < g->numVerts; trg++) 
	    if ( *(g->adjM + (n*source) +trg) != INFINITY ) 
		succ[nsucc++] = trg;
    } else { // LIST
	ENode p = *(g->aList + source);
	p = p->next; // skip dummy node
	while (p != NULL ) {
	    succ[nsucc++] = p->target;
	    p = p->next;
	}
    }
    /* allocate and return an array the right size */
    int* result = (int*) malloc((nsucc + 1) * sizeof(int));
    for (int i = 0; i < nsucc; i++)
	result[i] = succ[i];
    result[nsucc] = -1;        
    return result;
}


/* a freshly allocated array with the predecessor
   vertices of target, if any, followed by an entry with -1
   to indicate end of sequence.
   Precondition: target is in range.
   Known bug: not implemented for LIST.
*/
int* predecessors(Graph g, int target) {
    if (g->repType == LIST) {
	fprintf(stderr, "graph predecessors not implemented for LIST representation.\n");
	exit(1);
    } // else MATRIX
    /* get precedessors into temporary array in the current stack frame */
    int n = g->numVerts;
    int pred[n]; // temporary array, big enough for all possible predecessors
    int npred = 0; // number of predecessors
    for (int src = 0; src < g->numVerts; src++) 
        if ( *(g->adjM + (n*src) +target) != INFINITY ) 
            pred[npred++] = src;
    /* allocate and return an array the right size */
    int* result = (int*) malloc((npred + 1) * sizeof(int));
    for (int i = 0; i < npred; i++)
	result[i] = pred[i];
    result[npred] = -1;        
    return result;
}




