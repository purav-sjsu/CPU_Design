// factorial_rec.cpp
// Prints n! for n = 0 to 12 using recursion.
// Compile: g++ -std=c++17 factorial_rec.cpp -o factorial_rec
// Run:     ./factorial_rec

#include <iostream>

// Returns n! recursively. Assumes n >= 0.
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    std::cout << "Factorial (recursive):" << std::endl;

    for (int n = 0; n <= 12; n++) {
        std::cout << "  " << n << "! = " << factorial(n) << std::endl;
    }

    return 0;
}
