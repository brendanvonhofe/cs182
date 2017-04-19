/* Simple interface for weighted directed graphs.

   The vertices are numbered 0 .. n-1 for some fixed n.
   Edges have nonnegative weights.

   The two standard graph representations are supported.
   Function cloneGraph() can be used to convert between 
   representations.

   Precondition: all functions except makeGraph assume their
   Graph argument is non-null.

*/

#define MATRIX 0
#define LIST 1

#define OK 0
#define ERROR 1

#ifndef _Graph
#define _Graph

struct graph;
typedef struct graph* Graph;

#endif

/* Make an empty graph with n vertices, using either adjacency matrix 
   or adjacency lists depending on whether rep==MATRIX or rep==LIST.
   Precondition: n>=1 and rep is MATRIX or LIST.
*/
Graph makeGraph(int n, int rep);

/* make a copy of g, but using the representation
   specified by rep (which is assumed to be MATRIX or LIST) 
*/
Graph cloneGraph(Graph G, int rep);

/* free the graph object and all its resources.
   Postcondition: g is no longer a valid pointer. 
   Alert: the caller should set their variable to null.
*/
void disposeGraph(Graph G);

/* number of vertices */
int numVerts(Graph G);

/* add edge from source to target with weight w, and return
   OK, if source and target are valid vertex numbers and
   there was no edge from source to target initially.
   Otherwise, make no change and return ERROR. 
*/
int addEdge(Graph G, int source, int target, float w);

/* delete edge from source to target, and return
   OK, if there was an edge from source.
   Otherwise, make no change and return ERROR. 
*/
int delEdge(Graph G, int source, int target);

/* return weight of the edge from source to target,
   if there is one; otherwise return INFINITY.
   Return -1.0 if source or target are out of range.
*/
float edge(Graph G, int source, int target);

/* a freshly allocated array with the successor
   vertices of source, if any, followed by an entry with -1
   to indicate end of sequence.
   Precondition: source is in range.
*/
int* successors(Graph G, int source);

/* a freshly allocated array with the predecessor
   vertices of target, if any, followed by an entry with -1
   to indicate end of sequence.
   Precondition: target is in range.
*/
int* predecessors(Graph G, int target);

