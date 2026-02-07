#include <easy2d/event/event_dispatcher.h>
#include <easy2d/event/event_queue.h>

namespace easy2d {

EventDispatcher::EventDispatcher() : nextId_(1) {
}

ListenerId EventDispatcher::addListener(EventType type, EventCallback callback) {
    ListenerId id = nextId_++;
    listeners_[type].push_back({id, type, callback});
    return id;
}

void EventDispatcher::removeListener(ListenerId id) {
    for (auto& [type, listeners] : listeners_) {
        auto it = std::remove_if(listeners.begin(), listeners.end(),
            [id](const Listener& l) { return l.id == id; });
        if (it != listeners.end()) {
            listeners.erase(it, listeners.end());
            return;
        }
    }
}

void EventDispatcher::removeAllListeners(EventType type) {
    listeners_.erase(type);
}

void EventDispatcher::removeAllListeners() {
    listeners_.clear();
}

void EventDispatcher::dispatch(Event& event) {
    auto it = listeners_.find(event.type);
    if (it != listeners_.end()) {
        for (auto& listener : it->second) {
            if (event.handled) break;
            listener.callback(event);
        }
    }
}

void EventDispatcher::dispatch(const Event& event) {
    Event mutableEvent = event;
    dispatch(mutableEvent);
}

void EventDispatcher::processQueue(EventQueue& queue) {
    Event event;
    while (queue.poll(event)) {
        dispatch(event);
    }
}

size_t EventDispatcher::getListenerCount(EventType type) const {
    auto it = listeners_.find(type);
    return (it != listeners_.end()) ? it->second.size() : 0;
}

size_t EventDispatcher::getTotalListenerCount() const {
    size_t count = 0;
    for (const auto& [type, listeners] : listeners_) {
        count += listeners.size();
    }
    return count;
}

} // namespace easy2d
