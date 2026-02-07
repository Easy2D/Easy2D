#pragma once

#include "easy2d/action/action.h"
#include "easy2d/core/math_types.h"
#include <vector>
#include <functional>

namespace easy2d {
    // Interval Action Base
    class IntervalAction : public Action
    {
    public:
        explicit IntervalAction(float duration);
        bool isDone() const override;
    };

    // Instant Action Base
    class InstantAction : public Action
    {
    public:
        InstantAction();
        bool isDone() const override;
    };

    // Move Actions
    class MoveBy : public IntervalAction
    {
    public:
        MoveBy(float duration, const Vec2& delta);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        Vec2 delta_;
        Vec2 startPosition_;
    };

    class MoveTo : public IntervalAction
    {
    public:
        MoveTo(float duration, const Vec2& position);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        Vec2 endPosition_;
        Vec2 startPosition_;
        Vec2 delta_;
    };

    // Scale Actions
    class ScaleBy : public IntervalAction
    {
    public:
        ScaleBy(float duration, float scale);
        ScaleBy(float duration, float scaleX, float scaleY);
        ScaleBy(float duration, const Vec2& scale);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        Vec2 deltaScale_;
        Vec2 startScale_;
    };

    class ScaleTo : public IntervalAction
    {
    public:
        ScaleTo(float duration, float scale);
        ScaleTo(float duration, float scaleX, float scaleY);
        ScaleTo(float duration, const Vec2& scale);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        Vec2 endScale_;
        Vec2 startScale_;
        Vec2 delta_;
    };

    // Rotate Actions
    class RotateBy : public IntervalAction
    {
    public:
        RotateBy(float duration, float deltaAngle);
        RotateBy(float duration, float deltaAngleX, float deltaAngleY);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        float deltaAngle_ = 0.0f;
        float startAngle_ = 0.0f;
    };

    class RotateTo : public IntervalAction
    {
    public:
        RotateTo(float duration, float angle);
        RotateTo(float duration, float angleX, float angleY);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        float endAngle_ = 0.0f;
        float startAngle_ = 0.0f;
        float deltaAngle_ = 0.0f;
    };

    // Fade Actions
    class FadeIn : public IntervalAction
    {
    public:
        explicit FadeIn(float duration);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        float startOpacity_ = 0.0f;
    };

    class FadeOut : public IntervalAction
    {
    public:
        explicit FadeOut(float duration);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        float startOpacity_ = 0.0f;
    };

    class FadeTo : public IntervalAction
    {
    public:
        FadeTo(float duration, float opacity);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        float endOpacity_ = 0.0f;
        float startOpacity_ = 0.0f;
        float deltaOpacity_ = 0.0f;
    };

    // Composite Actions
    class Sequence : public IntervalAction
    {
    public:
        Sequence(const std::vector<Action*>& actions);
        ~Sequence();
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        std::vector<Action*> actions_;
        int currentIndex_ = 0;
        float split_ = 0.0f;
        float last_ = 0.0f;
    };

    class Spawn : public IntervalAction
    {
    public:
        Spawn(const std::vector<Action*>& actions);
        ~Spawn();
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        std::vector<Action*> actions_;
    };

    // Loop Action
    class Loop : public Action
    {
    public:
        Loop(Action* action, int times = -1);
        ~Loop();
        
        bool isDone() const override;
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onStart() override;
        void onUpdate(float progress) override;

    private:
        Action* action_ = nullptr;
        int times_ = 1;
        int currentTimes_ = 0;
    };

    // Delay Action
    class Delay : public IntervalAction
    {
    public:
        explicit Delay(float duration);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onUpdate(float progress) override;
    };

    // CallFunc Action
    class CallFunc : public InstantAction
    {
    public:
        using Callback = std::function<void()>;
        
        explicit CallFunc(Callback callback);
        
        Action* clone() const override;
        Action* reverse() const override;

    protected:
        void onUpdate(float progress) override;

    private:
        Callback callback_;
    };

    // Helper functions
    inline Sequence* sequence(const std::vector<Action*>& actions) { return new Sequence(actions); }
    inline Spawn* spawn(const std::vector<Action*>& actions) { return new Spawn(actions); }
    inline Loop* loop(Action* action, int times = -1) { return new Loop(action, times); }
    inline Delay* delay(float duration) { return new Delay(duration); }
    inline CallFunc* callFunc(CallFunc::Callback callback) { return new CallFunc(std::move(callback)); }
}
