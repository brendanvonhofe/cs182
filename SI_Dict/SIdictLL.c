/* Brendan Von Hofe and Taber McFarlin
 * "I pledge my honor I have abided by the Stevens Honor System."
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SIdict.h"

typedef struct node* Node;
struct node {
        int val;
        char* key;
        Node next;
};

struct si_dict {
        Node front;
};

SIdict makeSIdict() {
    SIdict d = (SIdict) malloc(sizeof(struct si_dict));
    d->front = NULL;
    return d;
}

int hasKey(SIdict dict, char*key) {
    Node current = dict->front;
    while(current != NULL && strcmp(current->key, key) != 0) {
        current = current->next;
    }
    if(current == NULL) return 0;
    else return 1;
}

int addOrUpdate(SIdict dict, char* key, int val) {
    Node current = dict->front;
    while(current != NULL && strcmp(current->key, key) != 0) {
        current = current->next;
    }
    if(current == NULL) {
        Node n = (Node) malloc(sizeof(struct node));
        n->val = val;
        n->key = key;
        n->next = dict->front;
        dict->front = n;
        return 0;
    }
    else {
        current->val = val;
        return 1;
    }
}

int lookup(SIdict dict, char* key) {
    Node current = dict->front;
    while(strcmp(current->key, key) != 0) {
        current = current->next;
    }
    return current->val;
}

int remKey(SIdict dict, char* key) {
    Node current = dict->front;
    Node prev = NULL;
    while(current != NULL && strcmp(current->key, key) != 0) {
        prev = current;
        current = current->next;
    }
    if(current == NULL) return 0;
    else if (prev == NULL) {
        dict->front = dict->front->next;
        free(current);
    }
    else {
        prev->next = current->next;
        free(current);
    }
    return 1;
}

int main() {
    SIdict d = makeSIdict();
    printf("Front: %p\n", d->front);
    char k[3] = "hi";
    printf("HasKey: %d\n", hasKey(d, k));
    printf("Add: %d\n", addOrUpdate(d, k, 5));
    printf("Lookup: %d\n", lookup(d, k));
    char k2[4] = "bye";
    char k3[4] = "sky";
    addOrUpdate(d, k2, 2);
    addOrUpdate(d, k3, 3);
    printf("Update: %d\n", addOrUpdate(d, k, 7));
    printf("Add: %d\n", addOrUpdate(d, k3, 4));
    printf("Lookupdate: %d\n", lookup(d, k));
    printf("RemKey: %d\n", remKey(d, k2));
    printf("HasKEy: %d\n", hasKey(d, k));
    char e[1] = "";
    printf("Haskey: %d\n", hasKey(d, e));
    printf("RemKey: %d\n", remKey(d, k));
    printf("Remkey: %d\n", remKey(d, k3));
    printf("RemKey: %d\n", remKey(d, k2));
    char s[5] = "four";
    printf("Add: %d\n", addOrUpdate(d, s, 48));
    if(hasKey(d, s) == 1) {
        printf("Lookup: %d\n", lookup(d, s));
    }
}
