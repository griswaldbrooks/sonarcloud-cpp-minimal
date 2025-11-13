#pragma once

namespace trivial {

// Constexpr functions for compile-time evaluation
constexpr int add(int a, int b) {
    return a + b;
}

constexpr int multiply(int a, int b) {
    return a * b;
}

constexpr int subtract(int a, int b) {
    return a - b;
}

constexpr int divide(int a, int b) {
    // Division by zero returns 0
    return (b != 0) ? (a / b) : 0;
}

// Runtime versions for coverage testing
int add_runtime(int a, int b);
int multiply_runtime(int a, int b);
int subtract_runtime(int a, int b);
int divide_runtime(int a, int b);

} // namespace trivial
