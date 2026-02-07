#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>
#include <easy2d/core/color.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/event/event_dispatcher.h>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

namespace easy2d {

// 前向声明
class Scene;
class Action;
class RenderBackend;
struct RenderCommand;

// ============================================================================
// 节点基类 - 场景图的基础
// ============================================================================
class Node : public std::enable_shared_from_this<Node> {
public:
    Node();
    virtual ~Node();

    // ------------------------------------------------------------------------
    // 层级管理
    // ------------------------------------------------------------------------
    void addChild(Ptr<Node> child);
    void removeChild(Ptr<Node> child);
    void removeChildByName(const std::string& name);
    void removeFromParent();
    void removeAllChildren();
    
    Ptr<Node> getParent() const { return parent_.lock(); }
    const std::vector<Ptr<Node>>& getChildren() const { return children_; }
    Ptr<Node> getChildByName(const std::string& name) const;
    Ptr<Node> getChildByTag(int tag) const;

    // ------------------------------------------------------------------------
    // 变换属性
    // ------------------------------------------------------------------------
    void setPosition(const Vec2& pos);
    void setPosition(float x, float y);
    Vec2 getPosition() const { return position_; }

    void setRotation(float degrees);
    float getRotation() const { return rotation_; }

    void setScale(const Vec2& scale);
    void setScale(float scale);
    void setScale(float x, float y);
    Vec2 getScale() const { return scale_; }

    void setAnchor(const Vec2& anchor);
    void setAnchor(float x, float y);
    Vec2 getAnchor() const { return anchor_; }

    void setSkew(const Vec2& skew);
    void setSkew(float x, float y);
    Vec2 getSkew() const { return skew_; }

    void setOpacity(float opacity);
    float getOpacity() const { return opacity_; }

    void setVisible(bool visible);
    bool isVisible() const { return visible_; }

    void setZOrder(int zOrder);
    int getZOrder() const { return zOrder_; }

    // ------------------------------------------------------------------------
    // 世界变换
    // ------------------------------------------------------------------------
    Vec2 convertToWorldSpace(const Vec2& localPos) const;
    Vec2 convertToNodeSpace(const Vec2& worldPos) const;
    
    glm::mat4 getLocalTransform() const;
    glm::mat4 getWorldTransform() const;

    // ------------------------------------------------------------------------
    // 名称和标签
    // ------------------------------------------------------------------------
    void setName(const std::string& name) { name_ = name; }
    const std::string& getName() const { return name_; }
    
    void setTag(int tag) { tag_ = tag; }
    int getTag() const { return tag_; }

    // ------------------------------------------------------------------------
    // 生命周期回调
    // ------------------------------------------------------------------------
    virtual void onEnter();
    virtual void onExit();
    virtual void onUpdate(float dt);
    virtual void onRender(RenderBackend& renderer);
    virtual void onAttachToScene(Scene* scene);
    virtual void onDetachFromScene();

    // ------------------------------------------------------------------------
    // 边界框（用于空间索引）
    // ------------------------------------------------------------------------
    virtual Rect getBoundingBox() const;
    
    // 是否需要参与空间索引（默认 true）
    void setSpatialIndexed(bool indexed) { spatialIndexed_ = indexed; }
    bool isSpatialIndexed() const { return spatialIndexed_; }
    
    // 更新空间索引（手动调用，通常在边界框变化后）
    void updateSpatialIndex();

    // ------------------------------------------------------------------------
    // 动作系统
    // ------------------------------------------------------------------------
    void runAction(Ptr<Action> action);
    void stopAllActions();
    void stopAction(Ptr<Action> action);
    void stopActionByTag(int tag);
    Ptr<Action> getActionByTag(int tag) const;
    size_t getActionCount() const { return actions_.size(); }

    // ------------------------------------------------------------------------
    // 事件系统
    // ------------------------------------------------------------------------
    EventDispatcher& getEventDispatcher() { return eventDispatcher_; }

    // ------------------------------------------------------------------------
    // 内部方法
    // ------------------------------------------------------------------------
    void update(float dt);
    void render(RenderBackend& renderer);
    void sortChildren();
    
    bool isRunning() const { return running_; }
    Scene* getScene() const { return scene_; }

    // 多线程渲染命令收集
    virtual void collectRenderCommands(std::vector<RenderCommand>& commands, int parentZOrder = 0);

protected:
    // 子类重写
    virtual void onDraw(RenderBackend& renderer) {}
    virtual void onUpdateNode(float dt) {}
    virtual void generateRenderCommand(std::vector<RenderCommand>& commands, int zOrder) {};

    // 供子类访问的内部状态
    Vec2& getPositionRef() { return position_; }
    Vec2& getScaleRef() { return scale_; }
    Vec2& getAnchorRef() { return anchor_; }
    float getRotationRef() { return rotation_; }
    float getOpacityRef() { return opacity_; }

private:
    // 层级
    WeakPtr<Node> parent_;
    std::vector<Ptr<Node>> children_;
    bool childrenOrderDirty_ = false;

    // 变换
    Vec2 position_ = Vec2::Zero();
    float rotation_ = 0.0f;
    Vec2 scale_ = Vec2(1.0f, 1.0f);
    Vec2 anchor_ = Vec2(0.5f, 0.5f);
    Vec2 skew_ = Vec2::Zero();
    float opacity_ = 1.0f;
    bool visible_ = true;
    int zOrder_ = 0;

    // 缓存
    mutable bool transformDirty_ = true;
    mutable glm::mat4 localTransform_;
    mutable glm::mat4 worldTransform_;

    // 元数据
    std::string name_;
    int tag_ = -1;

    // 状态
    bool running_ = false;
    Scene* scene_ = nullptr;
    bool spatialIndexed_ = true;  // 是否参与空间索引
    Rect lastSpatialBounds_;      // 上一次的空间索引边界（用于检测变化）

    // 动作
    std::vector<Ptr<Action>> actions_;

    // 事件
    EventDispatcher eventDispatcher_;
};

} // namespace easy2d
