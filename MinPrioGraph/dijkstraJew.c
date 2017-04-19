//I pledge my honor that I have abided by the Stevens Honor System - Taber McFarlin
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "graph.h"
#include "minprio.h"
#include "graphio.h"

struct vert {
  int num;
  int parent;
  double d;
  int dequeued;
};
typedef struct vert* Vert;

Vert makeVert(int num, int parent, double d) {
  Vert v = (Vert) malloc(sizeof(struct vert));
  v->num = num;
  v->parent = parent;
  v->d = d;
  v->dequeued = 0;

  return v;
}

int compVert(Vert v, Vert u) {
  if (v->d < u->d)
    return -1;
  else if (v->d > u->d)
    return 1;
  else
    return 0;
}

/* Print the lengths and predecessor in a shortest path from s,
 * for each vertex that is reachable from s.  (For vertices that  
 * are not reachable from s, print nothing.)  Also print the 
 * name of the predecessor in a shortest path.
 *
 * Assume GI is non-null and valid.
 * Assume the source number, s, is in the graph GI->graph.
 * 
 * Use Dijkstra's algorithm and an implementation of minprio.h.
 * Use a format like this to show length and predecessor:
 * 
 * Shortest paths from vertex A 
 *   to B is 9.000000 via E
 *   to C is 10.000000 via B
 *   to D is 6.000000 via A
 *   to E is 7.000000 via A
 *   to F is 15.000000 via C
 */
void shortestPaths(GraphInfo GI, int s) {
  Vert v;
  int n = GI->graph->numVerts;
  MinPrio Q = makeQueue((Comparator) compVert, n);
  Handle hands[n];
  Vert V[n];
  int* succ;

  for (int i = 0; i < n; i++) {
    hands[i] = enqueue(Q, makeVert(i, -1, INFINITY));
    V[i] = hands[i]->content;
  }
  V[s]->d = 0;
  decreasedKey(Q, hands[s]);

  for (int i = 0; i < n; i++) {
    v = dequeueMin(Q);
    v->dequeued = 1;
    succ = successors(GI->graph, v->num);
    for (int j = 0; succ[j] != -1; j++)
      if (V[succ[j]]->dequeued == 0 &&
	  v->d + edge(GI->graph, v->num, succ[j]) < V[succ[j]]->d) {
	V[succ[j]]->d = v->d + edge(GI->graph, v->num, succ[j]);
	V[succ[j]]->parent = v->num;
	
	if (V[succ[j]]->dequeued == 0)
	  decreasedKey(Q, hands[succ[j]]);
      }
  }
    //Output: ------------------------------
  printf("Shortest paths from vertex %s\n", GI->vertnames[s]);
    for (int i = 0; i < n; i++)
      if (i != s)
	printf("  to %s is %f via %s\n",
	       GI->vertnames[i], V[i]->d, GI->vertnames[V[i]->parent]);
}

int main() {
    GraphInfo gi = readGraphMakeSymm("states49Contig.txt", MATRIX);
    shortestPaths(gi, 28);
}
