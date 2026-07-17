#include "Vec2.hpp"
#include <iostream>

void testVec2() {
    Vec2<int> v1(1, 2);
    Vec2<float> v2(3.5f, 4.5f);

    std::cout << "v1: (" << v1.x << ", " << v1.y << ")" << std::endl;
    std::cout << "v2: (" << v2.x << ", " << v2.y << ")" << std::endl;

    Vec2<int> result = v1 * 2;
    std::cout << "v1 * 2: (" << result.x << ", " << result.y << ")" << std::endl;

    result = v2.subtract(v1);
    std::cout << "v2 - v1: (" << result.x << ", " << result.y << ")" << std::endl;
}

int main() {
    testVec2();
    return 0;
}
