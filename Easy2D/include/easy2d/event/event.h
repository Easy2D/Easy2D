#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>
#include <cstdint>
#include <variant>

namespace easy2d {

// ============================================================================
// 事件类型枚举
// ============================================================================
enum class EventType {
    None = 0,
    
    // 窗口事件
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    
    // 键盘事件
    KeyPressed,
    KeyReleased,
    KeyRepeat,
    
    // 鼠标事件
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,

    // UI 事件
    UIHoverEnter,
    UIHoverExit,
    UIPressed,
    UIReleased,
    UIClicked,
    
    // 游戏手柄事件
    GamepadConnected,
    GamepadDisconnected,
    GamepadButtonPressed,
    GamepadButtonReleased,
    GamepadAxisMoved,
    
    // 触摸事件 (移动端)
    TouchBegan,
    TouchMoved,
    TouchEnded,
    TouchCancelled,
    
    // 自定义事件
    Custom
};

// ============================================================================
// 键盘事件数据
// ============================================================================
struct KeyEvent {
    int keyCode;
    int scancode;
    int mods;  // 修饰键 (Shift, Ctrl, Alt, etc.)
};

// ============================================================================
// 鼠标事件数据
// ============================================================================
struct MouseButtonEvent {
    int button;
    int mods;
    Vec2 position;
};

struct MouseMoveEvent {
    Vec2 position;
    Vec2 delta;
};

struct MouseScrollEvent {
    Vec2 offset;
    Vec2 position;
};

// ============================================================================
// 窗口事件数据
// ============================================================================
struct WindowResizeEvent {
    int width;
    int height;
};

struct WindowMoveEvent {
    int x;
    int y;
};

// ============================================================================
// 游戏手柄事件数据
// ============================================================================
struct GamepadButtonEvent {
    int gamepadId;
    int button;
};

struct GamepadAxisEvent {
    int gamepadId;
    int axis;
    float value;
};

// ============================================================================
// 触摸事件数据
// ============================================================================
struct TouchEvent {
    int touchId;
    Vec2 position;
};

// ============================================================================
// 自定义事件数据
// ============================================================================
struct CustomEvent {
    uint32_t id;
    void* data;
};

// ============================================================================
// 事件结构
// ============================================================================
struct Event {
    EventType type = EventType::None;
    double timestamp = 0.0;
    bool handled = false;
    
    // 事件数据联合体
    std::variant<
        std::monostate,
        KeyEvent,
        MouseButtonEvent,
        MouseMoveEvent,
        MouseScrollEvent,
        WindowResizeEvent,
        WindowMoveEvent,
        GamepadButtonEvent,
        GamepadAxisEvent,
        TouchEvent,
        CustomEvent
    > data;
    
    // 便捷访问方法
    bool isWindowEvent() const {
        return type == EventType::WindowClose || 
               type == EventType::WindowResize ||
               type == EventType::WindowFocus ||
               type == EventType::WindowLostFocus ||
               type == EventType::WindowMoved;
    }
    
    bool isKeyboardEvent() const {
        return type == EventType::KeyPressed || 
               type == EventType::KeyReleased ||
               type == EventType::KeyRepeat;
    }
    
    bool isMouseEvent() const {
        return type == EventType::MouseButtonPressed || 
               type == EventType::MouseButtonReleased ||
               type == EventType::MouseMoved ||
               type == EventType::MouseScrolled;
    }
    
    // 静态工厂方法
    static Event createWindowResize(int width, int height);
    static Event createWindowClose();
    static Event createKeyPress(int keyCode, int scancode, int mods);
    static Event createKeyRelease(int keyCode, int scancode, int mods);
    static Event createMouseButtonPress(int button, int mods, const Vec2& pos);
    static Event createMouseButtonRelease(int button, int mods, const Vec2& pos);
    static Event createMouseMove(const Vec2& pos, const Vec2& delta);
    static Event createMouseScroll(const Vec2& offset, const Vec2& pos);
};

} // namespace easy2d
