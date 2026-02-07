#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/color.h>
#include <easy2d/scene/scene.h>
#include <functional>

namespace easy2d {

// ============================================================================
// 过渡方向
// ============================================================================
enum class TransitionDirection {
    Left,
    Right,
    Up,
    Down
};

// ============================================================================
// 过渡效果基类
// ============================================================================
class Transition : public std::enable_shared_from_this<Transition> {
public:
    using FinishCallback = std::function<void()>;

    Transition(float duration);
    virtual ~Transition() = default;

    // 开始过渡
    void start(Ptr<Scene> from, Ptr<Scene> to);
    
    // 更新过渡进度
    void update(float dt);
    
    // 渲染过渡效果
    virtual void render(RenderBackend& renderer);

    // 是否完成
    bool isFinished() const { return isFinished_; }
    
    // 获取进度 [0, 1]
    float getProgress() const { return progress_; }
    
    // 获取淡入淡出进度 (0->1 for fade in, 1->0 for fade out)
    float getFadeInAlpha() const;
    float getFadeOutAlpha() const;

    // 设置完成回调
    void setFinishCallback(FinishCallback callback) { finishCallback_ = callback; }

    // 获取源场景和目标场景
    Ptr<Scene> getOutgoingScene() const { return outgoingScene_; }
    Ptr<Scene> getIncomingScene() const { return incomingScene_; }

protected:
    // 子类实现具体的渲染效果
    virtual void onRenderTransition(RenderBackend& renderer, float progress) = 0;
    
    // 过渡完成时调用
    virtual void onFinish();

    float duration_;
    float elapsed_;
    float progress_;
    bool isFinished_;
    bool isStarted_;
    
    Ptr<Scene> outgoingScene_;
    Ptr<Scene> incomingScene_;
    FinishCallback finishCallback_;
};

// ============================================================================
// 淡入淡出过渡
// ============================================================================
class FadeTransition : public Transition {
public:
    FadeTransition(float duration);
    
protected:
    void onRenderTransition(RenderBackend& renderer, float progress) override;
};

// ============================================================================
// 滑动过渡
// ============================================================================
class SlideTransition : public Transition {
public:
    SlideTransition(float duration, TransitionDirection direction);
    
protected:
    void onRenderTransition(RenderBackend& renderer, float progress) override;

private:
    TransitionDirection direction_;
};

// ============================================================================
// 缩放过渡
// ============================================================================
class ScaleTransition : public Transition {
public:
    ScaleTransition(float duration);
    
protected:
    void onRenderTransition(RenderBackend& renderer, float progress) override;
};

// ============================================================================
// 翻页过渡
// ============================================================================
class FlipTransition : public Transition {
public:
    enum class Axis { Horizontal, Vertical };
    
    FlipTransition(float duration, Axis axis = Axis::Horizontal);
    
protected:
    void onRenderTransition(RenderBackend& renderer, float progress) override;

private:
    Axis axis_;
};

// ============================================================================
// 马赛克/方块过渡
// ============================================================================
class BoxTransition : public Transition {
public:
    BoxTransition(float duration, int divisions = 8);
    
protected:
    void onRenderTransition(RenderBackend& renderer, float progress) override;

private:
    int divisions_;
};

} // namespace easy2d
