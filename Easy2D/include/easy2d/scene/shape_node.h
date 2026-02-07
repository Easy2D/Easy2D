#pragma once

#include <easy2d/scene/node.h>
#include <easy2d/core/color.h>
#include <easy2d/core/math_types.h>
#include <vector>

namespace easy2d {

// ============================================================================
// 形状类型
// ============================================================================
enum class ShapeType {
    Point,
    Line,
    Rect,
    Circle,
    Triangle,
    Polygon
};

// ============================================================================
// 形状节点 - 用于绘制几何形状
// ============================================================================
class ShapeNode : public Node {
public:
    ShapeNode();
    ~ShapeNode() override = default;

    // ------------------------------------------------------------------------
    // 静态创建方法
    // ------------------------------------------------------------------------
    static Ptr<ShapeNode> create();
    
    // 点
    static Ptr<ShapeNode> createPoint(const Vec2& pos, const Color& color);
    
    // 线
    static Ptr<ShapeNode> createLine(const Vec2& start, const Vec2& end, 
                                     const Color& color, float width = 1.0f);
    
    // 矩形
    static Ptr<ShapeNode> createRect(const Rect& rect, const Color& color, 
                                     float width = 1.0f);
    static Ptr<ShapeNode> createFilledRect(const Rect& rect, const Color& color);
    
    // 圆形
    static Ptr<ShapeNode> createCircle(const Vec2& center, float radius, 
                                       const Color& color, int segments = 32, 
                                       float width = 1.0f);
    static Ptr<ShapeNode> createFilledCircle(const Vec2& center, float radius, 
                                             const Color& color, int segments = 32);
    
    // 三角形
    static Ptr<ShapeNode> createTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3,
                                         const Color& color, float width = 1.0f);
    static Ptr<ShapeNode> createFilledTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3,
                                               const Color& color);
    
    // 多边形
    static Ptr<ShapeNode> createPolygon(const std::vector<Vec2>& points,
                                        const Color& color, float width = 1.0f);
    static Ptr<ShapeNode> createFilledPolygon(const std::vector<Vec2>& points,
                                              const Color& color);

    // ------------------------------------------------------------------------
    // 属性设置
    // ------------------------------------------------------------------------
    void setShapeType(ShapeType type) { shapeType_ = type; }
    ShapeType getShapeType() const { return shapeType_; }
    
    void setColor(const Color& color) { color_ = color; }
    Color getColor() const { return color_; }
    
    void setFilled(bool filled) { filled_ = filled; }
    bool isFilled() const { return filled_; }
    
    void setLineWidth(float width) { lineWidth_ = width; }
    float getLineWidth() const { return lineWidth_; }
    
    void setSegments(int segments) { segments_ = segments; }
    int getSegments() const { return segments_; }

    // ------------------------------------------------------------------------
    // 点设置
    // ------------------------------------------------------------------------
    void setPoints(const std::vector<Vec2>& points);
    const std::vector<Vec2>& getPoints() const { return points_; }
    void addPoint(const Vec2& point);
    void clearPoints();

    Rect getBoundingBox() const override;

protected:
    void onDraw(RenderBackend& renderer) override;
    void generateRenderCommand(std::vector<RenderCommand>& commands, int zOrder) override;

private:
    ShapeType shapeType_ = ShapeType::Rect;
    Color color_ = Colors::White;
    bool filled_ = false;
    float lineWidth_ = 1.0f;
    int segments_ = 32;
    std::vector<Vec2> points_;
};

} // namespace easy2d
