/* Brendan von Hofe
 * I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphAlgs.h"
#include "minprio.h"

Graph transClose(Graph g) {
    int n = g->numVerts;
    Graph R[n + 1];
    R[0] = g;
    for(int k = 1; k <= n; k++) {
        R[k] = makeGraph(n, MATRIX);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(edge(R[k-1], i, j) != INFINITY || (edge(R[k-1], i, k-1) != INFINITY && edge(R[k-1], k-1, j) != INFINITY)) {
                    addEdge(R[k], i, j, 0);
                }
            }
        } 
    }
    return R[n];
}

int floatComp(float x, float y) {
    return x - y;
}

struct vert_distance {
    int distance;
    int vert;
};
typedef struct vert_distance* Vertdis;

Graph minSpanTree(Graph g) {
    int n = g->numVerts;
    Graph T = makeGraph(n, MATRIX);
    MinPrio Q = makeQueue(floatComp, n);
    Handle queueTrack[n];
    for(int i = 0; i < n; i++) {
        queueTrack[i] = NULL;
    }
    int link[n];
    for(int i = 0; i < n; i++) {
        link[i] = NULL;
    }
    Vertdis src = malloc(sizeof(struct vert_distance));
    src->distance = 0;
    src->vert = 0; 
    queueTrack[src->vert] = enqueue(Q, src);
    for(int i = 0; i < n; i++) {
        if(src->vert != i) {
            Vertdis vert = malloc(sizeof(struct vert_distance));
            vert->distance = INFINITY;
            vert->vert = i;
            queueTrack[vert->vert] = enqueue(Q, vert);
        }
    }
    while(nonempty(Q) == 1) {
        Vertdis v = dequeueMin(Q);
        queueTrack[v->vert] = NULL;
        int* succs = successors(g, v->vert);
        int i = 0;
        while(succs[i] != -1) {
            int u = succs[i];
            float dist = edge(g, u, v->vert);
            Vertdis uVert = (Vertdis) queueTrack[u]->content;
            if(queueTrack[u] != NULL && dist < uVert->distance) {
                link[u] = v->vert;
                uVert->distance = dist;
                decreasedKey(Q, queueTrack[u]);
            } 
        }
    }
    for(int i = 0; i < n; i++) {
        addEdge(T, i, link[i], edge(g, i, link[i]));
        addEdge(T, link[i], i, edge(g, link[i], i));
    }
    return T;
}

