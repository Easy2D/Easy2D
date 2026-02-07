#include <easy2d/platform/window.h>
#include <easy2d/platform/input.h>
#include <easy2d/platform/glfw_user_pointer.h>
#include <easy2d/event/event_queue.h>
#include <easy2d/utils/logger.h>
#include <GLFW/glfw3.h>

// Windows 平台特定头文件，用于禁用最大化按钮
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <windows.h>
#endif

namespace easy2d {

Window::Window()
    : window_(nullptr)
    , width_(0)
    , height_(0)
    , fullscreen_(false)
    , vsync_(true)
    , userData_(nullptr)
    , eventQueue_(nullptr) {
}

Window::~Window() {
    destroy();
}

bool Window::create(const WindowConfig& config) {
    if (window_) {
        E2D_LOG_WARN("Window already created");
        return false;
    }

    // 设置 GLFW 窗口提示
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);  // 禁止窗口最大化

    if (config.msaaSamples > 0) {
        glfwWindowHint(GLFW_SAMPLES, config.msaaSamples);
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    GLFWmonitor* monitor = config.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window_ = glfwCreateWindow(config.width, config.height, config.title.c_str(), monitor, nullptr);

    if (!window_) {
        E2D_LOG_ERROR("Failed to create GLFW window");
        return false;
    }

    width_ = config.width;
    height_ = config.height;

    // 窗口居中显示
    if (!config.fullscreen && config.centerWindow) {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (primaryMonitor) {
            const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
            if (videoMode) {
                int screenWidth = videoMode->width;
                int screenHeight = videoMode->height;
                int windowX = (screenWidth - config.width) / 2;
                int windowY = (screenHeight - config.height) / 2;
                glfwSetWindowPos(window_, windowX, windowY);
                E2D_LOG_INFO("Window centered at: {}x{}", windowX, windowY);
            }
        }
    }
    fullscreen_ = config.fullscreen;
    vsync_ = config.vsync;

#ifdef _WIN32
    // 禁用最大化按钮（Windows 平台）
    if (!config.resizable && window_) {
        HWND hwnd = glfwGetWin32Window(window_);
        if (hwnd) {
            // 移除最大化按钮和可调整大小边框
            LONG style = GetWindowLong(hwnd, GWL_STYLE);
            style &= ~WS_MAXIMIZEBOX;  // 移除最大化按钮
            style &= ~WS_THICKFRAME;   // 移除可调整大小的边框
            SetWindowLong(hwnd, GWL_STYLE, style);
            // 强制窗口重绘以应用新样式
            SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            E2D_LOG_INFO("Window maximize button disabled");
        }
    }
#endif

    // 设置当前上下文
    glfwMakeContextCurrent(window_);

    // 设置 VSync
    glfwSwapInterval(vsync_ ? 1 : 0);

    // 设置用户指针
    glfwUserPointer_ = makeUnique<GlfwUserPointer>();
    glfwUserPointer_->window = this;
    glfwSetWindowUserPointer(window_, glfwUserPointer_.get());

    // 设置回调
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    glfwSetWindowFocusCallback(window_, windowFocusCallback);
    glfwSetWindowCloseCallback(window_, windowCloseCallback);

    // 创建输入管理器
    input_ = makeUnique<Input>();
    input_->init(window_);
    glfwUserPointer_->input = input_.get();

    E2D_LOG_INFO("Window created: {}x{}", width_, height_);
    return true;
}

void Window::destroy() {
    if (window_) {
        // 销毁自定义光标
        if (cursors_.arrow) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.arrow)); cursors_.arrow = nullptr; }
        if (cursors_.ibeam) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.ibeam)); cursors_.ibeam = nullptr; }
        if (cursors_.crosshair) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.crosshair)); cursors_.crosshair = nullptr; }
        if (cursors_.hand) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.hand)); cursors_.hand = nullptr; }
        if (cursors_.hresize) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.hresize)); cursors_.hresize = nullptr; }
        if (cursors_.vresize) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.vresize)); cursors_.vresize = nullptr; }
        if (cursors_.resizeAll) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.resizeAll)); cursors_.resizeAll = nullptr; }
        if (cursors_.resizeNWSE) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.resizeNWSE)); cursors_.resizeNWSE = nullptr; }
        if (cursors_.resizeNESW) { glfwDestroyCursor(static_cast<GLFWcursor*>(cursors_.resizeNESW)); cursors_.resizeNESW = nullptr; }

        input_.reset();
        glfwSetWindowUserPointer(window_, nullptr);
        glfwUserPointer_.reset();
        glfwDestroyWindow(window_);
        window_ = nullptr;
        E2D_LOG_INFO("Window destroyed");
    }
}

void Window::pollEvents() {
    glfwPollEvents();
    if (input_) {
        input_->update();
    }
}

void Window::swapBuffers() {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

bool Window::shouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void Window::setShouldClose(bool close) {
    if (window_) {
        glfwSetWindowShouldClose(window_, close);
    }
}

void Window::setTitle(const String& title) {
    if (window_) {
        glfwSetWindowTitle(window_, title.c_str());
    }
}

void Window::setSize(int width, int height) {
    if (window_) {
        glfwSetWindowSize(window_, width, height);
        width_ = width;
        height_ = height;
    }
}

void Window::setPosition(int x, int y) {
    if (window_) {
        glfwSetWindowPos(window_, x, y);
    }
}

Vec2 Window::getPosition() const {
    if (window_) {
        int x, y;
        glfwGetWindowPos(window_, &x, &y);
        return Vec2(static_cast<float>(x), static_cast<float>(y));
    }
    return Vec2::Zero();
}

void Window::setFullscreen(bool fullscreen) {
    if (window_ && fullscreen_ != fullscreen) {
        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        
        if (fullscreen) {
            glfwSetWindowMonitor(window_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            width_ = mode->width;
            height_ = mode->height;
        } else {
            glfwSetWindowMonitor(window_, nullptr, 100, 100, 800, 600, 0);
            width_ = 800;
            height_ = 600;
        }
        fullscreen_ = fullscreen;
    }
}

void Window::setVSync(bool enabled) {
    vsync_ = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::setResizable(bool resizable) {
    if (window_) {
        glfwSetWindowAttrib(window_, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    }
}

bool Window::isFocused() const {
    return window_ ? glfwGetWindowAttrib(window_, GLFW_FOCUSED) : false;
}

bool Window::isMinimized() const {
    return window_ ? glfwGetWindowAttrib(window_, GLFW_ICONIFIED) : false;
}

bool Window::isMaximized() const {
    return window_ ? glfwGetWindowAttrib(window_, GLFW_MAXIMIZED) : false;
}

float Window::getContentScaleX() const {
    if (!window_) return 1.0f;
    float xscale, yscale;
    glfwGetWindowContentScale(window_, &xscale, &yscale);
    return xscale;
}

float Window::getContentScaleY() const {
    if (!window_) return 1.0f;
    float xscale, yscale;
    glfwGetWindowContentScale(window_, &xscale, &yscale);
    return yscale;
}

Vec2 Window::getContentScale() const {
    if (!window_) return Vec2(1.0f, 1.0f);
    float xscale, yscale;
    glfwGetWindowContentScale(window_, &xscale, &yscale);
    return Vec2(xscale, yscale);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* ctx = static_cast<GlfwUserPointer*>(glfwGetWindowUserPointer(window));
    Window* self = ctx ? ctx->window : nullptr;
    if (self) {
        self->width_ = width;
        self->height_ = height;
        if (self->resizeCallback_) {
            self->resizeCallback_(width, height);
        }
    }
}

void Window::windowFocusCallback(GLFWwindow* window, int focused) {
    auto* ctx = static_cast<GlfwUserPointer*>(glfwGetWindowUserPointer(window));
    Window* self = ctx ? ctx->window : nullptr;
    if (self && self->focusCallback_) {
        self->focusCallback_(focused == GLFW_TRUE);
    }
}

void Window::windowCloseCallback(GLFWwindow* window) {
    auto* ctx = static_cast<GlfwUserPointer*>(glfwGetWindowUserPointer(window));
    Window* self = ctx ? ctx->window : nullptr;
    if (self && self->closeCallback_) {
        self->closeCallback_();
    }
}

void Window::setCursor(CursorShape shape) {
    if (!window_) return;

    void** cursorPtr = nullptr;
    int glfwShape = GLFW_ARROW_CURSOR;

    switch (shape) {
        case CursorShape::Arrow:
            cursorPtr = &cursors_.arrow;
            glfwShape = GLFW_ARROW_CURSOR;
            break;
        case CursorShape::IBeam:
            cursorPtr = &cursors_.ibeam;
            glfwShape = GLFW_IBEAM_CURSOR;
            break;
        case CursorShape::Crosshair:
            cursorPtr = &cursors_.crosshair;
            glfwShape = GLFW_CROSSHAIR_CURSOR;
            break;
        case CursorShape::Hand:
            cursorPtr = &cursors_.hand;
            glfwShape = GLFW_HAND_CURSOR;
            break;
        case CursorShape::HResize:
            cursorPtr = &cursors_.hresize;
            glfwShape = GLFW_HRESIZE_CURSOR;
            break;
        case CursorShape::VResize:
            cursorPtr = &cursors_.vresize;
            glfwShape = GLFW_VRESIZE_CURSOR;
            break;
        case CursorShape::ResizeAll:
            cursorPtr = &cursors_.resizeAll;
            glfwShape = GLFW_RESIZE_ALL_CURSOR;
            break;
        case CursorShape::ResizeNWSE:
            cursorPtr = &cursors_.resizeNWSE;
            glfwShape = GLFW_RESIZE_NWSE_CURSOR;
            break;
        case CursorShape::ResizeNESW:
            cursorPtr = &cursors_.resizeNESW;
            glfwShape = GLFW_RESIZE_NESW_CURSOR;
            break;
    }

    if (cursorPtr) {
        // 延迟创建光标
        if (!*cursorPtr) {
            *cursorPtr = glfwCreateStandardCursor(glfwShape);
        }
        if (*cursorPtr) {
            glfwSetCursor(window_, static_cast<GLFWcursor*>(*cursorPtr));
        }
    }
}

void Window::resetCursor() {
    if (window_) {
        glfwSetCursor(window_, nullptr);
    }
}

} // namespace easy2d
