#include <iostream>
#include <stdio.h>
#include <vector>

template<typename T>
T add(T a, T b) requires std::integral<T> {
    return a + b;
}

template<typename T>
std::vector<T> subtract(const std::vector<T>& a, const std::vector<T>& b) requires std::integral<T> {
    return a + b;
}

template<typename T>
std::vector<T> subtract(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Vector sizes must be equal");
    }
    std::vector<T> result(a.size()); // reserve memory
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

int main() {
    std::cout << "Hello" << std::endl;
    printf("Hello, World!\n");
    int result = add(5, 3);
    printf("5 + 3 = %d\n", result);
    
    // Method 1: Single element vectors using initializer list
    std::vector<float> a = {1.5f};
    std::vector<float> b = {2.5f};
    std::vector<float> diff = subtract(a, b);
    printf("Subtract vectors: ");
    for (const auto& val : diff) {
        printf("%.1f ", val);
    }
    printf("\n");
    
    return 0;
}

