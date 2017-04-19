/* BRENDAN VON HOFE
 * I pledge my honor that I have abided
 * by the Stevens Honor System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sv_dict* SVdict;
struct sv_dict {
    void* val;
    char* key;
    int balance;
    SVdict left;
    SVdict right;
    SVdict parent;
};  

SVdict makeSVdict() {
    SVdict d = malloc(sizeof(struct sv_dict));
    d->val = NULL;
    d->key = NULL;
    d->balance = 0;
    d->left = NULL;
    d->right = NULL;
    d->parent = NULL;
    return d;
}

int hasKey(SVdict d, char* key) {
    if(d->key == NULL) return 0;    
    while(d != NULL) {
        if(strcmp(d->key, key) == 0) return 1;
        else if(strcmp(d->key, key) > 0) d = d->left;
        else d = d->right;
    }
    return 0;
}

int strLen(char* str) {
    char cur = str[0];
    int len = 0;
    while(cur != NULL) {
        len++;
        cur = str[len];
    }
    return len+1;
}

void preorderKeys(SVdict d, int indent) {
    for(int i = 0; i < indent; i++) {
        printf(" ");
    }
    if(d != NULL) {
        printf("%s - %d\n", d->key, d->balance);
        preorderKeys(d->left, indent+1);
        preorderKeys(d->right, indent+1);
    }
    else {
        printf("*\n");
    }
}

int max(int x, int y) {
    if(x == y) return x;
    else if (x > y) return x;
    else return y;
}

void swapKeyAndVal(SVdict d, SVdict d2) {
    char* keyTemp = d->key;
    void* valTemp = d->val;
    d->key = d2->key;
    d->val = d2->val;
    d2->key = keyTemp;
    d2->val = valTemp;
}

void leftRotation(SVdict d) {
    SVdict rightTemp = d->right;
    SVdict leftTemp = d->left;
    SVdict rightLeftTemp = d->right->left;
    swapKeyAndVal(d, d->right);
    d->right = d->right->right;
    d->left = rightTemp;
    d->left->left = leftTemp;
    d->left->right = rightLeftTemp;
}


void rightRotation(SVdict d) {
    SVdict leftTemp = d->left;
    SVdict rightTemp = d->right;
    SVdict leftRightTemp = d->left->right;
    swapKeyAndVal(d, d->left);
    d->left = d->left->left;
    d->right = leftTemp;
    d->right->right = rightTemp;
    d->right->left = leftRightTemp;
}

void doubleLeftRightRotation(SVdict d) {
    leftRotation(d->left);
    rightRotation(d);
}

void doubleRightLeftRotation(SVdict d) {
    rightRotation(d->right);
    leftRotation(d);
}


void rotate(SVdict d) {
    if(d->balance == 2) {
        if(d->left->balance == 1) {
            rightRotation(d);
        }
        else if(d->left->balance == -1) {
            doubleLeftRightRotation(d);
        }
    } else if(d->balance == -2) {
        if(d->right->balance == -1) {
            leftRotation(d);
        } else if(d->right->balance == 1) {
            doubleRightLeftRotation(d);
        }
    }
    updateBalance(d);
}

int getHeight(SVdict d) {
    if(d == NULL) return 0;
    if(d->right != NULL && d->left != NULL) {
        return max(1 + getHeight(d->left), 1 + getHeight(d->right));
    } else if(d->left == NULL && d->right == NULL) {
        return 1; 
    } else if(d->left == NULL) {
        return 1 + getHeight(d->right);
    } else {
        return 1 + getHeight(d->left);
    }
}

int getBalance(SVdict d) {
    return (getHeight(d->left)-getHeight(d->right));
}

int reBalance(SVdict d) {
    if(d == NULL) return 0;
    d->balance = getBalance(d);
    reBalance(d->left);
    reBalance(d->right);
    if(d->balance > 1 || d->balance < -1) {
        rotate(d); 
        return -1;
    }
    updateBalance(d);
    return 1;
}

int updateBalance(SVdict d) {
    if(d == NULL) return 0;
    d->balance = getBalance(d);
    reBalance(d->left);
    reBalance(d->right);
    return 1;
}

//HAVE TO PASS ROOT OF TREE
int addOrUpdate(SVdict d, char* key, void* val) {
    if(d->key == NULL) {
        d->val = val;
        int keyLen = strLen(key);
        d->key = malloc(keyLen * sizeof(char));
        strcpy(d->key, key);
        return 0;
    }

    SVdict node = d;
    SVdict par = d;
    while(node != NULL) {
        if(strcmp(node->key, key) == 0) {
            node->val = val;
            return 1;
        }
        else if(strcmp(node->key, key) > 0) {
            par = node;
            node = node->left;
        }
        else {
            par = node;
            node = node->right;
        }
    }
    SVdict new = malloc(sizeof(struct sv_dict));
    new->val = val;
    int keyLen = strLen(key);
    new->key = malloc(keyLen * sizeof(char));
    strcpy(new->key, key);
    new->balance = 0;
    new->left = NULL;
    new->right = NULL;
    new->parent = par;
    if(strcmp(par->key, key) > 0) par->left = new;
    else par->right = new;
    reBalance(d);
    return 0;
}

int remKey(SVdict d, char* key) {
    if(hasKey(d, key) == 0) return 0;
    while(d != NULL) {
        if(strcmp(d->key, key) == 0) break;
        else if(strcmp(d->key, key) > 0) d = d->left;
        else d = d->right; 
    }
    if(d->parent == NULL) {
        if(d->right == NULL && d->left == NULL) {
            free(d->key);
            free(d);
        } else if(d->right == NULL) {
            swapKeyAndVal(d, d->left);
            SVdict temp = d->left;
            d->left = NULL;
            free(temp->key);
            free(temp);
        } else if(d->left == NULL) {
            swapKeyAndVal(d, d->right);
            SVdict temp = d->right;
            d->right = NULL;
            free(temp->key);
            free(temp);
        } else {
            SVdict temp = d->right;
            while(temp->left != NULL) {
                temp = temp->left;
            }
            if(temp->right != NULL) temp = temp->right;
            swapKeyAndVal(d, temp);
            if(temp->parent->left == temp) temp->parent->left = NULL;
            else if(temp->parent->right == temp) temp->parent->right = NULL;
            free(temp->key);
            free(temp);
        }
    } else if(d->right == NULL && d->left == NULL) {
        if(d->parent->right == d) d->parent->right = NULL;
        else if(d->parent->left == d) d->parent->left = NULL;
        free(d->key);
        free(d);
        return 1;
    } else if(d->right == NULL) {
        if(d->parent->right == d) {
            d->parent->right = d->left;
        } else {
            d->parent->left = d->left;
        }
    } else if(d->left == NULL) {
        if(d->parent->right == d) {
            d->parent->right = d->right;
        } else {
            d->parent->left = d->right;
        }
 
    } else {
        if(d->parent->right == d) {
            SVdict temp = d->right;
            while(temp->left != NULL) {
                temp = temp->left;
            }
            printf("TEMP THAT SHOULD BE T: %s\n", temp->key);
            swapKeyAndVal(d, temp);
            if(temp->parent->right == temp) temp->parent->right = NULL;
            else if(temp->parent->left == temp) temp->parent->left = NULL;
            free(temp->key);
            free(temp);
            return 1;
        } else {
            SVdict temp = d->left;
            while(temp->right != NULL) {
                temp = temp->right;
            }
            swapKeyAndVal(d, temp);
            if(temp->parent->right == temp) temp->parent->right = NULL;
            else if(temp->parent->left == temp) temp->parent->left = NULL;
            free(temp->key);
            free(temp);
            return 1;
        }
    }
    reBalance(d);
}

void* lookup(SVdict d, char* key) {
    if(d->key == NULL) return 0;    
    while(d != NULL) {
        if(strcmp(d->key, key) == 0) return d->val;
        else if(strcmp(d->key, key) > 0) d = d->left;
        else d = d->right;
    }
    return NULL;
}

void disposeSVdict(SVdict d) {
    if(d != NULL) {
        SVdict left = d->left;
        SVdict right = d->right;
        free(d->key);
        free(d);
        disposeSVdict(left);
        disposeSVdict(right);
    }
}

int main() {
    SVdict d = makeSVdict();


    addOrUpdate(d, "n", "a");
    addOrUpdate(d, "g", "c");
//    addOrUpdate(d, "t", "b");
    addOrUpdate(d, "d", "d");
    addOrUpdate(d, "k", "e");
//    addOrUpdate(d, "s", "j");
    addOrUpdate(d, "b", "k");
    addOrUpdate(d, "e", "f");
    addOrUpdate(d, "i", "g");
    addOrUpdate(d, "m", "h");
//    addOrUpdate(d, "p", "i");
//    addOrUpdate(d, "r", "l");
//    addOrUpdate(d, "q", "t");
    addOrUpdate(d, "a", "blah");
    addOrUpdate(d, "l", "hen");
//    addOrUpdate(d, "y", "ddi");
//    addOrUpdate(d, "o", "ucnnee");
    addOrUpdate(d, "h", "due");
    addOrUpdate(d, "c", "bem");
    addOrUpdate(d, "f", "lue");
//    addOrUpdate(d, "x", "pem");
    addOrUpdate(d, "j", "oo");
//    addOrUpdate(d, "u", "fff");
//    addOrUpdate(d, "v", "jee");
//    addOrUpdate(d, "w", "bbr");
//    addOrUpdate(d, "z", "vvv");

    preorderKeys(d, 0);
    disposeSVdict(d);
}

