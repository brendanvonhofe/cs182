#include "graph.h"

/****************************************************************
Graphs with names for vertices.
Provides function to read a graph from a file, and a 
function to write a graph to standard out.

File format is assumed to be:
- first line is a nonnegative integer N in decimal
- following N lines each have one vertex name (a sequence of 
  non-blank chars, at most MAX_NAMELEN of them).
- remainder of the file can have blank lines; non-blank lines 
have either two or three non-blank elements, in the form:
S T W
where S and T are vertex names, for an edge S->T, and W is a decimal weight.
If omitted, the weight is interpreted as DEFAULT_WEIGHT.
******************************************************************/

#define DEFAULT_WEIGHT 1.0 

#define MAX_NAMELEN 32 /* max length of a vertex name */

#ifndef _graphinfo
#define _graphinfo

/* A graph together with names for its vertices. */
struct graphinfo {
    Graph graph;
    char **vertnames; //array of numVerts vertex names
};
typedef struct graphinfo* GraphInfo;

#endif


/* Index of a given vertex name, or -1 if not found. */
int vertexNum(GraphInfo gi, char* name);


/* NOTE: To get the name for a given vertex number v, just use g->vertnames[v] */


/* Read a graph from a text file, assuming format described above. 
   Assumes filepath is a null-terminated string that is valid file path.
   Assumes the file has the format specified above.
*/
GraphInfo readGraph(char* filepath, int repType);


/* Same as readGraph, but make a symmetric graph by also adding T->S
   in addition to S->T, for a line with S T, unless there was already
   an edge T->S from a preceding line of the file.
*/
GraphInfo readGraphMakeSymm(char* filepath, int repType);



/* Prints the graph to stdout, in the file format.
   Assumes gi points to a valid object. 
   It prints each vertex's successors, with a space between groups,
   for readability.
*/
void writeGraph(GraphInfo gi);

