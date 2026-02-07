#pragma once

#include <easy2d/core/types.h>
#include <easy2d/event/event.h>
#include <queue>
#include <mutex>

namespace easy2d {

// ============================================================================
// 事件队列 - 线程安全的事件队列
// ============================================================================
class EventQueue {
public:
    EventQueue();
    ~EventQueue() = default;

    // 添加事件到队列
    void push(const Event& event);
    void push(Event&& event);
    
    // 从队列取出事件
    bool poll(Event& event);
    
    // 查看队列头部事件（不移除）
    bool peek(Event& event) const;
    
    // 清空队列
    void clear();
    
    // 队列状态
    bool empty() const;
    size_t size() const;

private:
    std::queue<Event> queue_;
    mutable std::mutex mutex_;
};

} // namespace easy2d
