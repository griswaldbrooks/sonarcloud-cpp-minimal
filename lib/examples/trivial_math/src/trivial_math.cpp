#include "trivial_math.h"

namespace trivial {

// Runtime (non-constexpr) implementations for coverage testing
int add_runtime(int a, int b) {
    return add(a, b);
}

int multiply_runtime(int a, int b) {
    return multiply(a, b);
}

int subtract_runtime(int a, int b) {
    return subtract(a, b);
}

int divide_runtime(int a, int b) {
    return divide(a, b);
}

} // namespace trivial
