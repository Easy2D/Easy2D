#include <easy2d/e2dlistener.h>
#include <easy2d/e2dmanager.h>
#include <easy2d/e2dnode.h>

easy2d::ButtonListenerBase::ButtonListenerBase()
	: _isHover(false)
	, _isPressed(false)
{
}

void easy2d::ButtonListenerBase::handle(Node* target, Event* evt)
{
	if (evt->target)
	{
		// 已有目标，不再处理
		return;
	}

	if (target->isVisible())
	{
		if (evt->type == Event::Type::MouseMove)
		{
			MouseMoveEvent* mme = dynamic_cast<MouseMoveEvent*>(evt);
			if (!evt->target && target->containsPoint(Point{ mme->x, mme->y }))
			{
				evt->target = this;

				if (!_isHover)
				{
					// Mouseover
					_isHover = true;
					handle(target, ButtonEvent::MouseEntered);
					Window::setCursor(Window::Cursor::Hand);
				}
			}
			else if (_isHover)
			{
				// Mouseout
				_isHover = false;
				_isPressed = false;

				handle(target, ButtonEvent::MouseExited);
				Window::setCursor(Window::Cursor::Normal);
			}
		}

		if (evt->type == Event::Type::MouseDown && _isHover)
		{
			_isPressed = true;
			evt->target = this;
			handle(target, ButtonEvent::Pressed);
			Window::setCursor(Window::Cursor::Hand);
		}

		if (evt->type == Event::Type::MouseUp && _isPressed)
		{
			_isPressed = false;
			evt->target = this;

			handle(target, ButtonEvent::Clicked);

			// 重置指针状态
			Window::setCursor(Window::Cursor::Normal);
		}
	}
}

// ButtonListener

easy2d::ButtonListener::ButtonListener()
	: ButtonListenerBase()
	, _callback()
{
}

easy2d::ButtonListener::ButtonListener(const Callback& func)
	: ButtonListenerBase()
	, _callback(func)
{
}

void easy2d::ButtonListener::setCallback(const Callback& func)
{
	_callback = func;
}

void easy2d::ButtonListener::handle(Node* target, ButtonEvent evt)
{
	if (_callback)
	{
		_callback(evt);
	}
}

// ToggleButtonListener

easy2d::ToggleButtonListener::ToggleButtonListener()
	: ButtonListenerBase()
	, _state(false)
	, _callback()
{
}

easy2d::ToggleButtonListener::ToggleButtonListener(bool toggle, const Callback& func)
	: ButtonListenerBase()
	, _state(toggle)
	, _callback(func)
{
}

void easy2d::ToggleButtonListener::setCallback(const Callback& func)
{
	_callback = func;
}

bool easy2d::ToggleButtonListener::getState() const
{
	return _state;
}

void easy2d::ToggleButtonListener::setState(bool state)
{
	_state = state;
}

void easy2d::ToggleButtonListener::toggle()
{
	_state = !_state;
}

void easy2d::ToggleButtonListener::handle(Node* target, ButtonEvent evt)
{
	if (evt == ButtonEvent::Clicked)
	{
		toggle();
	}

	if (_callback)
	{
		_callback(evt, _state);
	}
}
