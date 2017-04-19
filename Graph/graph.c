/* I pledge my honor that I have abided by the Stevens Honor System.
 * Brendan von Hofe
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "graph.h"

typedef struct vertex* Vertex;
struct vertex {
    float weight;
    int adjacent;
    Vertex next;
};

typedef struct linkedList *LinkedList;
struct linkedList {
    Vertex front;
    int vertex;
};

struct graph {
    int n;
    int type;
    float **matrix;
    LinkedList *list;
};

Graph makeGraph(int n, int rep) {
    if(rep == LIST) {
        Graph g = (Graph) malloc(sizeof(struct graph));
        g->type = LIST;
        g->n = n;
        g->matrix = NULL;
        g->list = malloc(n * sizeof(struct linkedList)); 
        for(int i = 0; i < n; i++) {
            g->list[i] = (LinkedList) malloc(sizeof(struct linkedList));
            g->list[i]->vertex = i; 
            g->list[i]->front = NULL;
        }
        return g;
    } 
    else {
        Graph g = (Graph) malloc(sizeof(struct graph));
        g->type = MATRIX;
        g->n = n;
        g->list = NULL;
        g->matrix = malloc(n * sizeof(float*));
        for(int i = 0; i < n; i++) {
            g->matrix[i] = malloc(n * sizeof(float));
            for(int j = 0; j < n; j++) {
                g->matrix[i][j] = INFINITY;
            }
        }
        return g;
    }
}

Graph cloneGraph(Graph G, int rep) {
    Graph g = makeGraph(G->n, rep);
    if(rep == LIST && G->type == MATRIX) {
        for(int i = 0; i < G->n; i++) {
            for(int j = 0; j < G->n; j++) {
                if(G->matrix[i][j] != INFINITY) {
                    addEdge(g, i, j, G->matrix[i][j]);    
                }
            }
        }
        return g;
    } 
    else if(rep == MATRIX && G->type == LIST) {
        for(int i = 0; i < G->n; i++) {
            Vertex cur = G->list[i]->front;
            while(cur != NULL) {
                g->matrix[i][cur->adjacent] = cur->weight;
                cur = cur->next;
            }
        }
        return g;
    }
    else if(rep == LIST && G->type == LIST) {
        for(int i = 0; i < G->n; i++) {
            Vertex cur = G->list[i]->front;
            while(cur != NULL) {
                addEdge(g, i, cur->adjacent, cur->weight); 
            }
        }
        return g;
    }
    else {
        for(int i = 0; i < G->n; i++) {
            for(int j = 0; j < G->n; j++) {
                if(G->matrix[i][j] != INFINITY) {
                    g->matrix[i][j] = G->matrix[i][j];
                }
            }
        }
        return g;

    }
}

void disposeGraph(Graph g) {
    if(g->type == LIST) {
        for(int i = 0; i < g->n; i++) {
            Vertex cur = g->list[i]->front;
            while(cur != NULL) {
                Vertex next = cur->next;
                free(cur);
                cur = next;
            }
            free(g->list[i]);
        }
        free(g->list);
        free(g);
    }
    else {
        for(int i = 0; i < g->n; i++) {
            free(g->matrix[i]);
        }
        free(g->matrix);
        free(g);
    }
}

int numVerts(Graph G) {
    return G->n;
}

int addEdge(Graph G, int source, int target, float w) {
    if(source >= G->n || target >= G->n || source < 0 || target < 0) return ERROR;
    if(G->type == LIST) {
        if(G->list[source]->front == NULL) {
            G->list[source]->front = (Vertex) malloc(sizeof(struct vertex));
            G->list[source]->front->weight = w;
            G->list[source]->front->adjacent = target;
            G->list[source]->front->next = NULL;
            return OK;
        }
        else {
            Vertex cur = G->list[source]->front;
            Vertex prev = NULL;
            while(cur != NULL && cur->adjacent != target) {
                prev = cur;
                cur = cur->next;
            }
            if(cur == NULL) {
                prev->next = (Vertex) malloc(sizeof(struct vertex));
                prev->next->weight = w;
                prev->next->adjacent = target;
                return OK;
            }
            else {
                return ERROR;
            }
        }
    }
    else {
        if(G->matrix[source][target] == INFINITY) {
            G->matrix[source][target] = w;
            return OK;
        }
        else return ERROR;
    }
}

int delEdge(Graph G, int source, int target) {
    if(source >= G->n || target >= G->n || source < 0 || target < 0) return ERROR;
    if(G->type == LIST) {
        Vertex cur = G->list[source]->front;
        Vertex prev = NULL;
        while(1) {
            if(cur == NULL) {
                return ERROR;
            }
            if(cur->adjacent == target) {
                if(prev != NULL) prev->next = cur->next;
                cur->weight = 0;
                cur->adjacent = 0;
                cur->next = NULL;
                free(cur);
                return OK;
            }
            prev = cur;
            cur = cur->next;
        }
    }
    else {
        if(G->matrix[source][target] == INFINITY) return ERROR;
        else { 
            G->matrix[source][target] = INFINITY;
            return OK;        
        }
    }
}

float edge(Graph G, int source, int target) {
    if(source >= G->n || target >= G->n || source < 0 || target < 0) return -1;
    if(G->type == LIST) {
        Vertex cur = G->list[source]->front;
        while(cur != NULL && cur->adjacent != target) {
            cur = cur->next;
        }  
        if(cur == NULL) {
            return INFINITY;
        }
        else {
            return cur->weight;
        }
    }
    else {
        return G->matrix[source][target];
    }
}

int* successors(Graph G, int source) {
    if(G->type == LIST) {
        Vertex cur = G->list[source]->front;
        int len = 0;
        while(cur != NULL) {
            cur = cur->next;
            len++;
        }
        int *successors = malloc((len+1) * sizeof(int));
        cur = G->list[source]->front;
        for(int i = 0; i < len; i++) {
            successors[i] = cur->adjacent;
            cur = cur->next; 
        }
        successors[len] = -1;
        return successors;
    }
    else {
        int count = 0;
        for(int i = 0; i < G->n; i++) {
            if(G->matrix[source][i] != INFINITY) count++;
        }
        int *scsrs = malloc((count+1) * sizeof(int));
        count = 0;
        for(int i = 0; i < G->n; i++) {
            if(G->matrix[source][i] != INFINITY) {
                scsrs[count] = i; 
                count++;
            }
        }
        scsrs[count] = -1;
        return scsrs;
    }
}

int* predecessors(Graph G, int target) {
    if(G->type == LIST) {
        int len = 0;
        for(int i = 0; i < G->n; i++) {
            Vertex cur = G->list[i]->front;
            while(cur != NULL) {
                if(cur->adjacent == target) len++;
                cur = cur->next;
            }
        }
        int *predecessors = malloc((len+1) * sizeof(int));
        len = 0;
        for(int i = 0; i < G->n; i++) {
            Vertex cur = G->list[i]->front;
            while(cur != NULL) {
                if(cur->adjacent == target) {
                    predecessors[len] = i;
                    len++;
                }
                cur = cur->next;
            }
        }
        predecessors[len] = -1;
        return predecessors;
    }
    else {
        int count = 0;
        for(int i = 0; i < G->n; i++) {
            if(G->matrix[i][target] != INFINITY) count++;
        }
        int *prdcsrs = malloc((count+1) * sizeof(int));
        count = 0;
        for(int i = 0; i < G->n; i++) {
            if(G->matrix[i][target] != INFINITY) {
                prdcsrs[count] = i; 
                count++;
            }
        }
        prdcsrs[count] = -1;
        return prdcsrs;
    }
}

void printGraph(Graph g) {
    if(g->type == MATRIX) { 
        printf("    ");
        for(int i = 0; i < g->n; i++) {
            printf("    (%d)", i);
        }
        printf("\n\n");
        for(int i = 0; i < g->n; i++) {
            printf("(%d)  ", i);
            for(int j = 0; j < g->n; j++) {
                printf("%6.0f ", g->matrix[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("    ");
        for(int i = 0; i < g->n; i++) {
            printf("    (%d)", i);
        }
        printf("\n\n");

        for(int i = 0; i < g->n; i++) {
            Vertex cur = g->list[i]->front;
            printf("(%d)  ", i);
            while(cur != NULL) {
                printf("%6.0f ", cur->weight);
                cur = cur->next;
            }
            printf("\n");
        }
    }
}

/*
* no verts
* no edges
* add edge, check succ's
* clone (all 4)
* change orig, check clone
* dump graph-print
* add, delete sequences
* complete graph
*/

int main() {
    Graph g = makeGraph(10, LIST);
    addEdge(g, 0, 1, 420);
    addEdge(g, 0, 2, 420);
    addEdge(g, 0, 3, 420);

    addEdge(g, 2, 4, 69);
    addEdge(g, 2, 5, 69);
    addEdge(g, 2, 6, 69);
    printGraph(g);
    printf("\n");
    Graph G = cloneGraph(g, MATRIX);
    printGraph(G);
}
