#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>
#include <easy2d/core/color.h>
#include <easy2d/core/string.h>
#include <variant>
#include <vector>

namespace easy2d {

// 前向声明
class Texture;
class FontAtlas;

// ============================================================================
// 渲染命令类型
// ============================================================================
enum class RenderCommandType {
    Sprite,
    Line,
    Rect,
    FilledRect,
    Circle,
    FilledCircle,
    Triangle,
    FilledTriangle,
    Polygon,
    FilledPolygon,
    Text
};

// ============================================================================
// 精灵数据
// ============================================================================
struct SpriteData {
    Ptr<Texture> texture;
    Rect destRect;
    Rect srcRect;
    Color tint;
    float rotation;
    Vec2 anchor;
};

// ============================================================================
// 直线数据
// ============================================================================
struct LineData {
    Vec2 start;
    Vec2 end;
    Color color;
    float width;
};

// ============================================================================
// 矩形数据
// ============================================================================
struct RectData {
    Rect rect;
    Color color;
    float width;
};

// ============================================================================
// 圆形数据
// ============================================================================
struct CircleData {
    Vec2 center;
    float radius;
    Color color;
    int segments;
    float width;
};

// ============================================================================
// 三角形数据
// ============================================================================
struct TriangleData {
    Vec2 p1;
    Vec2 p2;
    Vec2 p3;
    Color color;
    float width;
};

// ============================================================================
// 多边形数据
// ============================================================================
struct PolygonData {
    std::vector<Vec2> points;
    Color color;
    float width;
};

// ============================================================================
// 文字数据
// ============================================================================
struct TextData {
    Ptr<FontAtlas> font;
    String text;
    Vec2 position;
    Color color;
};

// ============================================================================
// 渲染命令
// ============================================================================
struct RenderCommand {
    RenderCommandType type;
    int zOrder;

    std::variant<
        SpriteData,
        LineData,
        RectData,
        CircleData,
        TriangleData,
        PolygonData,
        TextData
    > data;

    // 用于排序
    bool operator<(const RenderCommand& other) const {
        return zOrder < other.zOrder;
    }
};

} // namespace easy2d
