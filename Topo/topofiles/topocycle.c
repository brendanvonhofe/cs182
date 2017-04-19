#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "graph.h"
#include "dfs.h"
#include "topocycle.h"
#include "graphio.h"

/* White == Not-visited
 * Gray == Visited
 * Black == Dead-end
*/
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define DEFAULT_WEIGHT 1.0 
#define MAX_NAMELEN 32 /* max length of a vertex name */

struct dfsinfo;
void DFSvisit(int vert);
DFSinfo DFS(Graph g);

/* list node to represent an edge, for adjacency list */
struct eNode {
    float wt;
    int target; 
    struct eNode* next; 
};
typedef struct eNode* ENode;


/* The graph; see graph.h for type Graph. 
   Depending on the repType field, one of the fields adjM or aLists is not used.
   (We could use a union for this, but it's not worth the trouble.)
   Invariant: each adjacency list begins with a dummy node.
*/
struct graph {
    short repType; /* MATRIX or LIST representation */
    int numVerts;
    float* adjM; /* the adjacency matrix, size numVerts*numVerts */
    ENode* aList; /* array of adjacency lists; length numVerts */
};

Graph G;
int count;
int fincount;
int* color;
int* disc;
int* fin;
int* par;
int* finorder;

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
    printf("Made it to DFS");
    //instantiate global variables
    G = g;
    count = 0;
    fincount = 0;
    color = malloc(g->numVerts * sizeof(int));
    disc = malloc(g->numVerts * sizeof(int));
    fin = malloc(g->numVerts * sizeof(int));
    par = malloc(g->numVerts * sizeof(int));
    finorder = malloc(g->numVerts * sizeof(int));
    for(int i = 0; i < g->numVerts; i++) {
        color[i] = WHITE;
        disc[i] = 0;
        fin[i] = 0;
        par[i] = INFINITY;
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
    //printArr(disc);
    //printArr(fin);
    //printArr(par);
    return dfs;
}


void DFSvisit(int vert) {
    printf("Visiting: %d", vert);
    color[vert] = GRAY;
    count++;
    disc[vert] = count;
    int* sux = successors(G, vert);
    int succLen = sizeof(sux) / sizeof(int);
    for(int i = 0; i < succLen - 1; i++) {
        int u = sux[i];
        if(color[u] == WHITE) {
            par[u] = vert;
            DFSvisit(u);
        }
    }
    color[vert] = BLACK;
    count++;
    fin[vert] = count;
    finorder[fincount] = vert;
}

/* A graph together with names for its vertices. 
*/
/*
struct graphinfo {
    Graph graph;
    char **vertnames; //array of numVerts vertex names
};
typedef struct graphinfo* GraphInfo;
*/

/* Index of a given vertex name, or -1 if not found. */
/* TODO: could sort the vertices once read, and use bin search. */
int indexOf(GraphInfo gi, char* name) {
    int i = 0;
    int n = numVerts(gi->graph);
    while ( i < n && strcmp(gi->vertnames[i], name) )
	i++;
    if (i < n) 
	return i;
    else return -1;
}


/* attempt to open a file; halt execution if fail */
FILE* tryOpen(char* path, char* mode) {
    FILE* file = fopen(path, mode);
    if (file == NULL) {
	fprintf(stderr, "tryOpen fatal error: could not open file %s\n", path);
	exit(1);
    }
    return file;
}

/* Read a graph from a text file, using the given representation. 
Assumes path is a null-terminated string that is valid file path.
Assumes file has the specified format.
*/
GraphInfo readGraph(char* path, int repType) { 

    /* open file, get number of vertices, allocate and initialize GraphInfo */
    FILE* infile = tryOpen(path, "r");
    int numVerts;
    fscanf(infile, "%i", &numVerts); 

    GraphInfo gi = (GraphInfo) malloc(sizeof(struct graphinfo)); 
    gi->graph = makeGraph(numVerts, repType);
    char **vertnames = (char**) malloc(numVerts * sizeof(char *));
    gi->vertnames = vertnames; 

    /* get vertex names */
    char source[MAX_NAMELEN + 1];
    int i = 0;
    while( i < numVerts && fscanf(infile, "%s", source) != EOF ) {
	// read, determine length, and make copy in the heap
	vertnames[i] = (char *) malloc(strlen(source)); 
	strcpy(vertnames[i], source);
	i++;
    }

    /* get the edges */
    char target[MAX_NAMELEN + 1]; 
    float weight;
    int result = fscanf(infile, "%s %s %f", source, target, &weight);
    while( result != EOF) {
	if (result >= 2 ) { // read at least two items
	    if (result == 2) // weight not included
		weight = DEFAULT_WEIGHT;
	    addEdge(gi->graph, indexOf(gi, source), indexOf(gi, target), weight);
	    result = fscanf(infile, "%s %s %f", source, target, &weight);
	} else { 
	 //   fprintf(stderr, "readGraph fatal error: unexpected file format in %s\n", path);
	    exit(1);
	}
    }

    /* clean up and return */
    fclose(infile);
    return gi;
}

/* Prints the information produced by DFS, using names of vertices.
Assumes di->graph == gi->graph.  */
void printDFSinfo(DFSinfo di, GraphInfo gi) {
    assert (di->graph == gi->graph);
    int numV = numVerts(di->graph);
    printf("Printing DFS info \n");
    for (int v = 0; v < numV; v++) {
        printf("Vertex %s discovered %3i finished %3i ",
               gi->vertnames[v], di->discover[v], di->finish[v]);
        if (di->parent[v] != -1)
            printf("parent %s \n", gi->vertnames[di->parent[v]]);
        else
            printf("parent %i \n", di->parent[v]);
    }
    printf("Vertices in order of finish:\n");
    for (int i = 0; i < numV; i++)
        printf("%s ", gi->vertnames[di->finorder[i]]);
    printf("\ndone printing DFS info\n");
}

/* Make an empty graph with n vertices, using either adjacency matrix 
   or adjacency lists depending on whether rep==MATRIX or rep==LIST.
   Precondition: n>=1 and rep is MATRIX or LIST.
*/
Graph makeGraph(int n, int rep) {
    Graph g = (Graph) malloc(sizeof(struct graph));
    g->numVerts = n;
    g->repType = rep;
    if (rep == MATRIX) {
	g->aList = NULL;
	g->adjM = (float*) malloc(n * n * sizeof(float));
	for (int src = 0; src < n; src++)
	    for (int trg = 0; trg < n; trg++)
		*(g->adjM + (n*src) + trg) = INFINITY; // (g->adjM)[src][trg] = INFINITY;
    } else { // repType is LIST
	g->adjM = NULL;
	g->aList = (ENode*) malloc(n * sizeof(ENode));
	// initialize with dummy nodes
	for (int src = 0; src < n; src++) {
	    ENode node = (ENode) malloc(sizeof(struct eNode));
	    node->wt = 0.0; 
	    node->target = 0; 
	    node->next = NULL;
	    *(g->aList + src) = node;
	}
    }
    return g;
}


/* make a copy of g, but using the representation
   specified by rep (which is assumed to be MATRIX or LIST) 
   Known bug: not implemented
*/
Graph cloneGraph(Graph g, int rep){ 
    printf("cloneGraph not supported.\n");
    exit(1); 
}


/* free the graph object and all its resources.
   Postcondition: g is no longer a valid pointer. 
   Alert: the caller should set their variable to null.
   Known bug: does not free nodes of adjacency lists.
*/
void disposeGraph(Graph g) {
    if (g->repType == MATRIX)
	free(g->adjM);
    else  // repType is LIST
	free(g->aList);
    free(g);
}


/* number of vertices */
int numVerts(Graph g){ 
    return g->numVerts;
}


/* add edge from source to target with weight w, and return
   OK, if source and target are valid vertex numbers and
   there was no edge from source to target initially.
   Otherwise, make no change and return ERROR. 
*/
int addEdge(Graph g, int source, int target, float w) {
    int n = g->numVerts;
    if (source < 0 || source >= n || target < 0 || target >= n)
	return ERROR;
    if (g->repType == MATRIX) {
	if ( *(g->adjM + (n*source) + target) != INFINITY )
	    return ERROR; // an edge was already present
	*(g->adjM + (n*source) + target) = w;
    } else { // rep is LIST; attach at end of list
	ENode p = *(g->aList + source);
	while (p->next != NULL && p->next->target != target) 
	    p = p->next;
	if (p->next != NULL )
	    return ERROR; // an edge was already present
	else {
	    ENode node = (ENode) malloc(sizeof(struct eNode));
	    p->next = node;
	    node->wt = w;
	    node->target = target;
	    node->next = NULL;
	}
    }
    return OK;
}    

/* delete edge from source to target, and return
   OK, if there was an edge from source.
   Otherwise, make no change and return ERROR. 
*/
int delEdge(Graph g, int source, int target) { 
    int n = g->numVerts;
    if (source < 0 || source >= n || target < 0 || target >= n)
	return ERROR;
    if (g->repType == MATRIX) {
	if ( *(g->adjM + (n*source) + target) == INFINITY )
	    return ERROR;
	*(g->adjM + (n*source) + target) = INFINITY;
    } else { // LIST
	ENode follow = *(g->aList + source);
	ENode p = follow->next; // skip dummy node
	while (p != NULL && p->target != target) {
	    follow = p;
	    p = p->next;
	}
	if (p == NULL)
	    return ERROR; 
	else {
	    follow->next = p->next;
	    free(p);
	}
    }
    return OK;
}


/* return weight of the edge from source to target,
   if there is one; otherwise return INFINITY.
   Return -1.0 if source or target are out of range.
*/
float edge(Graph g, int source, int target) {
    int n = g->numVerts;
    if (source < 0 || source >= n || target < 0 || target >= n)
	return -1.0; 
    if (g->repType == MATRIX) {
	return *(g->adjM + (n * source) + target);
    } else { // LIST
	ENode p = *(g->aList + source);
	p = p->next; // skip dummy node
	while (p != NULL && p->target != target)   
	    p = p->next;
	if (p == NULL)
	    return INFINITY; 
	else 
	    return p->wt;
    }
}


/* a freshly allocated array with the successor
   vertices of source, if any, followed by an entry with -1
   to indicate end of sequence.
   Precondition: source is in range.
*/
int* successors(Graph g, int source) {
    int n = g->numVerts;
    int succ[n]; // temporary array, on the stack, big enough for all possible successors 
    int nsucc = 0; // actual number of successors

    /* get precedessors into temporary array in the current stack frame */
    if (g->repType == MATRIX) {
	for (int trg = 0; trg < g->numVerts; trg++) 
	    if ( *(g->adjM + (n*source) +trg) != INFINITY ) 
		succ[nsucc++] = trg;
    } else { // LIST
	ENode p = *(g->aList + source);
	p = p->next; // skip dummy node
	while (p != NULL ) {
	    succ[nsucc++] = p->target;
	    p = p->next;
	}
    }
    /* allocate and return an array the right size */
    int* result = (int*) malloc((nsucc + 1) * sizeof(int));
    for (int i = 0; i < nsucc; i++)
	result[i] = succ[i];
    result[nsucc] = -1;        
    return result;
}


/* a freshly allocated array with the predecessor
   vertices of target, if any, followed by an entry with -1
   to indicate end of sequence.
   Precondition: target is in range.
   Known bug: not implemented for LIST.
*/
int* predecessors(Graph g, int target) {
    if (g->repType == LIST) {
	fprintf(stderr, "graph predecessors not implemented for LIST representation.\n");
	exit(1);
    } // else MATRIX
    /* get precedessors into temporary array in the current stack frame */
    int n = g->numVerts;
    int pred[n]; // temporary array, big enough for all possible predecessors
    int npred = 0; // number of predecessors
    for (int src = 0; src < g->numVerts; src++) 
        if ( *(g->adjM + (n*src) +target) != INFINITY ) 
            pred[npred++] = src;
    /* allocate and return an array the right size */
    int* result = (int*) malloc((npred + 1) * sizeof(int));
    for (int i = 0; i < npred; i++)
	result[i] = pred[i];
    result[npred] = -1;        
    return result;
}

/* Index of a given vertex name, or -1 if not found. */
/* TODO: could sort the vertices once read, and use bin search. */
int vertexNum(GraphInfo gi, char* name) {
    int i = 0;
    int n = numVerts(gi->graph);
    while ( i < n && strcmp(gi->vertnames[i], name) )
	i++;
    if (i < n) 
	return i;
    else return -1;
}


/* attempt to open a file; halt execution if fail */
FILE* tryOpen(char* path, char* mode) {
    FILE* file = fopen(path, mode);
    if (file == NULL) {
	fprintf(stderr, "graphio:readGraph:tryOpen fatal error: could not open file %s\n", path);
	exit(1);
    }
    return file;
}



/* Read a graph from a text file, using the given representation. 
Assumes filepath is a null-terminated string that is valid file path.
Assumes the file exists and has the specified format.
If makeSymmetric is MAKE_SYMM, add both (S,T) and (T,S) for each edge S T in the file 
(except when (T,S) present from an earlier line of the file).
*/
/*
GraphInfo readGraphX(char* filepath, int repType, int makeSymmetric) { 

    /* open file, get number of vertices */
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
	fprintf(stderr, "graphio:readGraph - fatal error: could not open file %s\n", filepath);
	exit(1);
    }
    int numVerts;
    fscanf(file, "%i", &numVerts); 

    /* allocate and initialize GraphInfo */
    GraphInfo gi = (GraphInfo) malloc(sizeof(struct graphinfo)); 
    gi->graph = makeGraph(numVerts, repType);
    char **vertnames = (char**) malloc(numVerts * sizeof(char *));
    gi->vertnames = vertnames; 

    /* get vertex names */
    char source[MAX_NAMELEN + 1];
    int i = 0;
    while( i < numVerts && fscanf(file, "%s", source) != EOF ) {
	// read, determine length, and make copy in the heap
      vertnames[i] = (char *) malloc(strlen(source) + 1); 
      strcpy(vertnames[i], source);
      i++;
    }

    /* get the edges */
    char target[MAX_NAMELEN + 1]; 
    float weight;
    int result = fscanf(file, "%s %s %f", source, target, &weight);
    while( result != EOF && result != 0 ) {
	if (result >= 2 ) { // read at least two items
	    int src = vertexNum(gi, source);
	    int trg = vertexNum(gi, target);
	    if (result == 2) // weight not included
		weight = DEFAULT_WEIGHT;
	    addEdge(gi->graph, src, trg, weight);
	    if (makeSymmetric) 
		addEdge(gi->graph, trg, src, weight);
	} 
    result = fscanf(file, "%s %s %f", source, target, &weight);
    }
    /* clean up and return */
    fclose(file);

      return gi;
}*/


/* see graphio.h */
GraphInfo readGraph(char* filepath, int repType) {
    return readGraphX(filepath, repType, 0);
}


/* see graphio.h */
GraphInfo readGraphMakeSymm(char* filepath, int repType) {
    return readGraphX(filepath, repType, 1);
}



/* Prints the graph to stdout, in the file format.
Assumes gi points to a valid object. 
It prints each vertex's successors, with a space between groups,
for readability.
*/
void writeGraph(GraphInfo gi) {
    int numV = numVerts(gi->graph);
    printf("%i \n", numV);
    for (int v = 0; v < numV; v++)
	printf("%s \n", gi->vertnames[v]);
    for (int v = 0; v < numV; v++) {
      int* succs = successors(gi->graph, v);
      int i = 0;
      if (succs[0] != -1) 
	    printf("\n");
      while (succs[i] != -1) {
	    printf("%s %s \n", gi->vertnames[v], gi->vertnames[succs[i]]);
	    i++;
      }
    }
    printf("\n");	
}

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
    for(int i = 0; i < gi->graph->numVerts; i++) {
        for(int j = 0; j < gi->graph->numVerts; j++) {
            //printf("ANTS:%d\n", proper_ancestor(di, i, j));
            if(proper_ancestor(di, i, j) == 1) {
                anc = i;
                v = j;
                break;
            }
        }
    }
    if(anc != -1) {
        int vert = di->parent[v];
        printf("Cycle: ");
        while(vert != v) {
            printf("%d ", vert);
            vert = di->parent[vert]; 
        }
        printf("%d\n ", anc);
    }
    else printf("NO incestors");
}

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
