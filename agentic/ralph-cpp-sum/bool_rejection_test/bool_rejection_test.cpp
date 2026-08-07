#include "ralph_sum.hpp"
#include <iostream>

// This test is designed to FAIL at compile time
// It should not compile because sum(bool, bool) is rejected by the requires clause

int main() {
    // This should fail at compile time due to the requires clause
    auto result1 = sum(true, false);
    auto result2 = sum(bool{true}, bool{false});
    (void)result1;
    (void)result2;
    std::cout << "Result: " << result1 << " " << result2 << std::endl;
    return 0;
}
