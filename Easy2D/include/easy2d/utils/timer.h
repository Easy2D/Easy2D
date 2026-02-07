#pragma once

#include <easy2d/core/types.h>
#include <chrono>
#include <map>
#include <vector>
#include <memory>

namespace easy2d {

// ============================================================================
// Timer 类 - 单次/重复计时器
// ============================================================================
class Timer {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;
    using Callback = Function<void()>;

    Timer(float interval, bool repeat, Callback callback);
    
    /// 更新计时器，返回 true 如果触发了回调
    bool update(float deltaTime);
    
    /// 重置计时器
    void reset();
    
    /// 暂停计时器
    void pause();
    
    /// 恢复计时器
    void resume();
    
    /// 取消计时器（标记为无效）
    void cancel();
    
    /// 是否有效
    bool isValid() const { return valid_; }
    
    /// 是否暂停
    bool isPaused() const { return paused_; }
    
    /// 获取剩余时间（秒）
    float getRemaining() const;
    
    /// 获取唯一ID
    uint32 getId() const { return id_; }

private:
    uint32 id_;
    float interval_;
    float elapsed_;
    bool repeat_;
    bool paused_;
    bool valid_;
    Callback callback_;
    
    static uint32 nextId_;
};

// ============================================================================
// TimerManager 类 - 管理所有计时器
// ============================================================================
class TimerManager {
public:
    TimerManager() = default;
    ~TimerManager() = default;
    
    /// 创建单次计时器，返回计时器ID
    uint32 addTimer(float delay, Timer::Callback callback);
    
    /// 创建重复计时器，返回计时器ID
    uint32 addRepeatingTimer(float interval, Timer::Callback callback);
    
    /// 取消指定ID的计时器
    void cancelTimer(uint32 timerId);
    
    /// 暂停指定ID的计时器
    void pauseTimer(uint32 timerId);
    
    /// 恢复指定ID的计时器
    void resumeTimer(uint32 timerId);
    
    /// 更新所有计时器（每帧调用）
    void update(float deltaTime);
    
    /// 清除所有计时器
    void clear();
    
    /// 获取计时器数量
    size_t getTimerCount() const { return timers_.size(); }

private:
    std::map<uint32, std::unique_ptr<Timer>> timers_;
    std::vector<uint32> timersToRemove_;
};

} // namespace easy2d
