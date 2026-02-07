#include <easy2d/graphics/camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace easy2d {

Camera::Camera()
    : left_(-1.0f), right_(1.0f), bottom_(-1.0f), top_(1.0f) {
}

Camera::Camera(float left, float right, float bottom, float top)
    : left_(left), right_(right), bottom_(bottom), top_(top) {
}

Camera::Camera(const Size& viewport)
    : left_(0.0f), right_(viewport.width), bottom_(viewport.height), top_(0.0f) {
}

void Camera::setPosition(const Vec2& position) {
    position_ = position;
    viewDirty_ = true;
}

void Camera::setPosition(float x, float y) {
    position_.x = x;
    position_.y = y;
    viewDirty_ = true;
}

void Camera::setRotation(float degrees) {
    rotation_ = degrees;
    viewDirty_ = true;
}

void Camera::setZoom(float zoom) {
    zoom_ = zoom;
    viewDirty_ = true;
    projDirty_ = true;
}

void Camera::setViewport(float left, float right, float bottom, float top) {
    left_ = left;
    right_ = right;
    bottom_ = bottom;
    top_ = top;
    projDirty_ = true;
}

void Camera::setViewport(const Rect& rect) {
    left_ = rect.left();
    right_ = rect.right();
    bottom_ = rect.bottom();
    top_ = rect.top();
    projDirty_ = true;
}

Rect Camera::getViewport() const {
    return Rect(left_, top_, right_ - left_, bottom_ - top_);
}

glm::mat4 Camera::getViewMatrix() const {
    if (viewDirty_) {
        viewMatrix_ = glm::mat4(1.0f);
        // 对于2D相机，我们只需要平移（注意Y轴方向）
        viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(-position_.x, -position_.y, 0.0f));
        viewDirty_ = false;
    }
    return viewMatrix_;
}

glm::mat4 Camera::getProjectionMatrix() const {
    if (projDirty_) {
        // 对于2D游戏，Y轴向下增长（屏幕坐标系）
        // OpenGL默认Y轴向上，所以需要反转Y轴
        // glm::ortho(left, right, bottom, top)
        // 为了Y轴向下：传入 (bottom=height, top=0)，这样Y轴翻转
        projMatrix_ = glm::ortho(
            left_, right_,     // X轴：从左到右
            bottom_, top_,     // Y轴：从下到上（传入bottom>top，实现Y轴向下增长）
            -1.0f, 1.0f
        );
        projDirty_ = false;
    }
    return projMatrix_;
}

glm::mat4 Camera::getViewProjectionMatrix() const {
    // 对于2D相机，我们主要依赖投影矩阵
    // 视口变换已经处理了坐标系转换
    return getProjectionMatrix();
}

Vec2 Camera::screenToWorld(const Vec2& screenPos) const {
    // 屏幕坐标直接映射到世界坐标（在2D中通常相同）
    return screenPos;
}

Vec2 Camera::worldToScreen(const Vec2& worldPos) const {
    // 世界坐标直接映射到屏幕坐标（在2D中通常相同）
    return worldPos;
}

Vec2 Camera::screenToWorld(float x, float y) const {
    return screenToWorld(Vec2(x, y));
}

Vec2 Camera::worldToScreen(float x, float y) const {
    return worldToScreen(Vec2(x, y));
}

void Camera::move(const Vec2& offset) {
    position_ += offset;
    viewDirty_ = true;
}

void Camera::move(float x, float y) {
    position_.x += x;
    position_.y += y;
    viewDirty_ = true;
}

void Camera::setBounds(const Rect& bounds) {
    bounds_ = bounds;
    hasBounds_ = true;
}

void Camera::clearBounds() {
    hasBounds_ = false;
}

void Camera::clampToBounds() {
    if (!hasBounds_) return;
    
    float viewportWidth = (right_ - left_) / zoom_;
    float viewportHeight = (bottom_ - top_) / zoom_;
    
    float minX = bounds_.left() + viewportWidth * 0.5f;
    float maxX = bounds_.right() - viewportWidth * 0.5f;
    float minY = bounds_.top() + viewportHeight * 0.5f;
    float maxY = bounds_.bottom() - viewportHeight * 0.5f;
    
    if (minX > maxX) {
        position_.x = bounds_.center().x;
    } else {
        position_.x = std::clamp(position_.x, minX, maxX);
    }
    
    if (minY > maxY) {
        position_.y = bounds_.center().y;
    } else {
        position_.y = std::clamp(position_.y, minY, maxY);
    }
    
    viewDirty_ = true;
}

void Camera::lookAt(const Vec2& target) {
    position_ = target;
    viewDirty_ = true;
}

} // namespace easy2d
