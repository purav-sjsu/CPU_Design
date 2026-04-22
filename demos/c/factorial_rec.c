// factorial_rec.c
// Prints n! for n = 0 to 12 using recursion.
// Compile: gcc -std=c11 factorial_rec.c -o factorial_rec
// Run:     ./factorial_rec

#include <stdio.h>

// Returns n! recursively. Assumes n >= 0.
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main(void) {
    printf("Factorial (recursive):\n");

    for (int n = 0; n <= 12; n++) {
        printf("  %d! = %lld\n", n, factorial(n));
    }

    return 0;
}
