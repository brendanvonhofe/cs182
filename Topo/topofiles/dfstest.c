/* some tests for dfs.c 
   Includes code to read a graph from a file.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "graph.h"
#include "dfs.c"
#include "topocycleSKEL.c"
#include "graphio.c"
#include "dfstestSKEL.c"

#define DEFAULT_WEIGHT 1.0 

#define MAX_NAMELEN 32 /* max length of a vertex name */

/****************************************************************
Read graph from file, with strings for vertex names.

File format is assumed to be:
- first line is a nonnegative integer N in decimal
- following N lines each have one vertex name (a sequence of 
  non-blank chars, at most MAX_NAMELEN of them).
- remainder of the file can have blank lines; non-blank lines 
have either two or three non-blank elements, in the form:
S T W
where S and T are vertex names and W is a decimal weight.
If omitted, the weight is interpreted as DEFAULT_WEIGHT.
******************************************************************/

/*
int main() {
    GraphInfo gi; 
    DFSinfo di;
    gi = readGraph("prereqCatch22.txt", LIST);
    printf("gi->graph:\n");
    writeGraph(gi);
    printDFSinfo(di, gi);
    di = DFS(gi->graph);
    return 0;
}*/

/* A graph together with names for its vertices. 
*/
/*
struct graphinfo {
    Graph graph;
    char **vertnames; //array of numVerts vertex names
};
typedef struct graphinfo* GraphInfo;*/


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


/**********************
 * Test DFS
 **********************/


