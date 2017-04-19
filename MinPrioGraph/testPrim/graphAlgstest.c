#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "graphio.h"
#include "graphAlgs.h"

/* Tests for Prim's MST algorithm.
 * Note: directly reads and writes the graph field of GraphInfo objects,
 * in order to print the spanning tree.
 * (Perhaps the graphio API can be improved for this?)
 *
 * TODO: make additional tests by permuting the vertex numbering in test files.
 */

/* Test MST on a graph.
 * Assume path is nonnull name of nonempty connected graph.
 * Assume the graph has NO symmetric edges, since
 * we will make it symmetric.
 */
void testMST(char* path) {
    GraphInfo gi; 
    printf("Testing %s \n", path); 
    /* load and print the graph */
    gi = readGraphMakeSymm(path, LIST);
    writeGraph(gi);
    /* compute the MST and print that */
    Graph orig = gi->graph; 
    Graph tree = minSpanTree(orig);
    printf("\n A min spanning tree for %s:\n", path);
    gi->graph = tree; 
    writeGraph(gi);
    disposeGraph(orig);
    disposeGraph(tree);
}

int main() {

    printf("graphAlgtest: TESTING PRIM\n");

    testMST("triangleMST.txt"); // a simple graph

    testMST("LevitinPrim.txt"); // the graph example in Levitin book 

    testMST("sixMST.txt"); // slightly larger graph

    /* Here's a bigger graph.  It's connected, because Alaska and Hawaii 
     * are not included.  Unlike the previous examples, this one does
     * NOT have a unique MST. */
    testMST("states49Contig.txt"); // bigger graph, uniform weights

}

