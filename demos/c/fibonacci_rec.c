// fibonacci_rec.c
// Prints the first N Fibonacci numbers using recursion.
// Compile: gcc -std=c11 fibonacci_rec.c -o fibonacci_rec
// Run:     ./fibonacci_rec

#include <stdio.h>

// Returns the nth Fibonacci number (0-indexed: fib(0)=0, fib(1)=1, ...).
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main(void) {
    const int N = 10;

    printf("Fibonacci sequence (recursive), first %d terms:\n", N);

    for (int i = 0; i < N; i++) {
        printf("  F(%d) = %d\n", i, fib(i));
    }

    return 0;
}
