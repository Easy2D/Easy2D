#include <easy2d/platform/input.h>
#include <easy2d/platform/glfw_user_pointer.h>
#include <GLFW/glfw3.h>

namespace easy2d {

Input::Input()
    : window_(nullptr)
    , mouseScroll_(0.0f)
    , prevMouseScroll_(0.0f)
    , mouseScrollDelta_(0.0f)
    , mouseVisible_(true)
    , mouseLocked_(false) {
    keyStates_.fill(false);
    prevKeyStates_.fill(false);
    mouseStates_.fill(false);
    prevMouseStates_.fill(false);
}

Input::~Input() {
    shutdown();
}

void Input::init(GLFWwindow* window) {
    window_ = window;

    // 设置滚动回调
    glfwSetScrollCallback(window_, scrollCallback);
    
    // 获取初始鼠标位置
    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    mousePosition_ = Vec2(static_cast<float>(x), static_cast<float>(y));
    prevMousePosition_ = mousePosition_;
}

void Input::shutdown() {
    window_ = nullptr;
}

void Input::update() {
    // 保存上一帧状态
    prevKeyStates_ = keyStates_;
    prevMouseStates_ = mouseStates_;
    prevMousePosition_ = mousePosition_;
    prevMouseScroll_ = mouseScroll_;

    // 更新键盘状态
    for (int i = 0; i < MAX_KEYS; ++i) {
        keyStates_[i] = glfwGetKey(window_, i) == GLFW_PRESS;
    }

    // 更新鼠标按钮状态
    for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i) {
        mouseStates_[i] = glfwGetMouseButton(window_, i) == GLFW_PRESS;
    }

    // 更新鼠标位置
    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    mousePosition_ = Vec2(static_cast<float>(x), static_cast<float>(y));
    
    // 计算鼠标增量
    if (mouseLocked_) {
        // 如果鼠标锁定，重置到中心
        int width, height;
        glfwGetWindowSize(window_, &width, &height);
        mouseDelta_ = Vec2(static_cast<float>(x) - width * 0.5f, 
                          static_cast<float>(y) - height * 0.5f);
        glfwSetCursorPos(window_, width * 0.5, height * 0.5);
    } else {
        mouseDelta_ = mousePosition_ - prevMousePosition_;
    }

    // 计算滚动增量
    mouseScrollDelta_ = mouseScroll_ - prevMouseScroll_;
}

bool Input::isKeyDown(int keyCode) const {
    if (keyCode < 0 || keyCode >= MAX_KEYS) return false;
    return keyStates_[keyCode];
}

bool Input::isKeyPressed(int keyCode) const {
    if (keyCode < 0 || keyCode >= MAX_KEYS) return false;
    return keyStates_[keyCode] && !prevKeyStates_[keyCode];
}

bool Input::isKeyReleased(int keyCode) const {
    if (keyCode < 0 || keyCode >= MAX_KEYS) return false;
    return !keyStates_[keyCode] && prevKeyStates_[keyCode];
}

bool Input::isMouseDown(MouseButton button) const {
    int index = static_cast<int>(button);
    if (index < 0 || index >= MAX_MOUSE_BUTTONS) return false;
    return mouseStates_[index];
}

bool Input::isMousePressed(MouseButton button) const {
    int index = static_cast<int>(button);
    if (index < 0 || index >= MAX_MOUSE_BUTTONS) return false;
    return mouseStates_[index] && !prevMouseStates_[index];
}

bool Input::isMouseReleased(MouseButton button) const {
    int index = static_cast<int>(button);
    if (index < 0 || index >= MAX_MOUSE_BUTTONS) return false;
    return !mouseStates_[index] && prevMouseStates_[index];
}

Vec2 Input::getMousePosition() const {
    return mousePosition_;
}

Vec2 Input::getMouseDelta() const {
    return mouseDelta_;
}

float Input::getMouseScroll() const {
    return mouseScroll_;
}

float Input::getMouseScrollDelta() const {
    return mouseScrollDelta_;
}

void Input::setMousePosition(const Vec2& position) {
    if (window_) {
        glfwSetCursorPos(window_, position.x, position.y);
    }
}

void Input::setMouseVisible(bool visible) {
    mouseVisible_ = visible;
    if (window_) {
        glfwSetInputMode(window_, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
    }
}

void Input::setMouseLocked(bool locked) {
    mouseLocked_ = locked;
    if (window_) {
        if (locked) {
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // 重置到中心
            int width, height;
            glfwGetWindowSize(window_, &width, &height);
            glfwSetCursorPos(window_, width * 0.5, height * 0.5);
        } else {
            glfwSetInputMode(window_, GLFW_CURSOR, mouseVisible_ ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
        }
    }
}

bool Input::isAnyKeyDown() const {
    for (bool state : keyStates_) {
        if (state) return true;
    }
    return false;
}

bool Input::isAnyMouseDown() const {
    for (bool state : mouseStates_) {
        if (state) return true;
    }
    return false;
}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* ctx = static_cast<GlfwUserPointer*>(glfwGetWindowUserPointer(window));
    Input* input = ctx ? ctx->input : nullptr;
    if (input) {
        input->mouseScroll_ += static_cast<float>(yoffset);
    }
}

} // namespace easy2d
