// factorial_iter.c
// Prints n! for n = 0 to 12 using an iterative loop.
// Compile: gcc -std=c11 factorial_iter.c -o factorial_iter
// Run:     ./factorial_iter

#include <stdio.h>

int main(void) {
    printf("Factorial (iterative):\n");

    long long result = 1;
    for (int n = 0; n <= 12; n++) {
        if (n > 0) result *= n;
        printf("  %d! = %lld\n", n, result);
    }

    return 0;
}
