#include <stdio.h>

int main()
{
    printf("1\n"); // F_0
    int f0 = 1;
    int f1 = 1;
    for (int n = 1; n < 30; n++) {
        int f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
        printf("%d\n", f0); // F_n
    }
}
