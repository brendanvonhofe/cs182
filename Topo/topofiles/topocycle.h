#include "graph.h"
#include "graphio.h"

/* Whether the graph has a cycle.  
   If so, print the vertices of a cycle, using the vertex names not their numbers.
   (Just print one cycle, even if there are several.)
   If not cyclic, print all the vertex names, in a topologically sorted order.

   For this purpose, we use Levitin's definition of a cycle, which does not include 
   self-loops, and which includes the starting vertex also at the end of the path.
   So the shortest cycle has three vertices: A B A where there are edges A->B and B->A.
*/
void topocycle(GraphInfo gi);
