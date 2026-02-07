#pragma once

#include <functional>
#include <memory>

namespace easy2d {
    class Node;

    enum class ActionState
    {
        Idle,
        Running,
        Paused,
        Completed
    };

    class Action
    {
    public:
        using ProgressCallback = std::function<void(float)>;
        using CompletionCallback = std::function<void()>;

        Action();
        virtual ~Action() = default;

        Action(const Action&) = delete;
        Action& operator=(const Action&) = delete;
        Action(Action&&) = default;
        Action& operator=(Action&&) = default;

        virtual void start(Node* target);
        virtual void stop();
        virtual void update(float dt);
        virtual void step(float dt);

        virtual bool isDone() const = 0;
        virtual Action* clone() const = 0;
        virtual Action* reverse() const = 0;

        void pause();
        void resume();
        void restart();

        ActionState getState() const { return state_; }
        float getElapsed() const { return elapsed_; }
        float getDuration() const { return duration_; }
        Node* getTarget() const { return target_; }
        Node* getOriginalTarget() const { return originalTarget_; }

        void setDuration(float duration) { duration_ = duration; }
        void setSpeed(float speed) { speed_ = speed; }
        float getSpeed() const { return speed_; }

        void setProgressCallback(ProgressCallback callback) { progressCallback_ = std::move(callback); }
        void setCompletionCallback(CompletionCallback callback) { completionCallback_ = std::move(callback); }

        void setTag(int tag) { tag_ = tag; }
        int getTag() const { return tag_; }

    protected:
        virtual void onStart() {}
        virtual void onUpdate(float progress) = 0;
        virtual void onComplete() {}

        void setDone() { state_ = ActionState::Completed; }

        Node* target_ = nullptr;
        Node* originalTarget_ = nullptr;
        ActionState state_ = ActionState::Idle;
        float elapsed_ = 0.0f;
        float duration_ = 0.0f;
        float speed_ = 1.0f;
        int tag_ = -1;
        ProgressCallback progressCallback_;
        CompletionCallback completionCallback_;
    };

    using ActionPtr = std::unique_ptr<Action>;
}
