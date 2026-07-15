#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    // Add more methods as needed
    Vector3 multiply(const Vector3& other) const {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }

    Vector3 subtract(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
};

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

#endif // VECTOR_HPP
