/* Takes a graph, performs a depth-first-search, and returns information
 * about each vertex gained from the search.
 *
 * I pledge my honor that I have abided by the Stevens Honor System.
 * BRENDAN VON HOFE
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dfs.h"
#include "graph.c"

/* White == Not-visited
 * Gray == Visited
 * Black == Dead-end
*/
#define WHITE 0
#define GRAY 1
#define BLACK 2

struct dfsinfo;
void DFSvisit(int vert);
DFSinfo DFS(Graph g);

Graph G;
int count;
int fincount;
int* color;
int* disc;
int* fin;
int* par;
int* finordr;

void printArr(int* arr) {
    for(int i = 0; i < G->numVerts; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* takes a graph and return a DFSinfo object containing arrays that
 * contain the time that each vertex is discovered, finished, and
 * an array that contains a parent to each vertex
*/
DFSinfo DFS(Graph g) {
    //instantiate global variables
    G = g;
    count = 0;
    fincount = 0;
    color = malloc(g->numVerts * sizeof(int));
    disc = malloc(g->numVerts * sizeof(int));
    fin = malloc(g->numVerts * sizeof(int));
    par = malloc(g->numVerts * sizeof(int));
    finordr = malloc(g->numVerts * sizeof(int));
    for(int i = 0; i < g->numVerts; i++) {
        color[i] = WHITE;
        disc[i] = 0;
        fin[i] = 0;
        par[i] = INFINITY;
        finordr[i] = 0;
    } 

    //call dfs visit on all non-visited vertices
    for(int i = 0; i < g->numVerts; i++) {
        if(color[i] == WHITE) {
            DFSvisit(i);
        }
    }

    //create DFSinfo object and copy over info arrays
    DFSinfo dfs = (DFSinfo) malloc(sizeof(struct dfsinfo));
    dfs->discover = disc;
    dfs->finish = fin;
    dfs->parent = par;
    dfs->graph = g;
    dfs->finorder = finordr;
    //printArr(disc);
    //printArr(fin);
    //printArr(par);
    return dfs;
}


void DFSvisit(int vert) {
    //printf("Visiting: %d\n", vert);
    color[vert] = GRAY;
    count++;
    disc[vert] = count;
    int* sux = successors(G, vert);
    int succLen = sizeof(sux) / sizeof(int);
    int sCount = 0;
    while(sux[sCount] != -1) {
        int u = sux[sCount];
        if(color[u] == WHITE) {
            par[u] = vert;
            DFSvisit(u);
        }
        sCount++;
    }
    color[vert] = BLACK;
    count++;
    fin[vert] = count;
    finordr[fincount] = vert;
    fincount++;
}


