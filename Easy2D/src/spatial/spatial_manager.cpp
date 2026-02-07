#include <easy2d/spatial/spatial_manager.h>
#include <easy2d/spatial/quadtree.h>
#include <easy2d/spatial/spatial_hash.h>
#include <easy2d/scene/node.h>
#include <chrono>

namespace easy2d {

SpatialManager::SpatialManager() 
    : worldBounds_(0, 0, 10000, 10000) {
    selectOptimalStrategy();
}

SpatialManager::SpatialManager(const Rect& worldBounds)
    : worldBounds_(worldBounds) {
    selectOptimalStrategy();
}

void SpatialManager::setStrategy(SpatialStrategy strategy) {
    if (currentStrategy_ == strategy) return;
    
    currentStrategy_ = strategy;
    rebuild();
}

void SpatialManager::setAutoThresholds(size_t quadTreeThreshold, size_t hashThreshold) {
    quadTreeThreshold_ = quadTreeThreshold;
    hashThreshold_ = hashThreshold;
    
    if (currentStrategy_ == SpatialStrategy::Auto) {
        selectOptimalStrategy();
    }
}

void SpatialManager::setWorldBounds(const Rect& bounds) {
    worldBounds_ = bounds;
    rebuild();
}

void SpatialManager::insert(Node* node, const Rect& bounds) {
    if (!index_) {
        selectOptimalStrategy();
    }
    
    if (index_) {
        index_->insert(node, bounds);
    }
}

void SpatialManager::remove(Node* node) {
    if (index_) {
        index_->remove(node);
    }
}

void SpatialManager::update(Node* node, const Rect& newBounds) {
    if (index_) {
        index_->update(node, newBounds);
    }
}

std::vector<Node*> SpatialManager::query(const Rect& area) const {
    if (!index_) return {};
    
    auto start = std::chrono::high_resolution_clock::now();
    auto results = index_->query(area);
    auto end = std::chrono::high_resolution_clock::now();
    
    queryCount_++;
    totalQueryTime_ += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    return results;
}

std::vector<Node*> SpatialManager::query(const Vec2& point) const {
    if (!index_) return {};
    return index_->query(point);
}

std::vector<std::pair<Node*, Node*>> SpatialManager::queryCollisions() const {
    if (!index_) return {};
    return index_->queryCollisions();
}

void SpatialManager::query(const Rect& area, const QueryCallback& callback) const {
    auto results = query(area);
    for (Node* node : results) {
        if (!callback(node)) break;
    }
}

void SpatialManager::query(const Vec2& point, const QueryCallback& callback) const {
    auto results = query(point);
    for (Node* node : results) {
        if (!callback(node)) break;
    }
}

void SpatialManager::clear() {
    if (index_) {
        index_->clear();
    }
}

size_t SpatialManager::size() const {
    return index_ ? index_->size() : 0;
}

bool SpatialManager::empty() const {
    return index_ ? index_->empty() : true;
}

void SpatialManager::rebuild() {
    if (!index_) {
        selectOptimalStrategy();
        return;
    }
    
    auto oldIndex = std::move(index_);
    selectOptimalStrategy();
    
    if (index_ && oldIndex) {
        auto bounds = Rect(worldBounds_);
        auto objects = oldIndex->query(bounds);
        for (Node* node : objects) {
            if (node) {
                auto nodeBounds = node->getBoundingBox();
                index_->insert(node, nodeBounds);
            }
        }
    }
}

void SpatialManager::optimize() {
    if (currentStrategy_ == SpatialStrategy::Auto) {
        selectOptimalStrategy();
    }
    
    if (index_) {
        index_->rebuild();
    }
}

SpatialStrategy SpatialManager::getCurrentStrategy() const {
    return activeStrategy_;
}

const char* SpatialManager::getStrategyName() const {
    switch (activeStrategy_) {
        case SpatialStrategy::QuadTree: return "QuadTree";
        case SpatialStrategy::SpatialHash: return "SpatialHash";
        default: return "Unknown";
    }
}

std::unique_ptr<ISpatialIndex> SpatialManager::createIndex(SpatialStrategy strategy, const Rect& bounds) {
    switch (strategy) {
        case SpatialStrategy::QuadTree:
            return std::make_unique<QuadTree>(bounds);
        case SpatialStrategy::SpatialHash:
            return std::make_unique<SpatialHash>(64.0f);
        default:
            return std::make_unique<QuadTree>(bounds);
    }
}

void SpatialManager::selectOptimalStrategy() {
    if (currentStrategy_ != SpatialStrategy::Auto) {
        activeStrategy_ = currentStrategy_;
    } else {
        size_t currentSize = index_ ? index_->size() : 0;
        
        if (currentSize < quadTreeThreshold_) {
            activeStrategy_ = SpatialStrategy::QuadTree;
        } else if (currentSize > hashThreshold_) {
            activeStrategy_ = SpatialStrategy::SpatialHash;
        } else {
            // Keep current strategy in transition zone
            if (!index_) {
                activeStrategy_ = SpatialStrategy::QuadTree;
            }
        }
    }
    
    index_ = createIndex(activeStrategy_, worldBounds_);
}

}
