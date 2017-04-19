#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "SIdict.h"

#define MAXKEYCHARS 32  
#define PRESENT 0
#define DELETED 1

/* SIdict represented by binary search tree. 

   Each function assumes the preconditions that 
   the dictionary and key parameters are non-null.
*/

typedef struct tnode* Tnode;
struct tnode { char key[MAXKEYCHARS]; 
               int val; 
               Tnode left; 
               Tnode right; 
               int deleted; };

struct si_dict { Tnode root; };

SIdict makeSIdict() {
    SIdict d = (SIdict) malloc(sizeof(struct si_dict));
    d->root = NULL;
    return d;
}

/*returns 1 if key is present, 0 if not*/

int hasKey(SIdict d, char* key) {
    Tnode cur = d->root;
    while(cur != NULL && strcmp(cur->key, key) != 0) {
        if(strcmp(cur->key, key) > 0) cur = cur->left;
        else cur = cur->right;
    }
    if(cur == NULL || cur->deleted == DELETED) return 0;
    else return 1;
}


/*returns 0 if a new node is created, 1 if a node was updated*/

int addOrUpdate(SIdict d, char* key, int val) {
    Tnode cur = d->root;
    Tnode prev;
    while(cur != NULL && strcmp(cur->key, key) != 0) {
        if(strcmp(cur->key, key) > 0) { 
            prev = cur;
            cur = cur->left;
        }
        else { 
            prev = cur;
            cur = cur->right;
        }
    }
    if(cur == NULL) {
        if(strcmp(prev->key, key) > 0) {
            Tnode n = (Tnode) malloc(sizeof(struct tnode));
            n->val = val;
            strcpy(n->key, key);
            n->deleted = PRESENT;
            if(d->root == NULL) d->root = n;
            else prev->left = n;
            return 0;
        }        
        else {
            Tnode n = (Tnode) malloc(sizeof(struct tnode));
            n->val = val;
            strcpy(n->key, key);
            n->deleted = PRESENT;
            if(d->root == NULL) d->root = n; 
            else prev->right = n;
            return 0;
        }
    }
    else {
        if(cur->deleted == DELETED) cur->deleted = PRESENT;
        cur->val = val;
        return 1;
    }
}

/*returns value associated with the key*/

int lookup(SIdict d, char* key) {
    Tnode cur = d->root;
    while(strcmp(cur->key, key) != 0) {
        if(strcmp(cur->key, key) > 0) cur = cur->left;
        else cur = cur->right;
    }
    return cur->val;
}

/*returns 1 if a node was deleted, 0 if the key didn't exist*/

int remKey(SIdict d, char* key) {
    Tnode cur = d->root;
    Tnode prev = NULL;
    while(cur != NULL && strcmp(cur->key, key) != 0) {
        if(strcmp(cur->key, key) > 0) {
            prev = cur;
            cur = cur->left;
        }
        else { 
            prev = cur;
            cur = cur->right;
        }
    }
    if(cur == NULL) return 0;
    if(cur->right == NULL && cur->left == NULL) {
        cur->val = NULL;
        cur->key = NULL;
        free(cur);
    }
    if(cur->right == NULL || cur->left == NULL) {
        if(cur->right == NULL) {
            cur->val = NULL; 
            cur->key = NULL;
            prev->
                        
        }
    }
    
    else {
        Tnode new = cur;
        


        cur->deleted = DELETED;
        return 1;
    }
}

int main() {
    SIdict d = makeSIdict();
    for(int i = 97; i < 102; i++) {
        char str[] = {i, '\0'};
        char* fluke = "jerrygarcia";
        str[0] = i;
        addOrUpdate(d, str, i-96);
        if(hasKey(d, fluke) == 1) printf("%s\n", fluke);
        if(i % 2 == 1) {
                remKey(d, str);
        }
        addOrUpdate(d, str, i-96);
        if(hasKey(d, str) == 1) printf("TEST #%d\n", lookup(d, str));
    }
}
