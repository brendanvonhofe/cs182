#ifndef GRAPHALGS_H
#define GRAPHALGS_H
#include "graph.h"

/* rev 4/18/16 DN assume graphs nonempty; MST edges have orig weight */

/* Construct a graph that is the transitive closure of g.
 * Assume g is non-null pointer to non-empty graph.
 * Ignore the weights of g.  The weights of the new graph
 * should all be 0. There is an edge u->v in the new graph
 * if and only if there is a path from u to v in g.
 * 
 * Implement this using Warshall's algorithm and matrix 
 * representation for the result graph.
 */
Graph transClose(Graph g);


/* Assume g is non-null pointer to non-empty graph.
 * Assume g is a symmetric graph, so it can be 
 * considered as an undirected graph.  
 * (In particular, the weight u->v must equal weight v->u
 * for every u,v.)
 * Assume the weights are not INFINITY and g is connected.
 * 
 * Return a minimum spanning tree, as a new graph.
 * (So it will have the same number of vertices, 
 * but only some of the edges.  Weights same as in g.)
 * 
 * Implement this using Prim's algorithm.
 */
Graph minSpanTree(Graph g);

#endif
