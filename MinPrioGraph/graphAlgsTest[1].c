#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"
#include "graphAlgs.h"
#include "graphio.h"

/* Tests for Transitive Closure
 *
 * Note: directly reads and writes the graph field of GraphInfo objects,
 * in order to print the spanning tree.
 * (Perhaps the graphio API can be improved for this?)
 *
 * Prints transitive closure of a graph to stdout
 * 
 */

void testTransClose(char *path)
{

    printf("Testing %s \n", path);
    GraphInfo gi = readGraph(path, MATRIX);

    GraphInfo gi2 = (GraphInfo)malloc(sizeof(struct graphinfo));
    gi2->vertnames = gi->vertnames;
    gi2->graph = transClose(gi->graph);

    printf("\nTransitive closure for %s:\n", path);
    writeGraph(gi2);
    disposeGraph(gi->graph);
    disposeGraph(gi2->graph);

}

int main(int argc, char *argv[])
{
    testTransClose("graph1.txt");
    testTransClose("prereqs.txt");
    testTransClose("statesContig.txt");

    return 0;

}
