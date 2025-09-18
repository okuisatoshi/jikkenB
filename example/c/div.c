#include <stdio.h>
#include <limits.h>

int main()
{
    int n;
    scanf("%d", &n);
    // n = 12;
    // n = 1;
    // n = 99999989;
    // n = INT_MAX;
    // n = INT_MAX - 1;
    int i = 1;
    while (i <= n) {
        if (n % i <= 0) printf("%d\n", i);
        i = i + 1;
    }
}
