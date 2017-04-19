#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graphio.h"

/* change history:
 * DN 2/2016
 * DN 3/10/2016 to use getline scanning edges (works in linux and is better style). */


/* max data to read from a line of input, allowing for two vertex names and weight */
#define LINE_LEN (2 * (MAX_NAMELEN+1) + 20) 


/* Index of a given vertex name, or -1 if not found. */
/* TODO: could sort the vertices once read, and use bin search. */
int vertexNum(GraphInfo gi, char* name) {
    int i = 0;
    int n = numVerts(gi->graph);
    while ( i < n && strcmp(gi->vertnames[i], name) )
        i++;
    if (i < n) 
        return i;
    else 
        return -1;
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
GraphInfo readGraphX(char* filepath, int repType, int makeSymmetric) { 

    /* open file, initialize input buffer, get number of vertices */
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "graphio:readGraph - fatal error: could not open file %s\n", filepath);
        exit(1);
    }
    size_t lineSize = LINE_LEN;
    char* line = (char *) malloc(lineSize);
    getline(&line, &lineSize, file); /* get one line of input */
    int numVerts;
    sscanf(line, "%i", &numVerts); /* parse the line */

    /* allocate and initialize GraphInfo */
    GraphInfo gi = (GraphInfo) malloc(sizeof(struct graphinfo)); 
    gi->graph = makeGraph(numVerts, repType);
    char **vertnames = (char**) malloc(numVerts * sizeof(char *));
    gi->vertnames = vertnames; 

    /* get vertex names */
    char source[MAX_NAMELEN + 1];
    int i = 0;
    while( i < numVerts && getline(&line, &lineSize, file) > 0 ) {
        sscanf(line, "%s", source);
        vertnames[i] = (char *) malloc(strlen(source) + 1); 
        strcpy(vertnames[i], source);
        i++;
    }
    if (i != numVerts) {
        fprintf(stderr, "graphio:readGraph - file format error getting vertex names in %s\n", filepath);      
        exit(1);
    }

    /* get the edges */
    char target[MAX_NAMELEN + 1]; 
    float weight;

    while ( getline(&line, &lineSize, file) > 0 ) {
        int result = sscanf(line, "%s %s %f\n", source, target, &weight);

        if (result >= 2 ) { // read at least two items
            int src = vertexNum(gi, source);
            int trg = vertexNum(gi, target);
            if (result == 2) // weight not included
                weight = DEFAULT_WEIGHT;
            addEdge(gi->graph, src, trg, weight);
            if (makeSymmetric) 
                addEdge(gi->graph, trg, src, weight);
        } else 
            if (result == 1) 
                fprintf(stderr, "graphio:readGraph - file format error (ignored) getting edges in %s\n", filepath);      
    }

    /* clean up and return */
    fclose(file);

    return gi;
}


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
