// fibonacci_iter.c
// Prints the first N Fibonacci numbers using an iterative loop.
// Compile: gcc -std=c11 fibonacci_iter.c -o fibonacci_iter
// Run:     ./fibonacci_iter

#include <stdio.h>

int main(void) {
    const int N = 10;

    printf("Fibonacci sequence (iterative), first %d terms:\n", N);

    int a = 0, b = 1;
    for (int i = 0; i < N; i++) {
        printf("  F(%d) = %d\n", i, a);
        int next = a + b;
        a = b;
        b = next;
    }

    return 0;
}
