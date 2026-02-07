#include <easy2d/utils/timer.h>
#include <algorithm>

namespace easy2d {

uint32 Timer::nextId_ = 1;

Timer::Timer(float interval, bool repeat, Callback callback)
    : interval_(interval)
    , elapsed_(0.0f)
    , repeat_(repeat)
    , paused_(false)
    , valid_(true)
    , callback_(std::move(callback))
{
    id_ = nextId_++;
}

bool Timer::update(float deltaTime) {
    if (!valid_ || paused_) {
        return false;
    }
    
    elapsed_ += deltaTime;
    
    if (elapsed_ >= interval_) {
        if (callback_) {
            callback_();
        }
        
        if (repeat_) {
            elapsed_ = 0.0f;
            return true;
        } else {
            valid_ = false;
            return true;
        }
    }
    
    return false;
}

void Timer::reset() {
    elapsed_ = 0.0f;
    valid_ = true;
    paused_ = false;
}

void Timer::pause() {
    paused_ = true;
}

void Timer::resume() {
    paused_ = false;
}

void Timer::cancel() {
    valid_ = false;
}

float Timer::getRemaining() const {
    if (!valid_ || paused_) {
        return 0.0f;
    }
    return std::max(0.0f, interval_ - elapsed_);
}

// ============================================================================
// TimerManager 实现
// ============================================================================

uint32 TimerManager::addTimer(float delay, Timer::Callback callback) {
    auto timer = std::make_unique<Timer>(delay, false, std::move(callback));
    uint32 id = timer->getId();
    timers_.emplace(id, std::move(timer));
    return id;
}

uint32 TimerManager::addRepeatingTimer(float interval, Timer::Callback callback) {
    auto timer = std::make_unique<Timer>(interval, true, std::move(callback));
    uint32 id = timer->getId();
    timers_.emplace(id, std::move(timer));
    return id;
}

void TimerManager::cancelTimer(uint32 timerId) {
    auto it = timers_.find(timerId);
    if (it != timers_.end()) {
        it->second->cancel();
        timersToRemove_.push_back(timerId);
    }
}

void TimerManager::pauseTimer(uint32 timerId) {
    auto it = timers_.find(timerId);
    if (it != timers_.end()) {
        it->second->pause();
    }
}

void TimerManager::resumeTimer(uint32 timerId) {
    auto it = timers_.find(timerId);
    if (it != timers_.end()) {
        it->second->resume();
    }
}

void TimerManager::update(float deltaTime) {
    timersToRemove_.clear();
    
    for (auto& [id, timer] : timers_) {
        timer->update(deltaTime);
        if (!timer->isValid()) {
            timersToRemove_.push_back(id);
        }
    }
    
    for (uint32 id : timersToRemove_) {
        timers_.erase(id);
    }
}

void TimerManager::clear() {
    timers_.clear();
    timersToRemove_.clear();
}

} // namespace easy2d
