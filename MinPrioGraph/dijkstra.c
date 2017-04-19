/* Brendan von Hofe
 * I pledge my honor that I have abided by the Stevens Honor System
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dijkstra.h"
#include "minprio.h"

// parent value of -1 means no parent
struct vertex {
    int v;
    int par;
    int used;
    float d;
};
typedef struct vertex* Vertex;

Vertex makeVertex(float d, int p, int v) {
    Vertex new = malloc(sizeof(struct vertex));
    new->d = d;
    new->par = p;
    new->v = v;
    new->used = 0;
    return new;
}

void printVertexInfo(Vertex *arr, int len) {
    for(int i = 0; i < len; i++) {
        printf("W:%f U:%d\n", arr[i]->d, arr[i]->used);
    }
    printf("\n");
}

int VertCompare(Vertex lhs, Vertex rhs) {
    float left, right;
    left = lhs->d; right = rhs->d;
    if(left == right) return 0;
    else if(left > right) return 1;
    else return -1;
}

void shortestPaths(GraphInfo GI, int s) {
    int n = GI->graph->numVerts;
    MinPrio q = makeQueue((Comparator)VertCompare, n);
    Vertex V[n];
    Handle hands[n];
    Vertex vert;
    int* succs;

    for(int i = 0; i < n; i++) {
        hands[i] = enqueue(q, makeVertex(INFINITY, -1, i));
        V[i] = hands[i]->content;
    }
    V[s]->d = 0;
    decreasedKey(q, hands[s]);
    for(int i = 0; i < n; i++) {
        vert = dequeueMin(q);
        vert->used = 1;
        //printf("At i=%d vert=%d\n", i, vert->v);
        //printVertexInfo(V, n);
        succs = successors(GI->graph, vert->v);
        float pathWeight;
        for(int j = 0; succs[j] != -1; j++) {
            pathWeight = vert->d + edge(GI->graph, vert->v, succs[j]);
            if(V[succs[j]]->used == 0 && pathWeight < V[succs[j]]->d) {
                V[succs[j]]->d = pathWeight;
                V[succs[j]]->par = vert->v;
                if(V[succs[j]]->used == 0) decreasedKey(q, hands[succs[j]]);
            }
        }
    }
    printf("Shortest paths from vertex %s\n", GI->vertnames[s]);
    for(int i = 0; i < n; i++) {
        if(i != s && V[i]->d != INFINITY) {
            printf("  to %s is %f via %s\n", GI->vertnames[i], V[i]->d, GI->vertnames[V[i]->par]);
        }
    }
}

int main() {
    GraphInfo gi = readGraphMakeSymm("states49Contig.txt", MATRIX);
    shortestPaths(gi, 28);
}
