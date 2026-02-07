#pragma once

#include <easy2d/spatial/spatial_index.h>
#include <memory>
#include <functional>

namespace easy2d {

class SpatialManager {
public:
    using QueryCallback = std::function<bool(Node*)>;

    SpatialManager();
    explicit SpatialManager(const Rect& worldBounds);
    ~SpatialManager() = default;

    void setStrategy(SpatialStrategy strategy);
    void setAutoThresholds(size_t quadTreeThreshold, size_t hashThreshold);
    
    void setWorldBounds(const Rect& bounds);
    Rect getWorldBounds() const { return worldBounds_; }

    void insert(Node* node, const Rect& bounds);
    void remove(Node* node);
    void update(Node* node, const Rect& newBounds);

    std::vector<Node*> query(const Rect& area) const;
    std::vector<Node*> query(const Vec2& point) const;
    std::vector<std::pair<Node*, Node*>> queryCollisions() const;

    void query(const Rect& area, const QueryCallback& callback) const;
    void query(const Vec2& point, const QueryCallback& callback) const;

    void clear();
    size_t size() const;
    bool empty() const;

    void rebuild();
    void optimize();

    SpatialStrategy getCurrentStrategy() const;
    const char* getStrategyName() const;

    static std::unique_ptr<ISpatialIndex> createIndex(SpatialStrategy strategy, const Rect& bounds);

private:
    void selectOptimalStrategy();

    SpatialStrategy currentStrategy_ = SpatialStrategy::Auto;
    SpatialStrategy activeStrategy_ = SpatialStrategy::QuadTree;
    std::unique_ptr<ISpatialIndex> index_;
    Rect worldBounds_;
    
    size_t quadTreeThreshold_ = 1000;
    size_t hashThreshold_ = 5000;
    
    mutable size_t queryCount_ = 0;
    mutable size_t totalQueryTime_ = 0;
};

}
