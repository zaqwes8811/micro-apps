#include "Vector.hpp"
#include <iostream>

void testVector3() {
    Vector3 v1(1.0f, 2.0f, 3.0f);
    Vector3 v2(4.0f, 5.0f, 6.0f);

    std::cout << "v1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
    std::cout << "v2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;

    Vector3 result = v1.multiply(v2);
    std::cout << "v1 * v2: (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;

    result = v1.subtract(v2);
    std::cout << "v1 - v2: (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;
}

int main() {
    testVector3();
    return 0;
}
