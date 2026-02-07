#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/string.h>
#include <easy2d/core/math_types.h>
#include <functional>

struct GLFWwindow;

namespace easy2d {

// 前向声明
class EventQueue;
class Input;
struct GlfwUserPointer;

// ============================================================================
// 窗口配置
// ============================================================================
struct WindowConfig {
    String title = "Easy2D Application";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool resizable = true;
    bool vsync = true;
    int msaaSamples = 0;
    bool centerWindow = true;  // 窗口是否居中显示
};

// ============================================================================
// 鼠标光标形状枚举
// ============================================================================
enum class CursorShape {
    Arrow,      // 默认箭头
    IBeam,      // 文本输入光标（I形）
    Crosshair,  // 十字准线
    Hand,       // 手型（链接悬停）
    HResize,    // 水平调整大小
    VResize,    // 垂直调整大小
    ResizeAll,  // 四向调整大小
    ResizeNWSE, // 对角线调整大小（西北-东南）
    ResizeNESW  // 对角线调整大小（东北-西南）
};

// ============================================================================
// Window 类 - GLFW 封装
// ============================================================================
class Window {
public:
    Window();
    ~Window();

    // 创建窗口
    bool create(const WindowConfig& config);
    void destroy();

    // 窗口操作
    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void setShouldClose(bool close);

    // 窗口属性
    void setTitle(const String& title);
    void setSize(int width, int height);
    void setPosition(int x, int y);
    void setFullscreen(bool fullscreen);
    void setVSync(bool enabled);
    void setResizable(bool resizable);

    // 获取窗口属性
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    Size getSize() const { return Size(static_cast<float>(width_), static_cast<float>(height_)); }
    Vec2 getPosition() const;
    bool isFullscreen() const { return fullscreen_; }
    bool isVSync() const { return vsync_; }

    // DPI 缩放支持
    float getContentScaleX() const;
    float getContentScaleY() const;
    Vec2 getContentScale() const;

    // 窗口状态
    bool isFocused() const;
    bool isMinimized() const;
    bool isMaximized() const;

    // 获取原生句柄
    GLFWwindow* getNativeHandle() const { return window_; }

    // 设置/获取用户数据
    void setUserData(void* data) { userData_ = data; }
    void* getUserData() const { return userData_; }

    // 事件队列
    void setEventQueue(EventQueue* queue) { eventQueue_ = queue; }
    EventQueue* getEventQueue() const { return eventQueue_; }

    // 获取输入管理器
    Input* getInput() const { return input_.get(); }

    // 鼠标光标设置
    void setCursor(CursorShape shape);
    void resetCursor();

    // 窗口回调
    using ResizeCallback = std::function<void(int width, int height)>;
    using FocusCallback = std::function<void(bool focused)>;
    using CloseCallback = std::function<void()>;

    void setResizeCallback(ResizeCallback callback) { resizeCallback_ = callback; }
    void setFocusCallback(FocusCallback callback) { focusCallback_ = callback; }
    void setCloseCallback(CloseCallback callback) { closeCallback_ = callback; }

private:
    GLFWwindow* window_;
    int width_;
    int height_;
    bool fullscreen_;
    bool vsync_;
    void* userData_;
    EventQueue* eventQueue_;
    UniquePtr<Input> input_;
    UniquePtr<GlfwUserPointer> glfwUserPointer_;

    // 光标管理
    struct CursorData {
        void* arrow = nullptr;
        void* ibeam = nullptr;
        void* crosshair = nullptr;
        void* hand = nullptr;
        void* hresize = nullptr;
        void* vresize = nullptr;
        void* resizeAll = nullptr;
        void* resizeNWSE = nullptr;
        void* resizeNESW = nullptr;
    } cursors_;

    ResizeCallback resizeCallback_;
    FocusCallback focusCallback_;
    CloseCallback closeCallback_;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void windowFocusCallback(GLFWwindow* window, int focused);
    static void windowCloseCallback(GLFWwindow* window);
};

} // namespace easy2d
