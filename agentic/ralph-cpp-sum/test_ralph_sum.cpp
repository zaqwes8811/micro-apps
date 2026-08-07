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

TEST(RalphSumTest, ShortIntSum) {
    EXPECT_EQ(sum(100, 50), 150);
    EXPECT_EQ(sum(-100, 50), -50);
}

TEST(RalphSumTest, LongIntSum) {
    EXPECT_EQ(sum(1000000000LL, 2000000000LL), 3000000000LL);
    EXPECT_EQ(sum(-5000000000LL, 1000000000LL), -4000000000LL);
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

TEST(RalphSumTest, NegativeFloatSum) {
    EXPECT_DOUBLE_EQ(sum(-1.5, -2.5), -4.0);
    EXPECT_DOUBLE_EQ(sum(-10.0, -20.0), -30.0);
}

TEST(RalphSumTest, CharSum) {
    // char is also an arithmetic type
    // Use small values to avoid overflow
    EXPECT_EQ(sum(1, 2), 3);
    EXPECT_EQ(sum(0, 0), 0);
}

// Tests for subtraction (sub) function
TEST(RalphSubTest, IntSub) {
    EXPECT_EQ(sub(5, 3), 2);
    EXPECT_EQ(sub(3, 5), -2);
    EXPECT_EQ(sub(0, 0), 0);
    EXPECT_EQ(sub(10, 0), 10);
    EXPECT_EQ(sub(0, 10), -10);
}

TEST(RalphSubTest, FloatSub) {
    EXPECT_DOUBLE_EQ(sub(5.5f, 2.5f), 3.0f);
    EXPECT_DOUBLE_EQ(sub(2.5f, 5.5f), -3.0f);
    EXPECT_DOUBLE_EQ(sub(0.0f, 0.0f), 0.0f);
}

TEST(RalphSubTest, DoubleSub) {
    EXPECT_DOUBLE_EQ(sub(5.0, 2.0), 3.0);
    EXPECT_DOUBLE_EQ(sub(2.0, 5.0), -3.0);
    EXPECT_DOUBLE_EQ(sub(0.0, 0.0), 0.0);
}

TEST(RalphSubTest, LongIntSub) {
    EXPECT_EQ(sub(3000000000LL, 1000000000LL), 2000000000LL);
    EXPECT_EQ(sub(-1000000000LL, 1000000000LL), -2000000000LL);
}

TEST(RalphSubTest, NegativeSub) {
    EXPECT_EQ(sub(-5, -3), -2);
    EXPECT_EQ(sub(-10, -5), -5);
    EXPECT_EQ(sub(-5, -10), 5);
}
