#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>
#include <array>

struct GLFWwindow;

namespace easy2d {

// ============================================================================
// 鼠标按钮枚举
// ============================================================================
enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7,
    Count = 8
};

// ============================================================================
// Input 类 - 输入管理
// ============================================================================
class Input {
public:
    Input();
    ~Input();

    // 初始化
    void init(GLFWwindow* window);
    void shutdown();

    // 每帧更新
    void update();

    // ------------------------------------------------------------------------
    // 键盘输入
    // ------------------------------------------------------------------------
    bool isKeyDown(int keyCode) const;
    bool isKeyPressed(int keyCode) const;
    bool isKeyReleased(int keyCode) const;

    // ------------------------------------------------------------------------
    // 鼠标输入
    // ------------------------------------------------------------------------
    bool isMouseDown(MouseButton button) const;
    bool isMousePressed(MouseButton button) const;
    bool isMouseReleased(MouseButton button) const;

    Vec2 getMousePosition() const;
    Vec2 getMouseDelta() const;
    float getMouseScroll() const;
    float getMouseScrollDelta() const;

    void setMousePosition(const Vec2& position);
    void setMouseVisible(bool visible);
    void setMouseLocked(bool locked);

    // ------------------------------------------------------------------------
    // 便捷方法
    // ------------------------------------------------------------------------
    bool isAnyKeyDown() const;
    bool isAnyMouseDown() const;

private:
    static constexpr int MAX_KEYS = 512;
    static constexpr int MAX_MOUSE_BUTTONS = 8;

    GLFWwindow* window_;

    // 键盘状态
    std::array<bool, MAX_KEYS> keyStates_;
    std::array<bool, MAX_KEYS> prevKeyStates_;

    // 鼠标状态
    std::array<bool, MAX_MOUSE_BUTTONS> mouseStates_;
    std::array<bool, MAX_MOUSE_BUTTONS> prevMouseStates_;

    // 鼠标位置和滚动
    Vec2 mousePosition_;
    Vec2 prevMousePosition_;
    Vec2 mouseDelta_;
    float mouseScroll_;
    float prevMouseScroll_;
    float mouseScrollDelta_;

    bool mouseVisible_;
    bool mouseLocked_;

    // GLFW 回调
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

} // namespace easy2d
