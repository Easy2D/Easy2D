#include "easy2d/action/actions.h"
#include "easy2d/scene/node.h"
#include <algorithm>

namespace easy2d {
    // IntervalAction
    IntervalAction::IntervalAction(float duration)
    {
        duration_ = duration;
    }

    bool IntervalAction::isDone() const
    {
        return elapsed_ >= duration_;
    }

    // InstantAction
    InstantAction::InstantAction()
    {
        duration_ = 0.0f;
    }

    bool InstantAction::isDone() const
    {
        return true;
    }

    // MoveBy
    MoveBy::MoveBy(float duration, const Vec2& delta)
        : IntervalAction(duration)
        , delta_(delta)
    {
    }

    void MoveBy::onStart()
    {
        startPosition_ = target_->getPosition();
    }

    void MoveBy::onUpdate(float progress)
    {
        Vec2 newPos = startPosition_ + delta_ * progress;
        target_->setPosition(newPos);
    }

    Action* MoveBy::clone() const
    {
        return new MoveBy(duration_, delta_);
    }

    Action* MoveBy::reverse() const
    {
        return new MoveBy(duration_, -delta_);
    }

    // MoveTo
    MoveTo::MoveTo(float duration, const Vec2& position)
        : IntervalAction(duration)
        , endPosition_(position)
    {
    }

    void MoveTo::onStart()
    {
        startPosition_ = target_->getPosition();
        delta_ = endPosition_ - startPosition_;
    }

    void MoveTo::onUpdate(float progress)
    {
        Vec2 newPos = startPosition_ + delta_ * progress;
        target_->setPosition(newPos);
    }

    Action* MoveTo::clone() const
    {
        return new MoveTo(duration_, endPosition_);
    }

    Action* MoveTo::reverse() const
    {
        return new MoveTo(duration_, startPosition_);
    }

    // ScaleBy
    ScaleBy::ScaleBy(float duration, float scale)
        : IntervalAction(duration)
        , deltaScale_(scale - 1.0f, scale - 1.0f)
    {
    }

    ScaleBy::ScaleBy(float duration, float scaleX, float scaleY)
        : IntervalAction(duration)
        , deltaScale_(scaleX - 1.0f, scaleY - 1.0f)
    {
    }

    ScaleBy::ScaleBy(float duration, const Vec2& scale)
        : IntervalAction(duration)
        , deltaScale_(scale.x - 1.0f, scale.y - 1.0f)
    {
    }

    void ScaleBy::onStart()
    {
        startScale_ = target_->getScale();
    }

    void ScaleBy::onUpdate(float progress)
    {
        Vec2 newScale = startScale_ + deltaScale_ * progress;
        target_->setScale(newScale);
    }

    Action* ScaleBy::clone() const
    {
        return new ScaleBy(duration_, Vec2(startScale_.x + deltaScale_.x, startScale_.y + deltaScale_.y));
    }

    Action* ScaleBy::reverse() const
    {
        return new ScaleBy(duration_, Vec2(startScale_.x - deltaScale_.x, startScale_.y - deltaScale_.y));
    }

    // ScaleTo
    ScaleTo::ScaleTo(float duration, float scale)
        : IntervalAction(duration)
        , endScale_(scale, scale)
    {
    }

    ScaleTo::ScaleTo(float duration, float scaleX, float scaleY)
        : IntervalAction(duration)
        , endScale_(scaleX, scaleY)
    {
    }

    ScaleTo::ScaleTo(float duration, const Vec2& scale)
        : IntervalAction(duration)
        , endScale_(scale)
    {
    }

    void ScaleTo::onStart()
    {
        startScale_ = target_->getScale();
        delta_ = endScale_ - startScale_;
    }

    void ScaleTo::onUpdate(float progress)
    {
        Vec2 newScale = startScale_ + delta_ * progress;
        target_->setScale(newScale);
    }

    Action* ScaleTo::clone() const
    {
        return new ScaleTo(duration_, endScale_);
    }

    Action* ScaleTo::reverse() const
    {
        return new ScaleTo(duration_, startScale_);
    }

    // RotateBy
    RotateBy::RotateBy(float duration, float deltaAngle)
        : IntervalAction(duration)
        , deltaAngle_(deltaAngle)
    {
    }

    RotateBy::RotateBy(float duration, float deltaAngleX, float deltaAngleY)
        : IntervalAction(duration)
        , deltaAngle_(deltaAngleX)
    {
        (void)deltaAngleY;
    }

    void RotateBy::onStart()
    {
        startAngle_ = target_->getRotation();
    }

    void RotateBy::onUpdate(float progress)
    {
        float newAngle = startAngle_ + deltaAngle_ * progress;
        target_->setRotation(newAngle);
    }

    Action* RotateBy::clone() const
    {
        return new RotateBy(duration_, deltaAngle_);
    }

    Action* RotateBy::reverse() const
    {
        return new RotateBy(duration_, -deltaAngle_);
    }

    // RotateTo
    RotateTo::RotateTo(float duration, float angle)
        : IntervalAction(duration)
        , endAngle_(angle)
    {
    }

    RotateTo::RotateTo(float duration, float angleX, float angleY)
        : IntervalAction(duration)
        , endAngle_(angleX)
    {
        (void)angleY;
    }

    void RotateTo::onStart()
    {
        startAngle_ = target_->getRotation();
        deltaAngle_ = endAngle_ - startAngle_;
        
        // Shortest path
        if (deltaAngle_ > 180.0f)
            deltaAngle_ -= 360.0f;
        if (deltaAngle_ < -180.0f)
            deltaAngle_ += 360.0f;
    }

    void RotateTo::onUpdate(float progress)
    {
        float newAngle = startAngle_ + deltaAngle_ * progress;
        target_->setRotation(newAngle);
    }

    Action* RotateTo::clone() const
    {
        return new RotateTo(duration_, endAngle_);
    }

    Action* RotateTo::reverse() const
    {
        return new RotateTo(duration_, startAngle_);
    }

    // FadeIn
    FadeIn::FadeIn(float duration)
        : IntervalAction(duration)
    {
    }

    void FadeIn::onStart()
    {
        startOpacity_ = target_->getOpacity();
        target_->setOpacity(0.0f);
    }

    void FadeIn::onUpdate(float progress)
    {
        target_->setOpacity(progress);
    }

    Action* FadeIn::clone() const
    {
        return new FadeIn(duration_);
    }

    Action* FadeIn::reverse() const
    {
        return new FadeOut(duration_);
    }

    // FadeOut
    FadeOut::FadeOut(float duration)
        : IntervalAction(duration)
    {
    }

    void FadeOut::onStart()
    {
        startOpacity_ = target_->getOpacity();
        target_->setOpacity(1.0f);
    }

    void FadeOut::onUpdate(float progress)
    {
        target_->setOpacity(1.0f - progress);
    }

    Action* FadeOut::clone() const
    {
        return new FadeOut(duration_);
    }

    Action* FadeOut::reverse() const
    {
        return new FadeIn(duration_);
    }

    // FadeTo
    FadeTo::FadeTo(float duration, float opacity)
        : IntervalAction(duration)
        , endOpacity_(opacity)
    {
    }

    void FadeTo::onStart()
    {
        startOpacity_ = target_->getOpacity();
        deltaOpacity_ = endOpacity_ - startOpacity_;
    }

    void FadeTo::onUpdate(float progress)
    {
        target_->setOpacity(startOpacity_ + deltaOpacity_ * progress);
    }

    Action* FadeTo::clone() const
    {
        return new FadeTo(duration_, endOpacity_);
    }

    Action* FadeTo::reverse() const
    {
        return new FadeTo(duration_, startOpacity_);
    }

    // Sequence
    Sequence::Sequence(const std::vector<Action*>& actions)
        : IntervalAction(0.0f)
    {
        for (auto* action : actions)
        {
            if (action)
            {
                actions_.push_back(action->clone());
                duration_ += action->getDuration();
            }
        }
    }

    Sequence::~Sequence()
    {
        for (auto* action : actions_)
            delete action;
    }

    void Sequence::onStart()
    {
        currentIndex_ = 0;
        split_ = 0.0f;
        last_ = -1.0f;
        
        if (!actions_.empty())
        {
            actions_[0]->start(target_);
        }
    }

    void Sequence::onUpdate(float progress)
    {
        int found = 0;
        float newTime = progress * duration_;
        
        if (newTime < last_)
        {
            // Rewind
            for (auto* action : actions_)
            {
                action->stop();
            }
        }
        
        last_ = newTime;
        
        for (size_t i = 0; i < actions_.size(); ++i)
        {
            split_ += actions_[i]->getDuration();
            
            if (split_ > newTime)
            {
                found = static_cast<int>(i);
                break;
            }
            else if (split_ == newTime)
            {
                found = static_cast<int>(i) + 1;
                break;
            }
        }
        
        if (found != currentIndex_)
        {
            if (currentIndex_ >= 0 && currentIndex_ < static_cast<int>(actions_.size()))
            {
                actions_[currentIndex_]->update(actions_[currentIndex_]->getDuration());
            }
            
            if (found >= 0 && found < static_cast<int>(actions_.size()))
            {
                actions_[found]->start(target_);
            }
            
            currentIndex_ = found;
        }
        
        if (currentIndex_ >= 0 && currentIndex_ < static_cast<int>(actions_.size()))
        {
            float localProgress = 0.0f;
            if (actions_[currentIndex_]->getDuration() > 0.0f)
            {
                localProgress = (newTime - (split_ - actions_[currentIndex_]->getDuration())) / actions_[currentIndex_]->getDuration();
            }
            actions_[currentIndex_]->step(actions_[currentIndex_]->getDuration() * localProgress);
        }
    }

    Action* Sequence::clone() const
    {
        return new Sequence(actions_);
    }

    Action* Sequence::reverse() const
    {
        std::vector<Action*> rev;
        for (auto it = actions_.rbegin(); it != actions_.rend(); ++it)
        {
            rev.push_back((*it)->reverse());
        }
        return new Sequence(rev);
    }

    // Spawn
    Spawn::Spawn(const std::vector<Action*>& actions)
        : IntervalAction(0.0f)
    {
        for (auto* action : actions)
        {
            if (action)
            {
                actions_.push_back(action->clone());
                duration_ = std::max(duration_, action->getDuration());
            }
        }
    }

    Spawn::~Spawn()
    {
        for (auto* action : actions_)
            delete action;
    }

    void Spawn::onStart()
    {
        for (auto* action : actions_)
        {
            action->start(target_);
        }
    }

    void Spawn::onUpdate(float progress)
    {
        for (auto* action : actions_)
        {
            float localProgress = 0.0f;
            if (action->getDuration() > 0.0f)
            {
                localProgress = std::min(1.0f, (progress * duration_) / action->getDuration());
            }
            else
            {
                localProgress = 1.0f;
            }
            action->step(action->getDuration() * localProgress);
        }
    }

    Action* Spawn::clone() const
    {
        return new Spawn(actions_);
    }

    Action* Spawn::reverse() const
    {
        std::vector<Action*> rev;
        for (auto* action : actions_)
        {
            rev.push_back(action->reverse());
        }
        return new Spawn(rev);
    }

    // Loop
    Loop::Loop(Action* action, int times)
        : action_(action ? action->clone() : nullptr)
        , times_(times)
        , currentTimes_(0)
    {
        if (action_)
        {
            duration_ = times < 0 ? -1.0f : action_->getDuration() * times;
        }
    }

    Loop::~Loop()
    {
        delete action_;
    }

    bool Loop::isDone() const
    {
        if (times_ < 0)
            return false;
        return currentTimes_ >= times_;
    }

    void Loop::onStart()
    {
        currentTimes_ = 0;
        if (action_)
        {
            action_->start(target_);
        }
    }

    void Loop::onUpdate(float progress)
    {
        if (!action_)
            return;

        float actionDuration = action_->getDuration();
        float dt = progress * duration_ - elapsed_;
        
        while (dt > 0.0f)
        {
            float localProgress = std::min(1.0f, dt / actionDuration);
            action_->step(actionDuration * localProgress);
            
            if (action_->isDone())
            {
                currentTimes_++;
                if (times_ > 0 && currentTimes_ >= times_)
                    break;
                action_->restart();
            }
            
            dt -= actionDuration;
        }
    }

    Action* Loop::clone() const
    {
        return new Loop(action_, times_);
    }

    Action* Loop::reverse() const
    {
        return new Loop(action_ ? action_->reverse() : nullptr, times_);
    }

    // Delay
    Delay::Delay(float duration)
        : IntervalAction(duration)
    {
    }

    void Delay::onUpdate(float progress)
    {
        // No update needed, just wait
    }

    Action* Delay::clone() const
    {
        return new Delay(duration_);
    }

    Action* Delay::reverse() const
    {
        return new Delay(duration_);
    }

    // CallFunc
    CallFunc::CallFunc(Callback callback)
        : callback_(std::move(callback))
    {
    }

    void CallFunc::onUpdate(float progress)
    {
        (void)progress;
        if (callback_)
            callback_();
    }

    Action* CallFunc::clone() const
    {
        return new CallFunc(callback_);
    }

    Action* CallFunc::reverse() const
    {
        return new CallFunc(callback_);
    }
}
