#include <easy2d/spatial/quadtree.h>
#include <easy2d/scene/node.h>
#include <algorithm>
#include <functional>

namespace easy2d {

QuadTree::QuadTreeNode::QuadTreeNode(const Rect& bounds, int level)
    : bounds(bounds), level(level) {}

bool QuadTree::QuadTreeNode::contains(const Rect& rect) const {
    return bounds.contains(rect);
}

bool QuadTree::QuadTreeNode::intersects(const Rect& rect) const {
    return bounds.intersects(rect);
}

QuadTree::QuadTree(const Rect& worldBounds)
    : worldBounds_(worldBounds) {
    root_ = std::make_unique<QuadTreeNode>(worldBounds, 0);
}

void QuadTree::insert(Node* node, const Rect& bounds) {
    if (!node || !root_->intersects(bounds)) return;
    insertIntoNode(root_.get(), node, bounds);
    objectCount_++;
}

void QuadTree::insertIntoNode(QuadTreeNode* node, Node* object, const Rect& bounds) {
    if (node->children[0]) {
        int index = -1;
        float midX = node->bounds.origin.x + node->bounds.size.width / 2.0f;
        float midY = node->bounds.origin.y + node->bounds.size.height / 2.0f;

        bool top = bounds.origin.y + bounds.size.height <= midY;
        bool bottom = bounds.origin.y >= midY;
        bool left = bounds.origin.x + bounds.size.width <= midX;
        bool right = bounds.origin.x >= midX;

        if (top && left) index = 0;
        else if (top && right) index = 1;
        else if (bottom && left) index = 2;
        else if (bottom && right) index = 3;

        if (index != -1) {
            insertIntoNode(node->children[index].get(), object, bounds);
            return;
        }
    }

    node->objects.emplace_back(object, bounds);

    if (node->objects.size() > MAX_OBJECTS && node->level < MAX_LEVELS) {
        if (!node->children[0]) {
            split(node);
        }
    }
}

void QuadTree::split(QuadTreeNode* node) {
    float midX = node->bounds.origin.x + node->bounds.size.width / 2.0f;
    float midY = node->bounds.origin.y + node->bounds.size.height / 2.0f;

    node->children[0] = std::make_unique<QuadTreeNode>(
        Rect(node->bounds.origin.x, node->bounds.origin.y, node->bounds.size.width / 2.0f, node->bounds.size.height / 2.0f),
        node->level + 1);
    node->children[1] = std::make_unique<QuadTreeNode>(
        Rect(midX, node->bounds.origin.y, node->bounds.size.width / 2.0f, node->bounds.size.height / 2.0f),
        node->level + 1);
    node->children[2] = std::make_unique<QuadTreeNode>(
        Rect(node->bounds.origin.x, midY, node->bounds.size.width / 2.0f, node->bounds.size.height / 2.0f),
        node->level + 1);
    node->children[3] = std::make_unique<QuadTreeNode>(
        Rect(midX, midY, node->bounds.size.width / 2.0f, node->bounds.size.height / 2.0f),
        node->level + 1);

    auto objects = std::move(node->objects);
    node->objects.clear();

    for (const auto& [obj, bounds] : objects) {
        insertIntoNode(node, obj, bounds);
    }
}

void QuadTree::remove(Node* node) {
    if (!node) return;
    if (removeFromNode(root_.get(), node)) {
        objectCount_--;
    }
}

bool QuadTree::removeFromNode(QuadTreeNode* node, Node* object) {
    auto it = std::find_if(node->objects.begin(), node->objects.end(),
        [object](const auto& pair) { return pair.first == object; });
    
    if (it != node->objects.end()) {
        node->objects.erase(it);
        return true;
    }

    if (node->children[0]) {
        for (auto& child : node->children) {
            if (removeFromNode(child.get(), object)) {
                return true;
            }
        }
    }

    return false;
}

void QuadTree::update(Node* node, const Rect& newBounds) {
    remove(node);
    insert(node, newBounds);
}

std::vector<Node*> QuadTree::query(const Rect& area) const {
    std::vector<Node*> results;
    queryNode(root_.get(), area, results);
    return results;
}

void QuadTree::queryNode(const QuadTreeNode* node, const Rect& area, std::vector<Node*>& results) const {
    if (!node || !node->intersects(area)) return;

    for (const auto& [obj, bounds] : node->objects) {
        if (bounds.intersects(area)) {
            results.push_back(obj);
        }
    }

    if (node->children[0]) {
        for (const auto& child : node->children) {
            queryNode(child.get(), area, results);
        }
    }
}

std::vector<Node*> QuadTree::query(const Vec2& point) const {
    std::vector<Node*> results;
    queryNode(root_.get(), point, results);
    return results;
}

void QuadTree::queryNode(const QuadTreeNode* node, const Vec2& point, std::vector<Node*>& results) const {
    if (!node || !node->bounds.containsPoint(point)) return;

    for (const auto& [obj, bounds] : node->objects) {
        if (bounds.containsPoint(point)) {
            results.push_back(obj);
        }
    }

    if (node->children[0]) {
        for (const auto& child : node->children) {
            queryNode(child.get(), point, results);
        }
    }
}

std::vector<std::pair<Node*, Node*>> QuadTree::queryCollisions() const {
    std::vector<std::pair<Node*, Node*>> collisions;
    collectCollisions(root_.get(), collisions);
    return collisions;
}

void QuadTree::collectCollisions(const QuadTreeNode* node, std::vector<std::pair<Node*, Node*>>& collisions) const {
    if (!node) return;

    std::vector<std::pair<Node*, Rect>> ancestors;
    ancestors.reserve(objectCount_);

    std::function<void(const QuadTreeNode*)> visit = [&](const QuadTreeNode* current) {
        if (!current) return;

        for (const auto& [obj, bounds] : current->objects) {
            for (const auto& [ancestorObj, ancestorBounds] : ancestors) {
                if (bounds.intersects(ancestorBounds)) {
                    collisions.emplace_back(ancestorObj, obj);
                }
            }
        }

        for (size_t i = 0; i < current->objects.size(); ++i) {
            for (size_t j = i + 1; j < current->objects.size(); ++j) {
                if (current->objects[i].second.intersects(current->objects[j].second)) {
                    collisions.emplace_back(current->objects[i].first, current->objects[j].first);
                }
            }
        }

        size_t oldSize = ancestors.size();
        ancestors.insert(ancestors.end(), current->objects.begin(), current->objects.end());

        if (current->children[0]) {
            for (const auto& child : current->children) {
                visit(child.get());
            }
        }

        ancestors.resize(oldSize);
    };

    visit(node);
}

void QuadTree::clear() {
    root_ = std::make_unique<QuadTreeNode>(worldBounds_, 0);
    objectCount_ = 0;
}

size_t QuadTree::size() const {
    return objectCount_;
}

bool QuadTree::empty() const {
    return objectCount_ == 0;
}

void QuadTree::rebuild() {
    std::vector<std::pair<Node*, Rect>> allObjects;
    
    std::function<void(QuadTreeNode*)> collect = [&](QuadTreeNode* node) {
        if (!node) return;
        for (const auto& obj : node->objects) {
            allObjects.push_back(obj);
        }
        if (node->children[0]) {
            for (auto& child : node->children) {
                collect(child.get());
            }
        }
    };
    
    collect(root_.get());
    clear();
    
    for (const auto& [obj, bounds] : allObjects) {
        insert(obj, bounds);
    }
}

}
