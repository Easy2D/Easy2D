#include <easy2d/spatial/spatial_hash.h>
#include <easy2d/scene/node.h>
#include <cstdint>

namespace easy2d {

SpatialHash::SpatialHash(float cellSize) : cellSize_(cellSize) {}

SpatialHash::CellKey SpatialHash::getCellKey(float x, float y) const {
    int64_t cellX = static_cast<int64_t>(std::floor(x / cellSize_));
    int64_t cellY = static_cast<int64_t>(std::floor(y / cellSize_));
    return {cellX, cellY};
}

void SpatialHash::getCellsForRect(const Rect& rect, std::vector<CellKey>& cells) const {
    cells.clear();
    
    CellKey minCell = getCellKey(rect.origin.x, rect.origin.y);
    CellKey maxCell = getCellKey(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
    
    for (int64_t x = minCell.first; x <= maxCell.first; ++x) {
        for (int64_t y = minCell.second; y <= maxCell.second; ++y) {
            cells.emplace_back(x, y);
        }
    }
}

void SpatialHash::insertIntoCells(Node* node, const Rect& bounds) {
    std::vector<CellKey> cells;
    getCellsForRect(bounds, cells);
    
    for (const auto& cell : cells) {
        grid_[cell].insert(node);
    }
}

void SpatialHash::removeFromCells(Node* node, const Rect& bounds) {
    std::vector<CellKey> cells;
    getCellsForRect(bounds, cells);
    
    for (const auto& cell : cells) {
        auto it = grid_.find(cell);
        if (it != grid_.end()) {
            it->second.erase(node);
            if (it->second.empty()) {
                grid_.erase(it);
            }
        }
    }
}

void SpatialHash::insert(Node* node, const Rect& bounds) {
    if (!node) return;
    
    insertIntoCells(node, bounds);
    objectBounds_[node] = bounds;
    objectCount_++;
}

void SpatialHash::remove(Node* node) {
    if (!node) return;
    
    auto it = objectBounds_.find(node);
    if (it != objectBounds_.end()) {
        removeFromCells(node, it->second);
        objectBounds_.erase(it);
        objectCount_--;
    }
}

void SpatialHash::update(Node* node, const Rect& newBounds) {
    auto it = objectBounds_.find(node);
    if (it != objectBounds_.end()) {
        removeFromCells(node, it->second);
        insertIntoCells(node, newBounds);
        it->second = newBounds;
    }
}

std::vector<Node*> SpatialHash::query(const Rect& area) const {
    std::vector<Node*> results;
    std::unordered_set<Node*> found;
    
    std::vector<CellKey> cells;
    getCellsForRect(area, cells);
    
    for (const auto& cell : cells) {
        auto it = grid_.find(cell);
        if (it != grid_.end()) {
            for (Node* node : it->second) {
                if (found.insert(node).second) {
                    auto boundsIt = objectBounds_.find(node);
                    if (boundsIt != objectBounds_.end() && boundsIt->second.intersects(area)) {
                        results.push_back(node);
                    }
                }
            }
        }
    }
    
    return results;
}

std::vector<Node*> SpatialHash::query(const Vec2& point) const {
    std::vector<Node*> results;
    
    CellKey cell = getCellKey(point.x, point.y);
    auto it = grid_.find(cell);
    
    if (it != grid_.end()) {
        for (Node* node : it->second) {
            auto boundsIt = objectBounds_.find(node);
            if (boundsIt != objectBounds_.end() && boundsIt->second.containsPoint(point)) {
                results.push_back(node);
            }
        }
    }
    
    return results;
}

std::vector<std::pair<Node*, Node*>> SpatialHash::queryCollisions() const {
    std::vector<std::pair<Node*, Node*>> collisions;
    struct PairHash {
        size_t operator()(const std::pair<Node*, Node*>& p) const noexcept {
            auto a = reinterpret_cast<std::uintptr_t>(p.first);
            auto b = reinterpret_cast<std::uintptr_t>(p.second);
            return std::hash<std::uintptr_t>{}(a) ^ (std::hash<std::uintptr_t>{}(b) << 1);
        }
    };
    auto makeOrdered = [](Node* a, Node* b) -> std::pair<Node*, Node*> {
        return a < b ? std::make_pair(a, b) : std::make_pair(b, a);
    };

    std::unordered_set<std::pair<Node*, Node*>, PairHash> seen;
    seen.reserve(objectCount_ * 2);
    
    for (const auto& [cell, objects] : grid_) {
        std::vector<Node*> cellObjects(objects.begin(), objects.end());
        
        for (size_t i = 0; i < cellObjects.size(); ++i) {
            auto bounds1 = objectBounds_.find(cellObjects[i]);
            if (bounds1 == objectBounds_.end()) continue;
            
            for (size_t j = i + 1; j < cellObjects.size(); ++j) {
                auto bounds2 = objectBounds_.find(cellObjects[j]);
                if (bounds2 == objectBounds_.end()) continue;
                
                if (bounds1->second.intersects(bounds2->second)) {
                    auto key = makeOrdered(cellObjects[i], cellObjects[j]);
                    if (seen.insert(key).second) {
                        collisions.emplace_back(key.first, key.second);
                    }
                }
            }
        }
    }
    
    return collisions;
}

void SpatialHash::clear() {
    grid_.clear();
    objectBounds_.clear();
    objectCount_ = 0;
}

size_t SpatialHash::size() const {
    return objectCount_;
}

bool SpatialHash::empty() const {
    return objectCount_ == 0;
}

void SpatialHash::rebuild() {
    auto bounds = objectBounds_;
    clear();
    
    for (const auto& [node, bound] : bounds) {
        insert(node, bound);
    }
}

void SpatialHash::setCellSize(float cellSize) {
    if (cellSize != cellSize_ && cellSize > 0) {
        cellSize_ = cellSize;
        rebuild();
    }
}

}
