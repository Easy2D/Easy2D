#pragma once

#include <easy2d/core/types.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

namespace easy2d {

// ---------------------------------------------------------------------------
// 常量
// ---------------------------------------------------------------------------
constexpr float PI_F       = 3.14159265358979323846f;
constexpr float DEG_TO_RAD = PI_F / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI_F;

// ---------------------------------------------------------------------------
// 2D 向量
// ---------------------------------------------------------------------------
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Vec2() = default;
    constexpr Vec2(float x, float y) : x(x), y(y) {}
    explicit Vec2(const glm::vec2& v) : x(v.x), y(v.y) {}

    glm::vec2 toGlm() const { return {x, y}; }
    static Vec2 fromGlm(const glm::vec2& v) { return {v.x, v.y}; }

    // 基础运算
    Vec2 operator+(const Vec2& v) const { return {x + v.x, y + v.y}; }
    Vec2 operator-(const Vec2& v) const { return {x - v.x, y - v.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }
    Vec2 operator/(float s) const { return {x / s, y / s}; }
    Vec2 operator-() const { return {-x, -y}; }

    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    Vec2& operator*=(float s) { x *= s; y *= s; return *this; }
    Vec2& operator/=(float s) { x /= s; y /= s; return *this; }

    bool operator==(const Vec2& v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vec2& v) const { return !(*this == v); }

    // 向量运算
    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSquared() const { return x * x + y * y; }

    Vec2 normalized() const {
        float len = length();
        if (len > 0.0f) return {x / len, y / len};
        return {0.0f, 0.0f};
    }

    float dot(const Vec2& v) const { return x * v.x + y * v.y; }
    float cross(const Vec2& v) const { return x * v.y - y * v.x; }

    float distance(const Vec2& v) const { return (*this - v).length(); }
    float angle() const { return std::atan2(y, x) * RAD_TO_DEG; }

    static Vec2 lerp(const Vec2& a, const Vec2& b, float t) {
        return a + (b - a) * t;
    }

    static constexpr Vec2 Zero()  { return {0.0f, 0.0f}; }
    static constexpr Vec2 One()   { return {1.0f, 1.0f}; }
    static constexpr Vec2 UnitX() { return {1.0f, 0.0f}; }
    static constexpr Vec2 UnitY() { return {0.0f, 1.0f}; }
};

inline Vec2 operator*(float s, const Vec2& v) { return v * s; }

using Point = Vec2;

// ---------------------------------------------------------------------------
// 3D 向量 (用于3D动作)
// ---------------------------------------------------------------------------
struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    constexpr Vec3() = default;
    constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit Vec3(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}

    glm::vec3 toGlm() const { return {x, y, z}; }
    static Vec3 fromGlm(const glm::vec3& v) { return {v.x, v.y, v.z}; }

    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }
    Vec3 operator-() const { return {-x, -y, -z}; }

    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    Vec3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

    bool operator==(const Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
    bool operator!=(const Vec3& v) const { return !(*this == v); }

    float length() const { return std::sqrt(x * x + y * y + z * z); }
    float lengthSquared() const { return x * x + y * y + z * z; }

    Vec3 normalized() const {
        float len = length();
        if (len > 0.0f) return {x / len, y / len, z / len};
        return {0.0f, 0.0f, 0.0f};
    }

    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

    static Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
        return a + (b - a) * t;
    }

    static constexpr Vec3 Zero()  { return {0.0f, 0.0f, 0.0f}; }
    static constexpr Vec3 One()   { return {1.0f, 1.0f, 1.0f}; }
};

inline Vec3 operator*(float s, const Vec3& v) { return v * s; }

// ---------------------------------------------------------------------------
// 2D 尺寸
// ---------------------------------------------------------------------------
struct Size {
    float width  = 0.0f;
    float height = 0.0f;

    constexpr Size() = default;
    constexpr Size(float w, float h) : width(w), height(h) {}

    bool operator==(const Size& s) const { return width == s.width && height == s.height; }
    bool operator!=(const Size& s) const { return !(*this == s); }

    float area() const { return width * height; }
    bool empty() const { return width <= 0.0f || height <= 0.0f; }

    static constexpr Size Zero() { return {0.0f, 0.0f}; }
};

// ---------------------------------------------------------------------------
// 2D 矩形
// ---------------------------------------------------------------------------
struct Rect {
    Point origin;
    Size  size;

    constexpr Rect() = default;
    constexpr Rect(float x, float y, float w, float h) : origin(x, y), size(w, h) {}
    constexpr Rect(const Point& o, const Size& s) : origin(o), size(s) {}

    float left()   const { return origin.x; }
    float top()    const { return origin.y; }
    float right()  const { return origin.x + size.width; }
    float bottom() const { return origin.y + size.height; }
    float width()  const { return size.width; }
    float height() const { return size.height; }
    Point center() const { return {origin.x + size.width * 0.5f, origin.y + size.height * 0.5f}; }

    bool empty() const { return size.empty(); }

    bool containsPoint(const Point& p) const {
        return p.x >= left() && p.x <= right() &&
               p.y >= top()  && p.y <= bottom();
    }

    bool contains(const Rect& r) const {
        return r.left() >= left() && r.right() <= right() &&
               r.top() >= top() && r.bottom() <= bottom();
    }

    bool intersects(const Rect& r) const {
        return !(left() > r.right() || right() < r.left() ||
                 top() > r.bottom() || bottom() < r.top());
    }

    Rect intersection(const Rect& r) const {
        float l = std::max(left(), r.left());
        float t = std::max(top(), r.top());
        float ri = std::min(right(), r.right());
        float b = std::min(bottom(), r.bottom());
        if (l < ri && t < b) return {l, t, ri - l, b - t};
        return {};
    }

    Rect unionWith(const Rect& r) const {
        if (empty()) return r;
        if (r.empty()) return *this;
        float l = std::min(left(), r.left());
        float t = std::min(top(), r.top());
        float ri = std::max(right(), r.right());
        float b = std::max(bottom(), r.bottom());
        return {l, t, ri - l, b - t};
    }

    bool operator==(const Rect& r) const { return origin == r.origin && size == r.size; }
    bool operator!=(const Rect& r) const { return !(*this == r); }

    static constexpr Rect Zero() { return {0, 0, 0, 0}; }
};

// ---------------------------------------------------------------------------
// 2D 变换矩阵（基于 glm::mat4，兼容 OpenGL）
// ---------------------------------------------------------------------------
struct Transform2D {
    glm::mat4 matrix{1.0f}; // 单位矩阵

    Transform2D() = default;
    explicit Transform2D(const glm::mat4& m) : matrix(m) {}

    static Transform2D identity() { return Transform2D{}; }

    static Transform2D translation(float x, float y) {
        Transform2D t;
        t.matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
        return t;
    }

    static Transform2D translation(const Vec2& v) {
        return translation(v.x, v.y);
    }

    static Transform2D rotation(float degrees) {
        Transform2D t;
        t.matrix = glm::rotate(glm::mat4(1.0f), degrees * DEG_TO_RAD, glm::vec3(0.0f, 0.0f, 1.0f));
        return t;
    }

    static Transform2D scaling(float sx, float sy) {
        Transform2D t;
        t.matrix = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, 1.0f));
        return t;
    }

    static Transform2D scaling(float s) {
        return scaling(s, s);
    }

    static Transform2D skewing(float skewX, float skewY) {
        Transform2D t;
        t.matrix = glm::mat4(1.0f);
        t.matrix[1][0] = std::tan(skewX * DEG_TO_RAD);
        t.matrix[0][1] = std::tan(skewY * DEG_TO_RAD);
        return t;
    }

    Transform2D operator*(const Transform2D& other) const {
        return Transform2D(matrix * other.matrix);
    }

    Transform2D& operator*=(const Transform2D& other) {
        matrix *= other.matrix;
        return *this;
    }

    Vec2 transformPoint(const Vec2& p) const {
        glm::vec4 result = matrix * glm::vec4(p.x, p.y, 0.0f, 1.0f);
        return {result.x, result.y};
    }

    Transform2D inverse() const {
        return Transform2D(glm::inverse(matrix));
    }
};

// ---------------------------------------------------------------------------
// 数学工具函数
// ---------------------------------------------------------------------------
namespace math {

inline float clamp(float value, float minVal, float maxVal) {
    return std::clamp(value, minVal, maxVal);
}

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float degrees(float radians) {
    return radians * RAD_TO_DEG;
}

inline float radians(float degrees) {
    return degrees * DEG_TO_RAD;
}

} // namespace math

} // namespace easy2d
