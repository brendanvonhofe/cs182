/* Simple interface for dictionaries that map strings to integers. */

struct si_dict;
typedef struct si_dict* SIdict;

/* make an empty one */
SIdict makeSIdict();

/* whether key is present 
   This and the other functions assume d non-null. */
int hasKey(SIdict d, char* key);

/* map key to val; return 1 if key was already present else 0 */
int addOrUpdate(SIdict d, char* key, int val);

/* get value associated with key
   Precondition: key is present */
int lookup(SIdict d, char* key);

/* Remove key/val; return 1 if key was already present else 0 */
int remKey(SIdict d, char* key);



