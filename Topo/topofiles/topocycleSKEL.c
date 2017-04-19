#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <assert.h>

//#include "graph.h"
//#include "graphio.h"
//#include "dfs.h"
#include "topocycle.h"
//#include "dfs.c"

#define ACYCLIC 0
#define CYCLIC 1

/*************************
 * DFS facts
 * 
 * For either a digraph or even undirected graph, u is a proper ancestor of v
 * in the DFS forest iff discover[u] < discover[v] < finish[v] < finish[u].
 * 
 * A 'back edge' goes from a vertex to an ancestor of that vertex.
 * A graph is cyclic if and only if the DFS forest has a back edge.
 *
 * To topologically sort an acyclic graph, order the vertices in reverse
 * of the 'finish' order.  (Can do that during DFS by pushing finished vertices
 * onto a linked list.  Or do it later by later by sorting, but that costs more.)
 * ************************ */ 



/* whether anc is a proper ancestor of v (i.e., ancestor and anc!=v) 
 * Assume u, v are vertices of di->graph. */
int proper_ancestor(DFSinfo di, int anc, int v) {
    return anc!=v &&
        di->discover[anc] < di->discover[v] &&
        di->discover[v] < di->finish[v] &&
        di->finish[v] < di->finish[anc];
}


void topocycle(GraphInfo gi) {
    if(gi == NULL) return;
    DFSinfo di = DFS(gi->graph);
    int anc = -1;
    int v = -1;
    int ancCount = 0;
    int* ancPairs;
    int n = gi->graph->numVerts;
    int cyclicity = ACYCLIC; 
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(proper_ancestor(di, i, j) == 1 && edge(gi->graph, j, i) != INFINITY) {
                ancCount++;
                cyclicity = CYCLIC;
                anc = i;
                v = j;
            }
        }
    }
    ancPairs = malloc(2*ancCount*sizeof(int));
    ancCount = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(proper_ancestor(di, i, j) == 1 && edge(gi->graph, j, i) != INFINITY) {
                ancPairs[ancCount] = i;
                ancCount++;
                ancPairs[ancCount] = j;
                ancCount++;
            }
        }
    } 
    //printf("Ancestor Arr\n");
    //for(int i = 0; i < ancCount - 1; i = i+2) {
      //  printf("%d %d   ", ancPairs[i], ancPairs[i+1]);
    //}
    printf("\n");
    int* cycle;
    int cycleLen;
    if(cyclicity == CYCLIC) {
        int vert = v;
        printf("Cycle: ");
        while(edge(gi->graph, v, vert) == INFINITY ||
                edge(gi->graph, v, vert) == -1.0) {
            cycleLen++;
            vert = di->parent[vert]; 
        }
        cycle = malloc(cycleLen*sizeof(int));
        cycleLen = 0;
        vert = v;
        while(edge(gi->graph, v, vert) == INFINITY ||
                edge(gi->graph, v, vert) == -1.0) {
            cycle[cycleLen] = vert;
            cycleLen++;
            vert = di->parent[vert]; 
        }
        printf("%d ", vert);
        for(int i = cycleLen-1; i >= 0; i--) {
            printf("%d ", cycle[i]); 
        }
        printf("%d\n\n", vert);
    }
    if(cyclicity == ACYCLIC); {
        printf("Topo Sort: \n");
        for(int i = n-1; i >= 0; i--) {
            printf("%s ", gi->vertnames[di->finorder[i]]);  
        }
        printf("\n\n");
    } 
}
