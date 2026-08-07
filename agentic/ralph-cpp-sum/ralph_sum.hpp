/**
 * @file ralph_sum.hpp
 * @brief Template sum function for any numeric type
 */

#ifndef RALPH_SUM_HPP
#define RALPH_SUM_HPP

// Template function that returns the sum of two numbers of any type
template <typename T>
constexpr T sum(T a, T b) {
    return a + b;
}

#endif // RALPH_SUM_HPP
