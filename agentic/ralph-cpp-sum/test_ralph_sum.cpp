/**
 * @file test_ralph_sum.cpp
 * @brief Gtest tests for the template sum function
 */

#include "ralph_sum.hpp"
#include <gtest/gtest.h>
#include <climits>

// Tests for integer types
TEST(RalphSumTest, IntSum) {
    EXPECT_EQ(sum(2, 3), 5);
    EXPECT_EQ(sum(-10, 5), -5);
    EXPECT_EQ(sum(0, 0), 0);
    EXPECT_EQ(sum(INT_MAX, 0), INT_MAX);
}

TEST(RalphSumTest, FloatSum) {
    EXPECT_DOUBLE_EQ(sum(1.5f, 2.5f), 4.0f);
    EXPECT_DOUBLE_EQ(sum(-3.14f, 2.71f), -0.43000006675720215f);
    EXPECT_DOUBLE_EQ(sum(0.0f, 0.0f), 0.0f);
}

TEST(RalphSumTest, DoubleSum) {
    EXPECT_DOUBLE_EQ(sum(1.0, 2.0), 3.0);
    EXPECT_DOUBLE_EQ(sum(-1.5, 2.5), 1.0);
    EXPECT_DOUBLE_EQ(sum(0.0, 0.0), 0.0);
}

TEST(RalphSumTest, LongIntSum) {
    EXPECT_EQ(sum(1000000000LL, 2000000000LL), 3000000000LL);
    EXPECT_EQ(sum(-5000000000LL, 1000000000LL), -4000000000LL);
}

TEST(RalphSumTest, BooleanSum) {
    // In C++, bool can be added to numbers (1 = true, 0 = false)
    EXPECT_EQ(sum(true, false), 1);  // true + false = 1
    EXPECT_EQ(sum(false, false), 0);  // false + false = 0
    EXPECT_EQ(sum(true, true), 1);  // true + true = 1 (C++ bool addition)
}

TEST(RalphSumTest, NegativeFloatSum) {
    EXPECT_DOUBLE_EQ(sum(-1.5, -2.5), -4.0);
    EXPECT_DOUBLE_EQ(sum(-10.0, -20.0), -30.0);
}
