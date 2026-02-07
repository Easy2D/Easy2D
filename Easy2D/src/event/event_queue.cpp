#include <easy2d/event/event_queue.h>

namespace easy2d {

EventQueue::EventQueue() = default;

void EventQueue::push(const Event& event) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(event);
}

void EventQueue::push(Event&& event) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(event));
}

bool EventQueue::poll(Event& event) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    event = queue_.front();
    queue_.pop();
    return true;
}

bool EventQueue::peek(Event& event) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    event = queue_.front();
    return true;
}

void EventQueue::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    while (!queue_.empty()) {
        queue_.pop();
    }
}

bool EventQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

size_t EventQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

} // namespace easy2d
