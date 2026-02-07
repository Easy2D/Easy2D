#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>
#include <vector>
#include <memory>

namespace easy2d {

class Node;

enum class SpatialStrategy {
    Auto,
    QuadTree,
    SpatialHash
};

struct SpatialQueryResult {
    Node* node;
    Rect bounds;
};

class ISpatialIndex {
public:
    virtual ~ISpatialIndex() = default;

    virtual void insert(Node* node, const Rect& bounds) = 0;
    virtual void remove(Node* node) = 0;
    virtual void update(Node* node, const Rect& newBounds) = 0;

    virtual std::vector<Node*> query(const Rect& area) const = 0;
    virtual std::vector<Node*> query(const Vec2& point) const = 0;
    virtual std::vector<std::pair<Node*, Node*>> queryCollisions() const = 0;

    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;

    virtual void rebuild() = 0;
};

using SpatialIndexPtr = std::unique_ptr<ISpatialIndex>;

}
