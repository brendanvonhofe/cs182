/* Interface for dictionaries that map strings to "void pointers".
 *
 * A key is a null-terminated string, and the dictionary stores a copy.
 * The associated value has type void*, so it can be any pointer type and is no
 * dereferenced by the dictionary implementation code.
 * 
 * The implementation should use AVL trees as discussed in class and Levitin 6.3.
 */

#ifndef SVDDICT_H
#define SVDDICT_H

#define MAXKEYLEN 33   /* key strings should be no longer than 32 plus the null */
#define EMPTY_PRINT .  /* character to indicate empty tree when printing */

struct sv_dict;
typedef struct sv_dict* SVdict;


/* make an empty dictionary */
SVdict makeSVdict();


/* Free the dictionary object and all its resources.
 * Precondition: d is non-null and has been initialized by makeSVdict. 
 * Postcondition: d is no longer a valid pointer. 
 * Alert: the caller should set their variable to null.
 * The dictionary owns its copies of the keys, but does not own
 * the values so those are not freed. 
 */
void disposeSVdict(SVdict d);


/* whether key is present 
   Precondition: d and key are non-null. */
int hasKey(SVdict d, char* key);


/* map key to val; return 1 if key was already present else 0 
 * Precondition: d and key are non-null. 
 * Stores a copy of key, not the pointer itself.
*/
int addOrUpdate(SVdict d, char* key, void* val);


/* get value associated with key, or NULL if key not in d.
 * Precondition: d and key are non-null.
 */
void* lookup(SVdict d, char* key);


/* Remove key/val; return 1 if key was already present else 0
   Precondition: d and key are non-null. */
int remKey(SVdict d, char* key);


/* Print the keys, in pre-order, to standard output. 
 * Print each key, preceded by some spaces, and followed by \n.  
 * If a subtree is empty, print EMPTY_PRINT.  
 * Nothing else should be printed. 
 * The number of preceding spaces should indicate the depth of the
 * tree, one space per level, with no spaces for the root. 
 * Precondition: d is non-null. 
 */
void preorderKeys(SVdict d);



#endif 
