#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "graph.c"

/* A simple, klunky test routine.  You need to manually inspect the printed output. */

void test1() {
    printf("\n running test1 \n");
    Graph g = makeGraph(10, MATRIX);
  
    addEdge(g, 0, 1, 0.0); addEdge(g, 0, 2, 0.0);
    addEdge(g, 1, 0, 0.0); addEdge(g, 1, 1, 1.0); addEdge(g, 1, 2, 2.0);
    int* s0 = successors(g, 0);
    int i = 0;
    while (s0[i] != -1) {
	printf("vert 0 has successor %i, ", s0[i]);
	i++;
    }
    printf("\n");
  
    s0 = successors(g, 1);
    i = 0;
    while (s0[i] != -1) {
	printf("vert 1 has successor %i, ", s0[i]);
	i++;
    }
    printf("\n");
  
    disposeGraph(g);
    g = NULL;
}

/* A fancier test that reads from two files.  
   You still need to manually inspect the printed output. */

/* contigUS() 
   Read graph of contiguous US states.
   Assumes CONTIG and CODES files are in current directory.
   Assumes CODES lists the 50 state codes plus "DC", 
   and CONTIG has lines of the form 
   XX YY 
   where XX and YY are state codes. 
   Note that the data file isn't symmetric; may want to add YY XX to graph too. 
*/

#define CONTIG "contiguous-usa.dat"
#define CODES "stateCodes.txt"
#define NUMSTATES 51 
#define CODELEN 3 /* two chars plus null */
#define WEIGHTLESS 0.0

/* index of the state, which is assumed to be present */
/* TODO: since the list is sorted, use binary search. */
int indexOf(char states[NUMSTATES][CODELEN], char state[CODELEN]) {
    int i = 0;
    while ( strcmp(states[i], state) )
	i++;
    return i;	
}

/* print the successors of a state */
void printSucc(Graph g, char states[NUMSTATES][CODELEN], char* state) {
    printf("successors of %s: ", state);
    int* s = successors(g, indexOf(states, state));
    for (int i = 0; s[i] != -1; i++)
	printf(" %i %s ", s[i], states[s[i]]);
    printf("\n");
}

void contigUS() {
    printf("\n running test contigUS \n");
    char states[NUMSTATES][CODELEN]; 
    char source[CODELEN];
    char target[CODELEN]; 
  
    /* read the state codes into states */
    FILE* infile = fopen(CODES, "r");
    if (infile == NULL) {
	fprintf(stderr, "fatal error: could not open file %s\n", CODES);
	exit(1);
    }
    int i = 0;
    while( fscanf(infile, "%s", source) != EOF ) 
	strcpy(states[i++], source);
    fclose(infile);
  
    /* read the edges */
    infile = fopen(CONTIG, "r");
    if (infile == NULL) {
	fprintf(stderr, "fatal error: could not open file %s\n", CONTIG);
	exit(1);
    }
    Graph g = makeGraph(NUMSTATES, MATRIX);
    int src, trg;
    while( fscanf(infile, "%s %s", source, target) != EOF ) {
	src = indexOf(states, source);
	trg = indexOf(states, target);
	addEdge(g, src, trg, WEIGHTLESS);
    } 
    fclose(infile);

    /* print the successors of some states */
    printSucc(g, states,"NJ");
    printSucc(g, states, "CO");

    /* cleanup */
    disposeGraph(g);
}


int main() {
    test1();
    contigUS();
}
