#include <gtest/gtest.h>
#include "trivial_math.h"

// Test constexpr functions (compile-time)
TEST(TrivialMath, AdditionConstexpr) {
    static_assert(trivial::add(2, 3) == 5, "Compile-time add test");
    EXPECT_EQ(trivial::add(2, 3), 5);
}

TEST(TrivialMath, MultiplicationConstexpr) {
    static_assert(trivial::multiply(2, 3) == 6, "Compile-time multiply test");
    EXPECT_EQ(trivial::multiply(2, 3), 6);
}

// Test runtime functions (for coverage)
TEST(TrivialMath, AdditionRuntime) {
    EXPECT_EQ(trivial::add_runtime(2, 3), 5);
    EXPECT_EQ(trivial::add_runtime(-1, 1), 0);
    EXPECT_EQ(trivial::add_runtime(0, 0), 0);
}

TEST(TrivialMath, MultiplicationRuntime) {
    EXPECT_EQ(trivial::multiply_runtime(2, 3), 6);
    EXPECT_EQ(trivial::multiply_runtime(-2, 3), -6);
    EXPECT_EQ(trivial::multiply_runtime(0, 5), 0);
}

TEST(TrivialMath, SubtractionRuntime) {
    EXPECT_EQ(trivial::subtract_runtime(5, 3), 2);
    EXPECT_EQ(trivial::subtract_runtime(3, 5), -2);
    EXPECT_EQ(trivial::subtract_runtime(0, 0), 0);
}

TEST(TrivialMath, DivisionRuntime) {
    EXPECT_EQ(trivial::divide_runtime(6, 3), 2);
    EXPECT_EQ(trivial::divide_runtime(7, 3), 2);  // Integer division
    EXPECT_EQ(trivial::divide_runtime(5, 0), 0);  // Division by zero returns 0
}
