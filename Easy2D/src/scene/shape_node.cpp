#include <easy2d/scene/shape_node.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/render_command.h>
#include <algorithm>
#include <cmath>
#include <limits>

namespace easy2d {

ShapeNode::ShapeNode() = default;

Ptr<ShapeNode> ShapeNode::create() {
    return makePtr<ShapeNode>();
}

Ptr<ShapeNode> ShapeNode::createPoint(const Vec2& pos, const Color& color) {
    auto node = makePtr<ShapeNode>();
    node->shapeType_ = ShapeType::Point;
    node->color_ = color;
    node->points_ = {pos};
    return node;
}

Ptr<ShapeNode> ShapeNode::createLine(const Vec2& start, const Vec2& end, 
                                     const Color& color, float width) {
    auto node = makePtr<ShapeNode>();
    node->shapeType_ = ShapeType::Line;
    node->color_ = color;
    node->lineWidth_ = width;
    node->points_ = {start, end};
    return node;
}

Ptr<ShapeNode> ShapeNode::createRect(const Rect& rect, const Color& color, float width) {
    auto node = makePtr<ShapeNode>();
    node->shapeType_ = ShapeType::Rect;
    node->color_ = color;
    node->lineWidth_ = width;
    node->filled_ = false;
    node->points_ = {
        Vec2(rect.left(), rect.top()),
        Vec2(rect.right(), rect.top()),
        Vec2(rect.right(), rect.bottom()),
        Vec2(rect.left(), rect.bottom())
    };
    return node;
}

Ptr<ShapeNode> ShapeNode::createFilledRect(const Rect& rect, const Color& color) {
    auto node = createRect(rect, color, 0);
    node->filled_ = true;
    return node;
}

Ptr<ShapeNode> ShapeNode::createCircle(const Vec2& center, float radius, 
                                       const Color& color, int segments, float width) {
    auto node = makePtr<ShapeNode>();
    node->shapeType_ = ShapeType::Circle;
    node->color_ = color;
    node->lineWidth_ = width;
    node->segments_ = segments;
    node->filled_ = false;
    node->points_ = {center};
    // Store radius in a point for simplicity
    node->addPoint(Vec2(radius, 0));
    return node;
}

Ptr<ShapeNode> ShapeNode::createFilledCircle(const Vec2& center, float radius, 
                                             const Color& color, int segments) {
    auto node = createCircle(center, radius, color, segments, 0);
    node->filled_ = true;
    return node;
}

Ptr<ShapeNode> ShapeNode::createTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3,
                                         const Color& color, float width) {
    auto node = makePtr<ShapeNode>();
    node->shapeType_ = ShapeType::Triangle;
    node->color_ = color;
    node->lineWidth_ = width;
    node->filled_ = false;
    node->points_ = {p1, p2, p3};
    return node;
}

Ptr<ShapeNode> ShapeNode::createFilledTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3,
                                               const Color& color) {
    auto node = createTriangle(p1, p2, p3, color, 0);
    node->filled_ = true;
    return node;
}

Ptr<ShapeNode> ShapeNode::createPolygon(const std::vector<Vec2>& points,
                                        const Color& color, float width) {
    auto node = makePtr<ShapeNode>();
    node->shapeType_ = ShapeType::Polygon;
    node->color_ = color;
    node->lineWidth_ = width;
    node->filled_ = false;
    node->points_ = points;
    return node;
}

Ptr<ShapeNode> ShapeNode::createFilledPolygon(const std::vector<Vec2>& points,
                                              const Color& color) {
    auto node = createPolygon(points, color, 0);
    node->filled_ = true;
    return node;
}

void ShapeNode::setPoints(const std::vector<Vec2>& points) {
    points_ = points;
    updateSpatialIndex();
}

void ShapeNode::addPoint(const Vec2& point) {
    points_.push_back(point);
    updateSpatialIndex();
}

void ShapeNode::clearPoints() {
    points_.clear();
    updateSpatialIndex();
}

Rect ShapeNode::getBoundingBox() const {
    if (points_.empty()) {
        return Rect();
    }

    Vec2 offset = getPosition();

    if (shapeType_ == ShapeType::Circle && points_.size() >= 2) {
        float radius = std::abs(points_[1].x);
        Vec2 center = points_[0] + offset;
        return Rect(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);
    }

    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for (const auto& p : points_) {
        Vec2 world = p + offset;
        minX = std::min(minX, world.x);
        minY = std::min(minY, world.y);
        maxX = std::max(maxX, world.x);
        maxY = std::max(maxY, world.y);
    }

    float inflate = 0.0f;
    if (!filled_ && (shapeType_ == ShapeType::Line || shapeType_ == ShapeType::Rect || shapeType_ == ShapeType::Triangle || shapeType_ == ShapeType::Polygon || shapeType_ == ShapeType::Point)) {
        inflate = std::max(0.0f, lineWidth_ * 0.5f);
    }
    if (shapeType_ == ShapeType::Point) {
        inflate = std::max(inflate, lineWidth_ * 0.5f);
    }

    return Rect(minX - inflate, minY - inflate, (maxX - minX) + inflate * 2.0f, (maxY - minY) + inflate * 2.0f);
}

void ShapeNode::onDraw(RenderBackend& renderer) {
    if (points_.empty()) {
        return;
    }
    
    Vec2 offset = getPosition();
    
    switch (shapeType_) {
        case ShapeType::Point:
            if (!points_.empty()) {
                renderer.fillCircle(points_[0] + offset, lineWidth_ * 0.5f, color_, 8);
            }
            break;
            
        case ShapeType::Line:
            if (points_.size() >= 2) {
                renderer.drawLine(points_[0] + offset, points_[1] + offset, color_, lineWidth_);
            }
            break;
            
        case ShapeType::Rect:
            if (points_.size() >= 4) {
                if (filled_) {
                    Rect rect(points_[0].x, points_[0].y, 
                             points_[2].x - points_[0].x, 
                             points_[2].y - points_[0].y);
                    renderer.fillRect(Rect(rect.origin + offset, rect.size), color_);
                } else {
                    for (size_t i = 0; i < points_.size(); ++i) {
                        Vec2 start = points_[i] + offset;
                        Vec2 end = points_[(i + 1) % points_.size()] + offset;
                        renderer.drawLine(start, end, color_, lineWidth_);
                    }
                }
            }
            break;
            
        case ShapeType::Circle:
            if (points_.size() >= 2) {
                float radius = points_[1].x;
                if (filled_) {
                    renderer.fillCircle(points_[0] + offset, radius, color_, segments_);
                } else {
                    renderer.drawCircle(points_[0] + offset, radius, color_, segments_, lineWidth_);
                }
            }
            break;
            
        case ShapeType::Triangle:
            if (points_.size() >= 3) {
                Vec2 p1 = points_[0] + offset;
                Vec2 p2 = points_[1] + offset;
                Vec2 p3 = points_[2] + offset;
                if (filled_) {
                    renderer.fillTriangle(p1, p2, p3, color_);
                } else {
                    renderer.drawLine(p1, p2, color_, lineWidth_);
                    renderer.drawLine(p2, p3, color_, lineWidth_);
                    renderer.drawLine(p3, p1, color_, lineWidth_);
                }
            }
            break;
            
        case ShapeType::Polygon:
            if (!points_.empty()) {
                std::vector<Vec2> transformedPoints;
                transformedPoints.reserve(points_.size());
                for (const auto& p : points_) {
                    transformedPoints.push_back(p + offset);
                }
                
                if (filled_) {
                    renderer.fillPolygon(transformedPoints, color_);
                } else {
                    renderer.drawPolygon(transformedPoints, color_, lineWidth_);
                }
            }
            break;
    }
}

void ShapeNode::generateRenderCommand(std::vector<RenderCommand>& commands, int zOrder) {
    if (points_.empty()) {
        return;
    }

    Vec2 offset = getPosition();
    RenderCommand cmd;
    cmd.zOrder = zOrder;

    switch (shapeType_) {
        case ShapeType::Point:
            if (!points_.empty()) {
                cmd.type = RenderCommandType::FilledCircle;
                cmd.data = CircleData{
                    points_[0] + offset,
                    lineWidth_ * 0.5f,
                    color_,
                    8,
                    0.0f
                };
            }
            break;

        case ShapeType::Line:
            if (points_.size() >= 2) {
                cmd.type = RenderCommandType::Line;
                cmd.data = LineData{
                    points_[0] + offset,
                    points_[1] + offset,
                    color_,
                    lineWidth_
                };
            }
            break;

        case ShapeType::Rect:
            if (points_.size() >= 4) {
                if (filled_) {
                    cmd.type = RenderCommandType::FilledRect;
                    Rect rect(points_[0].x, points_[0].y,
                             points_[2].x - points_[0].x,
                             points_[2].y - points_[0].y);
                    cmd.data = RectData{
                        Rect(rect.origin + offset, rect.size),
                        color_,
                        0.0f
                    };
                } else {
                    cmd.type = RenderCommandType::Rect;
                    Rect rect(points_[0].x, points_[0].y,
                             points_[2].x - points_[0].x,
                             points_[2].y - points_[0].y);
                    cmd.data = RectData{
                        Rect(rect.origin + offset, rect.size),
                        color_,
                        lineWidth_
                    };
                }
            }
            break;

        case ShapeType::Circle:
            if (points_.size() >= 2) {
                float radius = points_[1].x;
                if (filled_) {
                    cmd.type = RenderCommandType::FilledCircle;
                    cmd.data = CircleData{
                        points_[0] + offset,
                        radius,
                        color_,
                        segments_,
                        0.0f
                    };
                } else {
                    cmd.type = RenderCommandType::Circle;
                    cmd.data = CircleData{
                        points_[0] + offset,
                        radius,
                        color_,
                        segments_,
                        lineWidth_
                    };
                }
            }
            break;

        case ShapeType::Triangle:
            if (points_.size() >= 3) {
                Vec2 p1 = points_[0] + offset;
                Vec2 p2 = points_[1] + offset;
                Vec2 p3 = points_[2] + offset;
                if (filled_) {
                    cmd.type = RenderCommandType::FilledTriangle;
                    cmd.data = TriangleData{
                        p1, p2, p3,
                        color_,
                        0.0f
                    };
                } else {
                    cmd.type = RenderCommandType::Triangle;
                    cmd.data = TriangleData{
                        p1, p2, p3,
                        color_,
                        lineWidth_
                    };
                }
            }
            break;

        case ShapeType::Polygon:
            if (!points_.empty()) {
                std::vector<Vec2> transformedPoints;
                transformedPoints.reserve(points_.size());
                for (const auto& p : points_) {
                    transformedPoints.push_back(p + offset);
                }

                if (filled_) {
                    cmd.type = RenderCommandType::FilledPolygon;
                    cmd.data = PolygonData{
                        transformedPoints,
                        color_,
                        0.0f
                    };
                } else {
                    cmd.type = RenderCommandType::Polygon;
                    cmd.data = PolygonData{
                        transformedPoints,
                        color_,
                        lineWidth_
                    };
                }
            }
            break;
    }

    commands.push_back(std::move(cmd));
}

} // namespace easy2d
