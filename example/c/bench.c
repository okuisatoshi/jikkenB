#include <stdio.h>

int main()
{
    int n, c = 0;
    scanf("%d", &n);

    for (int i = n; i > 0; i--) { // n回反復
        for (int j = n; j > 0; j--) { // n回反復
            for (int k = n; k > 0; k--) { // n回反復
                n = n % 32768 + 1;
                c++;
            }
        }
    }

    printf("%d\n%d\n", n, c);
}
