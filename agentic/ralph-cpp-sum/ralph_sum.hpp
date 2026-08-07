/**
 * @file ralph_sum.hpp
 * @brief Template sum function for numeric types (excluding bool)
 */

/**
 * @file ralph_sum.hpp
 * @brief Template sum and sub functions for numeric types (excluding bool)
 */

#ifndef RALPH_SUM_HPP
#define RALPH_SUM_HPP

#include <type_traits>

// Template function that returns the sum of two numbers of any numeric type
// Requires C++20 for `requires` clause (uses std::is_same_v instead of std::same_as)
template <typename T>
requires (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
constexpr T sum(T a, T b) {
    return a + b;
}

// Template function that returns the difference of two numbers of any numeric type
// Requires C++20 for `requires` clause (uses std::is_same_v instead of std::same_as)
template <typename T>
requires (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
constexpr T sub(T a, T b) {
    return a - b;
}

#endif // RALPH_SUM_HPP
