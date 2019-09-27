#include <e2dcommon.h>

easy2d::Event::Event(Type type)
	: type(type)
	, target(nullptr)
{
}

easy2d::Event::~Event()
{
}

easy2d::MouseMoveEvent::MouseMoveEvent(float x, float y)
	: Event(Event::Type::MouseMove)
	, x(x)
	, y(y)
{
}

easy2d::MouseDownEvent::MouseDownEvent(float x, float y, MouseCode::Value btn)
	: Event(Event::Type::MouseDown)
	, x(x)
	, y(y)
	, button(btn)
{
}

easy2d::MouseUpEvent::MouseUpEvent(float x, float y, MouseCode::Value btn)
	: Event(Event::Type::MouseUp)
	, x(x)
	, y(y)
	, button(btn)
{
}

easy2d::MouseWheelEvent::MouseWheelEvent(float x, float y, float delta)
	: Event(Event::Type::MouseWheel)
	, x(x)
	, y(y)
	, delta(delta)
{
}

easy2d::KeyDownEvent::KeyDownEvent(KeyCode::Value key, int count)
	: Event(Event::Type::KeyDown)
	, key(key)
	, count(count)
{
}

easy2d::KeyUpEvent::KeyUpEvent(KeyCode::Value key, int count)
	: Event(Event::Type::KeyUp)
	, key(key)
	, count(count)
{
}
