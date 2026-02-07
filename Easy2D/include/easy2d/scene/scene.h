#pragma once

#include <easy2d/scene/node.h>
#include <easy2d/core/color.h>
#include <easy2d/graphics/camera.h>
#include <easy2d/spatial/spatial_manager.h>
#include <vector>

namespace easy2d {

// 前向声明
struct RenderCommand;

// ============================================================================
// 场景类 - 节点容器，管理整个场景图
// ============================================================================
class Scene : public Node {
public:
    Scene();
    ~Scene() override = default;

    // ------------------------------------------------------------------------
    // 场景属性
    // ------------------------------------------------------------------------
    void setBackgroundColor(const Color& color) { backgroundColor_ = color; }
    Color getBackgroundColor() const { return backgroundColor_; }
    
    // ------------------------------------------------------------------------
    // 摄像机
    // ------------------------------------------------------------------------
    void setCamera(Ptr<Camera> camera);
    Ptr<Camera> getCamera() const { return camera_; }
    
    Camera* getActiveCamera() const { 
        return camera_ ? camera_.get() : defaultCamera_.get(); 
    }

    // ------------------------------------------------------------------------
    // 视口和尺寸
    // ------------------------------------------------------------------------
    void setViewportSize(float width, float height);
    void setViewportSize(const Size& size);
    Size getViewportSize() const { return viewportSize_; }
    
    float getWidth() const { return viewportSize_.width; }
    float getHeight() const { return viewportSize_.height; }

    // ------------------------------------------------------------------------
    // 场景状态
    // ------------------------------------------------------------------------
    bool isPaused() const { return paused_; }
    void pause() { paused_ = true; }
    void resume() { paused_ = false; }

    // ------------------------------------------------------------------------
    // 渲染和更新
    // ------------------------------------------------------------------------
    void renderScene(RenderBackend& renderer);
    void renderContent(RenderBackend& renderer);
    void updateScene(float dt);
    void collectRenderCommands(std::vector<RenderCommand>& commands);

    // ------------------------------------------------------------------------
    // 空间索引系统
    // ------------------------------------------------------------------------
    SpatialManager& getSpatialManager() { return spatialManager_; }
    const SpatialManager& getSpatialManager() const { return spatialManager_; }
    
    // 启用/禁用空间索引
    void setSpatialIndexingEnabled(bool enabled) { spatialIndexingEnabled_ = enabled; }
    bool isSpatialIndexingEnabled() const { return spatialIndexingEnabled_; }
    
    // 节点空间索引管理（内部使用）
    void updateNodeInSpatialIndex(Node* node, const Rect& oldBounds, const Rect& newBounds);
    void removeNodeFromSpatialIndex(Node* node);
    
    // 碰撞检测查询
    std::vector<Node*> queryNodesInArea(const Rect& area) const;
    std::vector<Node*> queryNodesAtPoint(const Vec2& point) const;
    std::vector<std::pair<Node*, Node*>> queryCollisions() const;

    // ------------------------------------------------------------------------
    // 静态创建方法
    // ------------------------------------------------------------------------
    static Ptr<Scene> create();

protected:
    void onEnter() override;
    void onExit() override;

    friend class SceneManager;

private:
    Color backgroundColor_ = Colors::Black;
    Size viewportSize_ = Size::Zero();
    
    Ptr<Camera> camera_;
    Ptr<Camera> defaultCamera_;
    
    bool paused_ = false;
    
    // 空间索引系统
    SpatialManager spatialManager_;
    bool spatialIndexingEnabled_ = true;
};

} // namespace easy2d
