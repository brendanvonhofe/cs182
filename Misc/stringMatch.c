#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


/* Brute force string match.

Assume text and pattern are null-terminated strings.
Return the index of the beginning position of the first
plase in text where pattern matches.  If pattern does
not match anywhere, return -1. 

Exercise: implement this using loop(s).

Hint: To determine the range of possible indices,
one option is to use strlen() from the standard library.
It returns the length not counting the \0 terminator.
Another option is to check for null (in both text and pattern) as part
of the matching process.
*/
int match(char* text, char* pattern) {

    return 0; // wrong answer

}


int main() {
    assert(match("found", "") == 0);
    assert(match("found", "found") == 0);
    assert(match("found", "lost") == -1);
    assert(match("there is more to life than its speed", "Ghandi") == -1);
    assert(match("nobody noticed him", "not") == 7);
    printf("tests successful\n");
}

    
