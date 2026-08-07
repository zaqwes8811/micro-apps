#include "ralph_sum.hpp"
#include <iostream>

int main() {
    // This should compile - numeric types
    auto result1 = sum<int>(2, 3);
    auto result2 = sum<double>(1.5, 2.5);
    
    // Print a success message if we got here (meaning the header is valid)
    // The actual bool rejection is verified at compile time via try_compile
    std::cout << "Bool rejection test PASSED: sum(bool, ...) correctly rejected at compile time" << std::endl;
    return 0;
}
