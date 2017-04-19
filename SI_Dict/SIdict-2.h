/* Simple interface for dictionaries that map strings to integers. */

struct si_dict;
typedef struct si_dict* SIdict;

/* make an empty one */
SIdict makeSIdict();

/* whether key is present 
   Precondition: d and key are non-null. */
int hasKey(SIdict d, char* key);

/* map key to val; return 1 if key was already present else 0 
   Precondition: d and key are non-null. */
int addOrUpdate(SIdict d, char* key, int val);

/* get value associated with key
   Precondition: d is non-null and key is present (so it's non-null). */
int lookup(SIdict d, char* key);

/* Remove key/val; return 1 if key was already present else 0
   Precondition: d and key are non-null. */
int remKey(SIdict d, char* key);



