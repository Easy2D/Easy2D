#pragma once

#include <easy2d/core/types.h>
#include <glm/vec4.hpp>
#include <algorithm>

namespace easy2d {

/// RGBA 颜色（浮点数，每通道 0.0 - 1.0）
struct Color {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;

    constexpr Color() = default;

    constexpr Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}

    /// 从 0xRRGGBB 整数构造
    constexpr explicit Color(uint32_t rgb, float a = 1.0f)
        : r(static_cast<float>((rgb >> 16) & 0xFF) / 255.0f)
        , g(static_cast<float>((rgb >> 8)  & 0xFF) / 255.0f)
        , b(static_cast<float>((rgb)       & 0xFF) / 255.0f)
        , a(a) {}

    /// 从 0-255 整数构造
    static constexpr Color fromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    /// 转换为 glm::vec4
    glm::vec4 toVec4() const { return {r, g, b, a}; }

    /// 线性插值
    static Color lerp(const Color& a, const Color& b, float t) {
        t = std::clamp(t, 0.0f, 1.0f);
        return Color(
            a.r + (b.r - a.r) * t,
            a.g + (b.g - a.g) * t,
            a.b + (b.b - a.b) * t,
            a.a + (b.a - a.a) * t
        );
    }

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }
};

// 命名颜色常量
namespace Colors {
    inline constexpr Color White       {1.0f, 1.0f, 1.0f, 1.0f};
    inline constexpr Color Black       {0.0f, 0.0f, 0.0f, 1.0f};
    inline constexpr Color Red         {1.0f, 0.0f, 0.0f, 1.0f};
    inline constexpr Color Green       {0.0f, 1.0f, 0.0f, 1.0f};
    inline constexpr Color Blue        {0.0f, 0.0f, 1.0f, 1.0f};
    inline constexpr Color Yellow      {1.0f, 1.0f, 0.0f, 1.0f};
    inline constexpr Color Cyan        {0.0f, 1.0f, 1.0f, 1.0f};
    inline constexpr Color Magenta     {1.0f, 0.0f, 1.0f, 1.0f};
    inline constexpr Color Orange      {1.0f, 0.647f, 0.0f, 1.0f};
    inline constexpr Color Purple      {0.502f, 0.0f, 0.502f, 1.0f};
    inline constexpr Color Pink        {1.0f, 0.753f, 0.796f, 1.0f};
    inline constexpr Color Gray        {0.502f, 0.502f, 0.502f, 1.0f};
    inline constexpr Color LightGray   {0.827f, 0.827f, 0.827f, 1.0f};
    inline constexpr Color DarkGray    {0.412f, 0.412f, 0.412f, 1.0f};
    inline constexpr Color Brown       {0.647f, 0.165f, 0.165f, 1.0f};
    inline constexpr Color Gold        {1.0f, 0.843f, 0.0f, 1.0f};
    inline constexpr Color Silver      {0.753f, 0.753f, 0.753f, 1.0f};
    inline constexpr Color SkyBlue     {0.529f, 0.808f, 0.922f, 1.0f};
    inline constexpr Color LimeGreen   {0.196f, 0.804f, 0.196f, 1.0f};
    inline constexpr Color Coral       {1.0f, 0.498f, 0.314f, 1.0f};
    inline constexpr Color Transparent {0.0f, 0.0f, 0.0f, 0.0f};
}

// 为了向后兼容，在 Color 结构体内提供静态引用
struct ColorConstants {
    static const Color& White;
    static const Color& Black;
    static const Color& Red;
    static const Color& Green;
    static const Color& Blue;
    static const Color& Yellow;
    static const Color& Cyan;
    static const Color& Magenta;
    static const Color& Transparent;
};

} // namespace easy2d
