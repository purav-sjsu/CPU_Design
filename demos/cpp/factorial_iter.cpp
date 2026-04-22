// factorial_iter.cpp
// Prints n! for n = 0 to 12 using an iterative loop.
// Compile: g++ -std=c++17 factorial_iter.cpp -o factorial_iter
// Run:     ./factorial_iter

#include <iostream>

int main() {
    std::cout << "Factorial (iterative):" << std::endl;

    long long result = 1;
    for (int n = 0; n <= 12; n++) {
        if (n > 0) result *= n;
        std::cout << "  " << n << "! = " << result << std::endl;
    }

    return 0;
}
