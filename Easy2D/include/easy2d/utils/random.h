#pragma once

#include <easy2d/core/types.h>
#include <random>

namespace easy2d {

// ============================================================================
// Random 类 - 随机数生成器
// ============================================================================
class Random {
public:
    /// 获取单例实例
    static Random& getInstance();
    
    /// 设置随机种子
    void setSeed(uint32 seed);
    
    /// 使用当前时间作为种子
    void randomize();
    
    /// 获取 [0, 1) 范围内的随机浮点数
    float getFloat();
    
    /// 获取 [min, max] 范围内的随机浮点数
    float getFloat(float min, float max);
    
    /// 获取 [0, max] 范围内的随机整数
    int getInt(int max);
    
    /// 获取 [min, max] 范围内的随机整数
    int getInt(int min, int max);
    
    /// 获取随机布尔值
    bool getBool();
    
    /// 获取随机布尔值（带概率）
    bool getBool(float probability);
    
    /// 获取指定范围内的随机角度（弧度）
    float getAngle();
    
    /// 获取 [-1, 1] 范围内的随机数（用于方向）
    float getSigned();

private:
    Random();
    ~Random() = default;
    
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;
    
    std::mt19937 generator_;
    std::uniform_real_distribution<float> floatDist_;
};

// ============================================================================
// 便捷函数
// ============================================================================

/// 获取 [0, 1) 范围内的随机浮点数
inline float randomFloat() {
    return Random::getInstance().getFloat();
}

/// 获取 [min, max] 范围内的随机浮点数
inline float randomFloat(float min, float max) {
    return Random::getInstance().getFloat(min, max);
}

/// 获取 [0, max] 范围内的随机整数
inline int randomInt(int max) {
    return Random::getInstance().getInt(max);
}

/// 获取 [min, max] 范围内的随机整数
inline int randomInt(int min, int max) {
    return Random::getInstance().getInt(min, max);
}

/// 获取随机布尔值
inline bool randomBool() {
    return Random::getInstance().getBool();
}

/// 获取随机布尔值（带概率）
inline bool randomBool(float probability) {
    return Random::getInstance().getBool(probability);
}

} // namespace easy2d
