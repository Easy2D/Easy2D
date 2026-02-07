#pragma once

namespace easy2d {
    // Easing function type
    using EaseFunction = float (*)(float);

    // Linear (no easing)
    float easeLinear(float t);

    // Quadratic
    float easeInQuad(float t);
    float easeOutQuad(float t);
    float easeInOutQuad(float t);

    // Cubic
    float easeInCubic(float t);
    float easeOutCubic(float t);
    float easeInOutCubic(float t);

    // Quartic
    float easeInQuart(float t);
    float easeOutQuart(float t);
    float easeInOutQuart(float t);

    // Quintic
    float easeInQuint(float t);
    float easeOutQuint(float t);
    float easeInOutQuint(float t);

    // Sine
    float easeInSine(float t);
    float easeOutSine(float t);
    float easeInOutSine(float t);

    // Exponential
    float easeInExpo(float t);
    float easeOutExpo(float t);
    float easeInOutExpo(float t);

    // Circular
    float easeInCirc(float t);
    float easeOutCirc(float t);
    float easeInOutCirc(float t);

    // Back
    float easeInBack(float t);
    float easeOutBack(float t);
    float easeInOutBack(float t);

    // Elastic
    float easeInElastic(float t);
    float easeOutElastic(float t);
    float easeInOutElastic(float t);

    // Bounce
    float easeInBounce(float t);
    float easeOutBounce(float t);
    float easeInOutBounce(float t);

    // Ease Action wrapper
    class Action;
    
    class EaseAction
    {
    public:
        static Action* create(Action* action, EaseFunction easeFunc);
    };
}
