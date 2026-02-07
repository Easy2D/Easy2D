#pragma once

#include <easy2d/spatial/spatial_index.h>
#include <array>

namespace easy2d {

class QuadTree : public ISpatialIndex {
public:
    static constexpr int MAX_OBJECTS = 10;
    static constexpr int MAX_LEVELS = 5;

    struct QuadTreeNode {
        Rect bounds;
        int level;
        std::vector<std::pair<Node*, Rect>> objects;
        std::array<std::unique_ptr<QuadTreeNode>, 4> children;

        QuadTreeNode(const Rect& bounds, int level);
        bool contains(const Rect& rect) const;
        bool intersects(const Rect& rect) const;
    };

    explicit QuadTree(const Rect& worldBounds);
    ~QuadTree() override = default;

    void insert(Node* node, const Rect& bounds) override;
    void remove(Node* node) override;
    void update(Node* node, const Rect& newBounds) override;

    std::vector<Node*> query(const Rect& area) const override;
    std::vector<Node*> query(const Vec2& point) const override;
    std::vector<std::pair<Node*, Node*>> queryCollisions() const override;

    void clear() override;
    size_t size() const override;
    bool empty() const override;

    void rebuild() override;

private:
    void split(QuadTreeNode* node);
    void insertIntoNode(QuadTreeNode* node, Node* object, const Rect& bounds);
    void queryNode(const QuadTreeNode* node, const Rect& area, std::vector<Node*>& results) const;
    void queryNode(const QuadTreeNode* node, const Vec2& point, std::vector<Node*>& results) const;
    void collectCollisions(const QuadTreeNode* node, std::vector<std::pair<Node*, Node*>>& collisions) const;
    bool removeFromNode(QuadTreeNode* node, Node* object);

    std::unique_ptr<QuadTreeNode> root_;
    Rect worldBounds_;
    size_t objectCount_ = 0;
};

}
