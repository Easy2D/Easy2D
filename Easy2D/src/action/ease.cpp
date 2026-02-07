#include <easy2d/action/ease.h>
#include <cmath>

namespace easy2d {
    // Linear
    float easeLinear(float t)
    {
        return t;
    }

    // Quadratic
    float easeInQuad(float t)
    {
        return t * t;
    }

    float easeOutQuad(float t)
    {
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    float easeInOutQuad(float t)
    {
        return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
    }

    // Cubic
    float easeInCubic(float t)
    {
        return t * t * t;
    }

    float easeOutCubic(float t)
    {
        return 1.0f - std::pow(1.0f - t, 3.0f);
    }

    float easeInOutCubic(float t)
    {
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    // Quartic
    float easeInQuart(float t)
    {
        return t * t * t * t;
    }

    float easeOutQuart(float t)
    {
        return 1.0f - std::pow(1.0f - t, 4.0f);
    }

    float easeInOutQuart(float t)
    {
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
    }

    // Quintic
    float easeInQuint(float t)
    {
        return t * t * t * t * t;
    }

    float easeOutQuint(float t)
    {
        return 1.0f - std::pow(1.0f - t, 5.0f);
    }

    float easeInOutQuint(float t)
    {
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
    }

    // Sine
    float easeInSine(float t)
    {
        return 1.0f - std::cos((t * 3.14159265359f) / 2.0f);
    }

    float easeOutSine(float t)
    {
        return std::sin((t * 3.14159265359f) / 2.0f);
    }

    float easeInOutSine(float t)
    {
        return -(std::cos(3.14159265359f * t) - 1.0f) / 2.0f;
    }

    // Exponential
    float easeInExpo(float t)
    {
        return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
    }

    float easeOutExpo(float t)
    {
        return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
    }

    float easeInOutExpo(float t)
    {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    // Circular
    float easeInCirc(float t)
    {
        return 1.0f - std::sqrt(1.0f - std::pow(t, 2.0f));
    }

    float easeOutCirc(float t)
    {
        return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
    }

    float easeInOutCirc(float t)
    {
        return t < 0.5f
            ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f
            : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    // Back
    float easeInBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return c3 * t * t * t - c1 * t * t;
    }

    float easeOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
    }

    float easeInOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        return t < 0.5f
            ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
            : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    // Elastic
    float easeInElastic(float t)
    {
        const float c4 = (2.0f * 3.14159265359f) / 3.0f;
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
    }

    float easeOutElastic(float t)
    {
        const float c4 = (2.0f * 3.14159265359f) / 3.0f;
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
    }

    float easeInOutElastic(float t)
    {
        const float c5 = (2.0f * 3.14159265359f) / 4.5f;
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        return t < 0.5f
            ? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
            : (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
    }

    // Bounce
    namespace
    {
        float easeOutBounceInternal(float t)
        {
            const float n1 = 7.5625f;
            const float d1 = 2.75f;
            
            if (t < 1.0f / d1)
            {
                return n1 * t * t;
            }
            else if (t < 2.0f / d1)
            {
                t -= 1.5f / d1;
                return n1 * t * t + 0.75f;
            }
            else if (t < 2.5f / d1)
            {
                t -= 2.25f / d1;
                return n1 * t * t + 0.9375f;
            }
            else
            {
                t -= 2.625f / d1;
                return n1 * t * t + 0.984375f;
            }
        }
    }

    float easeInBounce(float t)
    {
        return 1.0f - easeOutBounceInternal(1.0f - t);
    }

    float easeOutBounce(float t)
    {
        return easeOutBounceInternal(t);
    }

    float easeInOutBounce(float t)
    {
        return t < 0.5f
            ? (1.0f - easeOutBounceInternal(1.0f - 2.0f * t)) / 2.0f
            : (1.0f + easeOutBounceInternal(2.0f * t - 1.0f)) / 2.0f;
    }
}
