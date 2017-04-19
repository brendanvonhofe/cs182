#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "dfstest.c"



/* Assuming the file exists and is an acyclic graph, print the vertices in a topological order 
 * Use the algorithm that finds no-predecessor vertices and deletes their successor edges. 
 * ALERT: modifies the graph by deleting edges.
 * 
 * Solves topocycleExercise.c */
void toposort2(GraphInfo gi) {
    Graph g = gi->graph;
    int numV = numVerts(g);
    short done[numV]; // 1 if vertex already printed, otherwise 0
    for (int i = 0; i < numV; i++) 
        done[i] = 0;

    for (int numPrinted = 0; numPrinted < numV; numPrinted++) {

        /* set noPred to a vertex with no predecessors and not already done */
        int noPred;
        int* ps; // predecessor list
        for (int v = 0; v < numV; v++) {
            if (!done[v]) {
                ps = predecessors(g, v);
                if (ps[0] == -1 ) { // list is empty?
                    noPred = v;
                    break;
                }
            }
        }

        /* print noPred and mark as done */ 
        printf("%s ", gi->vertnames[noPred]);
        done[noPred] = 1;

        /* get successors of noPred and delete them */
        int* snoPred = successors(gi->graph, noPred);
        for (int i = 0; snoPred[i] != -1; i++) 
            delEdge(gi->graph, noPred, snoPred[i]);
    }
    
    /* cleanup */
    printf("\n");
}


int main() {

    char* filepath;
    GraphInfo gi;

    filepath = "cycle3.txt";
    gi = readGraph(filepath, MATRIX);
    printf("Topological sort by predecessor technique %s\n", filepath);
    toposort2(gi); 
    printf("\nTopological sort by DFS technique %s\n", filepath);
    gi = readGraph(filepath, MATRIX); // MUST read again or clone original since toposort2 deletes
    topocycle(gi);


    filepath = "LevitinTopo.txt";
    gi = readGraph(filepath, MATRIX);
    printf("Topological sort by predecessor technique %s\n", filepath);
    toposort2(gi);
    printf("\nTopological sort by DFS technique %s\n", filepath);
    topocycle(readGraph(filepath, MATRIX));


    filepath = "prereqs.txt";
    gi = readGraph(filepath, MATRIX);
    printf("\nTopological sort by predecessor technique %s\n", filepath);
    toposort2(gi);
    printf("\nTopological sort by DFS technique %s\n", filepath);
    topocycle(readGraph(filepath, MATRIX));


    filepath = "prereqCatch22.txt";
    printf("\nCyclic graph %s\n", filepath);
    topocycle(readGraph(filepath, LIST));


    /* NOTE: not disposing of the graphs */ 

    return 0;
}
