// fibonacci_iter.cpp
// Prints the first N Fibonacci numbers using an iterative loop.
// Compile: g++ -std=c++17 fibonacci_iter.cpp -o fibonacci_iter
// Run:     ./fibonacci_iter

#include <iostream>

int main() {
    const int N = 10;

    std::cout << "Fibonacci sequence (iterative), first " << N << " terms:" << std::endl;

    int a = 0, b = 1;
    for (int i = 0; i < N; i++) {
        std::cout << "  F(" << i << ") = " << a << std::endl;
        int next = a + b;
        a = b;
        b = next;
    }

    return 0;
}
