#pragma once

#include <easy2d/core/types.h>
#include <easy2d/scene/scene.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <stack>
#include <vector>

namespace easy2d {

// 前向声明
struct RenderCommand;
class Transition;

// ============================================================================
// 场景切换特效类型
// ============================================================================
enum class TransitionType {
    None,
    Fade,
    SlideLeft,
    SlideRight,
    SlideUp,
    SlideDown,
    Scale,
    Flip
};

// ============================================================================
// 场景管理器 - 管理场景的生命周期和切换
// ============================================================================
class SceneManager {
public:
    using TransitionCallback = std::function<void()>;

    // ------------------------------------------------------------------------
    // 单例访问
    // ------------------------------------------------------------------------
    static SceneManager& getInstance();

    // ------------------------------------------------------------------------
    // 场景栈操作
    // ------------------------------------------------------------------------
    
    // 运行第一个场景
    void runWithScene(Ptr<Scene> scene);
    
    // 替换当前场景
    void replaceScene(Ptr<Scene> scene);
    void replaceScene(Ptr<Scene> scene, TransitionType transition, float duration = 0.5f);
    
    // 压入新场景（当前场景暂停）
    void pushScene(Ptr<Scene> scene);
    void pushScene(Ptr<Scene> scene, TransitionType transition, float duration = 0.5f);
    
    // 弹出当前场景（恢复上一个场景）
    void popScene();
    void popScene(TransitionType transition, float duration = 0.5f);
    
    // 弹出到根场景
    void popToRootScene();
    void popToRootScene(TransitionType transition, float duration = 0.5f);
    
    // 弹出到指定场景
    void popToScene(const std::string& name);
    void popToScene(const std::string& name, TransitionType transition, float duration = 0.5f);

    // ------------------------------------------------------------------------
    // 获取场景
    // ------------------------------------------------------------------------
    Ptr<Scene> getCurrentScene() const;
    Ptr<Scene> getPreviousScene() const;
    Ptr<Scene> getRootScene() const;
    
    // 通过名称获取场景
    Ptr<Scene> getSceneByName(const std::string& name) const;

    // ------------------------------------------------------------------------
    // 查询
    // ------------------------------------------------------------------------
    size_t getSceneCount() const { return sceneStack_.size(); }
    bool isEmpty() const { return sceneStack_.empty(); }
    bool hasScene(const std::string& name) const;

    // ------------------------------------------------------------------------
    // 更新和渲染
    // ------------------------------------------------------------------------
    void update(float dt);
    void render(RenderBackend& renderer);
    void collectRenderCommands(std::vector<RenderCommand>& commands);

    // ------------------------------------------------------------------------
    // 过渡控制
    // ------------------------------------------------------------------------
    bool isTransitioning() const { return isTransitioning_; }
    void setTransitionCallback(TransitionCallback callback) { transitionCallback_ = callback; }

    // ------------------------------------------------------------------------
    // 清理
    // ------------------------------------------------------------------------
    void end();
    void purgeCachedScenes();

public:
    SceneManager() = default;
    ~SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // 场景切换（供 Application 使用）
    void enterScene(Ptr<Scene> scene);
    void enterScene(Ptr<Scene> scene, Ptr<class Transition> transition);

private:
    void doSceneSwitch();
    void startTransition(Ptr<Scene> from, Ptr<Scene> to, TransitionType type, float duration, Function<void()> stackAction);
    void updateTransition(float dt);
    void finishTransition();
    void dispatchPointerEvents(Scene& scene);

    std::stack<Ptr<Scene>> sceneStack_;
    std::unordered_map<std::string, Ptr<Scene>> namedScenes_;
    
    // Transition state
    bool isTransitioning_ = false;
    TransitionType currentTransition_ = TransitionType::None;
    float transitionDuration_ = 0.0f;
    float transitionElapsed_ = 0.0f;
    Ptr<Scene> outgoingScene_;
    Ptr<Scene> incomingScene_;
    Ptr<Transition> activeTransition_;
    Function<void()> transitionStackAction_;
    TransitionCallback transitionCallback_;
    
    // Next scene to switch to (queued during transition)
    Ptr<Scene> nextScene_;
    bool sendCleanupToScene_ = false;

    Node* hoverTarget_ = nullptr;
    Node* captureTarget_ = nullptr;
    Vec2 lastPointerWorld_ = Vec2::Zero();
    bool hasLastPointerWorld_ = false;
};

} // namespace easy2d
