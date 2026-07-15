#ifndef VEC2_HPP
#define VEC2_HPP

#include <iostream>

template<typename T>
class Vec2 {
public:
    T x;
    T y;

    Vec2(T x = 0.0f, T y = 0.0f) : x(x), y(y) {}

    // Add more methods as needed
    Vec2<T> multiply(const Vec2& other) const {
        return Vec2(x * other.x, y * other.y);
    }

    template<typename U>
    Vec2<T> subtract(const Vec2<U>& other) const {
        return Vec2(static_cast<T>(x - other.x), static_cast<T>(y - other.y));
    }
};

std::ostream& operator<<(std::ostream& os, const Vec2<int>& v) {
    os << "(" << static_cast<int>(v.x) << ", " << static_cast<int>(v.y) << ")";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

#endif // VEC2_HPP
