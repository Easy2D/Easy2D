#pragma once

#include <easy2d/core/types.h>
#include <easy2d/event/event.h>
#include <functional>
#include <unordered_map>
#include <vector>

namespace easy2d {

// ============================================================================
// 事件监听器 ID
// ============================================================================
using ListenerId = uint64_t;

// ============================================================================
// 事件分发器
// ============================================================================
class EventDispatcher {
public:
    using EventCallback = std::function<void(Event&)>;

    EventDispatcher();
    ~EventDispatcher() = default;

    // 添加监听器
    ListenerId addListener(EventType type, EventCallback callback);
    
    // 移除监听器
    void removeListener(ListenerId id);
    void removeAllListeners(EventType type);
    void removeAllListeners();

    // 分发事件
    void dispatch(Event& event);
    void dispatch(const Event& event);

    // 处理事件队列
    void processQueue(class EventQueue& queue);

    // 统计
    size_t getListenerCount(EventType type) const;
    size_t getTotalListenerCount() const;

private:
    struct Listener {
        ListenerId id;
        EventType type;
        EventCallback callback;
    };

    std::unordered_map<EventType, std::vector<Listener>> listeners_;
    ListenerId nextId_;
};

} // namespace easy2d
