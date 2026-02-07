#pragma once

#include <easy2d/spatial/spatial_index.h>
#include <unordered_map>
#include <unordered_set>

namespace easy2d {

class SpatialHash : public ISpatialIndex {
public:
    using CellKey = std::pair<int64_t, int64_t>;
    
    struct CellKeyHash {
        size_t operator()(const CellKey& key) const {
            return std::hash<int64_t>()(key.first) ^ (std::hash<int64_t>()(key.second) << 1);
        }
    };

    explicit SpatialHash(float cellSize = 64.0f);
    ~SpatialHash() override = default;

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

    void setCellSize(float cellSize);
    float getCellSize() const { return cellSize_; }

private:
    CellKey getCellKey(float x, float y) const;
    void getCellsForRect(const Rect& rect, std::vector<CellKey>& cells) const;
    void insertIntoCells(Node* node, const Rect& bounds);
    void removeFromCells(Node* node, const Rect& bounds);

    float cellSize_;
    std::unordered_map<CellKey, std::unordered_set<Node*>, CellKeyHash> grid_;
    std::unordered_map<Node*, Rect> objectBounds_;
    size_t objectCount_ = 0;
};

}
