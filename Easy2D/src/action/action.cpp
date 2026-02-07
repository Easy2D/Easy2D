#include "easy2d/action/action.h"
#include "easy2d/scene/node.h"

namespace easy2d {
    Action::Action()
        : elapsed_(0.0f)
        , duration_(0.0f)
        , speed_(1.0f)
        , tag_(-1)
    {
    }

    void Action::start(Node* target)
    {
        target_ = target;
        originalTarget_ = target;
        elapsed_ = 0.0f;
        state_ = ActionState::Running;
        onStart();
    }

    void Action::stop()
    {
        target_ = nullptr;
        state_ = ActionState::Completed;
    }

    void Action::update(float dt)
    {
        if (state_ != ActionState::Running)
            return;

        step(dt);

        if (isDone())
        {
            state_ = ActionState::Completed;
            onComplete();
            if (completionCallback_)
                completionCallback_();
        }
    }

    void Action::step(float dt)
    {
        if (state_ != ActionState::Running)
            return;

        elapsed_ += dt * speed_;

        float progress = 0.0f;
        if (duration_ > 0.0f)
        {
            progress = std::min(1.0f, elapsed_ / duration_);
        }
        else
        {
            progress = 1.0f;
        }

        if (progressCallback_)
            progressCallback_(progress);

        onUpdate(progress);
    }

    void Action::pause()
    {
        if (state_ == ActionState::Running)
            state_ = ActionState::Paused;
    }

    void Action::resume()
    {
        if (state_ == ActionState::Paused)
            state_ = ActionState::Running;
    }

    void Action::restart()
    {
        elapsed_ = 0.0f;
        state_ = ActionState::Running;
        onStart();
    }
}
