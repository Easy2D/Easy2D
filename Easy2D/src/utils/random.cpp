#include <easy2d/utils/random.h>
#include <chrono>

namespace easy2d {

Random::Random()
    : floatDist_(0.0f, 1.0f)
{
    // 使用当前时间作为默认种子
    randomize();
}

Random& Random::getInstance() {
    static Random instance;
    return instance;
}

void Random::setSeed(uint32 seed) {
    generator_.seed(seed);
}

void Random::randomize() {
    auto now = std::chrono::high_resolution_clock::now();
    auto time = now.time_since_epoch().count();
    generator_.seed(static_cast<uint32>(time));
}

float Random::getFloat() {
    return floatDist_(generator_);
}

float Random::getFloat(float min, float max) {
    if (min >= max) {
        return min;
    }
    return min + floatDist_(generator_) * (max - min);
}

int Random::getInt(int max) {
    if (max <= 0) {
        return 0;
    }
    std::uniform_int_distribution<int> dist(0, max);
    return dist(generator_);
}

int Random::getInt(int min, int max) {
    if (min >= max) {
        return min;
    }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator_);
}

bool Random::getBool() {
    return floatDist_(generator_) >= 0.5f;
}

bool Random::getBool(float probability) {
    if (probability <= 0.0f) {
        return false;
    }
    if (probability >= 1.0f) {
        return true;
    }
    return floatDist_(generator_) < probability;
}

float Random::getAngle() {
    static const float TWO_PI = 6.28318530718f;
    return floatDist_(generator_) * TWO_PI;
}

float Random::getSigned() {
    return floatDist_(generator_) * 2.0f - 1.0f;
}

} // namespace easy2d
