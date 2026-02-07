#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/string.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/platform/window.h>
#include <memory>

namespace easy2d {

// 前向声明
class Input;
class AudioEngine;
class SceneManager;
class ResourceManager;
class TimerManager;
class EventQueue;
class EventDispatcher;
class Camera;

// ============================================================================
// Application 配置
// ============================================================================
struct AppConfig {
    String title = "Easy2D Application";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool resizable = true;  // 窗口是否可调整大小
    bool vsync = true;
    int fpsLimit = 0;  // 0 = 不限制
    BackendType renderBackend = BackendType::OpenGL;
    int msaaSamples = 0;
};

// ============================================================================
// Application 单例 - 应用主控
// ============================================================================
class Application {
public:
    // Meyer's 单例
    static Application& instance();

    // 禁止拷贝
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // ------------------------------------------------------------------------
    // 生命周期
    // ------------------------------------------------------------------------
    bool init(const AppConfig& config);
    void shutdown();
    void run();
    void quit();

    // ------------------------------------------------------------------------
    // 状态控制
    // ------------------------------------------------------------------------
    void pause();
    void resume();
    bool isPaused() const { return paused_; }
    bool isRunning() const { return running_; }

    // ------------------------------------------------------------------------
    // 子系统访问
    // ------------------------------------------------------------------------
    Window& window() { return *window_; }
    RenderBackend& renderer() { return *renderer_; }
    Input& input();
    AudioEngine& audio();
    SceneManager& scenes();
    ResourceManager& resources();
    TimerManager& timers();
    EventQueue& eventQueue();
    EventDispatcher& eventDispatcher();
    Camera& camera();

    // ------------------------------------------------------------------------
    // 便捷方法
    // ------------------------------------------------------------------------
    void enterScene(Ptr<class Scene> scene);
    void enterScene(Ptr<class Scene> scene, Ptr<class Transition> transition);
    
    float deltaTime() const { return deltaTime_; }
    float totalTime() const { return totalTime_; }
    int fps() const { return currentFps_; }

    // 获取配置
    const AppConfig& getConfig() const { return config_; }

private:
    Application() = default;
    ~Application();

    void mainLoop();
    void update();
    void render();

    // 配置
    AppConfig config_;

    // 子系统
    UniquePtr<Window> window_;
    UniquePtr<RenderBackend> renderer_;
    UniquePtr<SceneManager> sceneManager_;
    UniquePtr<ResourceManager> resourceManager_;
    UniquePtr<TimerManager> timerManager_;
    UniquePtr<EventQueue> eventQueue_;
    UniquePtr<EventDispatcher> eventDispatcher_;
    UniquePtr<Camera> camera_;

    // 状态
    bool initialized_ = false;
    bool running_ = false;
    bool paused_ = false;
    bool shouldQuit_ = false;

    // 时间
    float deltaTime_ = 0.0f;
    float totalTime_ = 0.0f;
    double lastFrameTime_ = 0.0;
    int frameCount_ = 0;
    float fpsTimer_ = 0.0f;
    int currentFps_ = 0;
};

} // namespace easy2d
