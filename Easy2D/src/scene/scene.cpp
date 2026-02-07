#include <easy2d/scene/scene.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/render_command.h>
#include <easy2d/utils/logger.h>

namespace easy2d {

Scene::Scene() {
    defaultCamera_ = makePtr<Camera>();
}

void Scene::setCamera(Ptr<Camera> camera) {
    camera_ = camera;
}

void Scene::setViewportSize(float width, float height) {
    viewportSize_ = Size(width, height);
    if (defaultCamera_) {
        defaultCamera_->setViewport(0, width, height, 0);
    }
    if (camera_) {
        camera_->setViewport(0, width, height, 0);
    }
}

void Scene::setViewportSize(const Size& size) {
    setViewportSize(size.width, size.height);
}

void Scene::renderScene(RenderBackend& renderer) {
    if (!isVisible()) return;
    
    // Begin frame with background color
    renderer.beginFrame(backgroundColor_);
    renderContent(renderer);
    renderer.endFrame();
}

void Scene::renderContent(RenderBackend& renderer) {
    if (!isVisible()) return;

    Camera* activeCam = getActiveCamera();
    if (activeCam) {
        renderer.setViewProjection(activeCam->getViewProjectionMatrix());
    }

    renderer.beginSpriteBatch();
    render(renderer);
    renderer.endSpriteBatch();
}

void Scene::updateScene(float dt) {
    if (!paused_) {
        update(dt);
    }
}

void Scene::onEnter() {
    Node::onEnter();
    
    // 初始化空间索引世界边界
    if (spatialIndexingEnabled_) {
        spatialManager_.setWorldBounds(Rect(0, 0, viewportSize_.width, viewportSize_.height));
    }
}

void Scene::onExit() {
    // 清理空间索引
    spatialManager_.clear();
    Node::onExit();
}

void Scene::updateNodeInSpatialIndex(Node* node, const Rect& oldBounds, const Rect& newBounds) {
    if (!spatialIndexingEnabled_ || !node || !node->isSpatialIndexed()) {
        return;
    }
    
    // 如果旧边界有效，先移除
    if (!oldBounds.empty()) {
        spatialManager_.remove(node);
    }
    
    // 如果新边界有效，插入
    if (!newBounds.empty()) {
        spatialManager_.insert(node, newBounds);
    }
}

void Scene::removeNodeFromSpatialIndex(Node* node) {
    if (!spatialIndexingEnabled_ || !node) {
        return;
    }
    
    spatialManager_.remove(node);
}

std::vector<Node*> Scene::queryNodesInArea(const Rect& area) const {
    if (!spatialIndexingEnabled_) {
        return {};
    }
    return spatialManager_.query(area);
}

std::vector<Node*> Scene::queryNodesAtPoint(const Vec2& point) const {
    if (!spatialIndexingEnabled_) {
        return {};
    }
    return spatialManager_.query(point);
}

std::vector<std::pair<Node*, Node*>> Scene::queryCollisions() const {
    if (!spatialIndexingEnabled_) {
        return {};
    }
    return spatialManager_.queryCollisions();
}

void Scene::collectRenderCommands(std::vector<RenderCommand>& commands) {
    if (!isVisible()) return;

    // 从场景的子节点开始收集渲染命令
    Node::collectRenderCommands(commands);
}

Ptr<Scene> Scene::create() {
    return makePtr<Scene>();
}

} // namespace easy2d
