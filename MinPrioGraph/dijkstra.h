#include "graphio.h"

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
void shortestPaths(GraphInfo GI, int s);
