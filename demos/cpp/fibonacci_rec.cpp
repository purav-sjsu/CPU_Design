// fibonacci_rec.cpp
// Prints the first N Fibonacci numbers using recursion.
// Compile: g++ -std=c++17 fibonacci_rec.cpp -o fibonacci_rec
// Run:     ./fibonacci_rec

#include <iostream>

// Returns the nth Fibonacci number (0-indexed: fib(0)=0, fib(1)=1, ...).
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    const int N = 10;

    std::cout << "Fibonacci sequence (recursive), first " << N << " terms:" << std::endl;

    for (int i = 0; i < N; i++) {
        std::cout << "  F(" << i << ") = " << fib(i) << std::endl;
    }

    return 0;
}
