#include <easy2d/event/event.h>

namespace easy2d {

Event Event::createWindowResize(int width, int height) {
    Event event;
    event.type = EventType::WindowResize;
    event.data = WindowResizeEvent{width, height};
    return event;
}

Event Event::createWindowClose() {
    Event event;
    event.type = EventType::WindowClose;
    return event;
}

Event Event::createKeyPress(int keyCode, int scancode, int mods) {
    Event event;
    event.type = EventType::KeyPressed;
    event.data = KeyEvent{keyCode, scancode, mods};
    return event;
}

Event Event::createKeyRelease(int keyCode, int scancode, int mods) {
    Event event;
    event.type = EventType::KeyReleased;
    event.data = KeyEvent{keyCode, scancode, mods};
    return event;
}

Event Event::createMouseButtonPress(int button, int mods, const Vec2& pos) {
    Event event;
    event.type = EventType::MouseButtonPressed;
    event.data = MouseButtonEvent{button, mods, pos};
    return event;
}

Event Event::createMouseButtonRelease(int button, int mods, const Vec2& pos) {
    Event event;
    event.type = EventType::MouseButtonReleased;
    event.data = MouseButtonEvent{button, mods, pos};
    return event;
}

Event Event::createMouseMove(const Vec2& pos, const Vec2& delta) {
    Event event;
    event.type = EventType::MouseMoved;
    event.data = MouseMoveEvent{pos, delta};
    return event;
}

Event Event::createMouseScroll(const Vec2& offset, const Vec2& pos) {
    Event event;
    event.type = EventType::MouseScrolled;
    event.data = MouseScrollEvent{offset, pos};
    return event;
}

} // namespace easy2d
