#include "scan.h"
#include <stdio.h>

int main() {
    int token;
    char name[MAX_TK_LEN];
    initscan();
    while ((token = get_next_token(name))) {
        printf("%4d:\t%8s\t%d:%d\n", token, name, row, col);
    }
}
