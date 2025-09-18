#include <stdio.h>

int main()
{
    int n = 100000;
    printf("2\n");
    for (int i = 3; i < n; i += 2) {
        int j = 2;
        for (; j * j <= i; j++) {
            if (i % j == 0) break;
        }
        if (j * j > i) printf("%d\n", i);
    }
}
