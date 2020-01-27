#include <easy2d/e2dnode.h>
#include <easy2d/e2dmanager.h>

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }


easy2d::Button::Button()
	: _func(nullptr)
	, _state(ButtonState::Normal)
	, _enable(true)
	, _isSelected(false)
	, _isHover(false)
	, _isPressed(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	addListener(std::bind(&Button::updateStatus, this, std::placeholders::_1), L"按钮功能监听器");
}

easy2d::Button::Button(Node * normal, const Callback& func)
	: Button()
{
	this->setNormal(normal);
	this->setClickFunc(func);
}

easy2d::Button::Button(Node * normal, Node * selected, const Callback& func)
	: Button()
{
	this->setNormal(normal);
	this->setSelected(selected);
	this->setClickFunc(func);
}

easy2d::Button::Button(Node * normal, Node * mouseover, Node * selected, const Callback& func)
	: Button()
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setClickFunc(func);
}

easy2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Node * disabled, const Callback& func)
	: Button()
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setClickFunc(func);
}

bool easy2d::Button::isEnable() const
{
	return _enable;
}

void easy2d::Button::setNormal(Node * normal)
{
	if (normal != _normal)
	{
		// 移除旧的
		if (_normal)
		{
			this->removeChild(_normal);
		}
		// 添加新的
		if (normal)
		{
			normal->setAnchor(0, 0);
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		_normal = normal;

		_updateVisiable();
	}
}

void easy2d::Button::setMouseOver(Node * mouseover)
{
	if (mouseover != _normal)
	{
		// 移除旧的
		if (_mouseover)
		{
			this->removeChild(_mouseover);
		}
		// 添加新的
		if (mouseover)
		{
			mouseover->setAnchor(0, 0);
			this->addChild(mouseover);
		}
		_mouseover = mouseover;
		_updateVisiable();
	}
}

void easy2d::Button::setSelected(Node * selected)
{
	if (selected != _normal)
	{
		// 移除旧的
		if (_selected)
		{
			this->removeChild(_selected);
		}
		// 添加新的
		if (selected)
		{
			selected->setAnchor(0, 0);
			this->addChild(selected);
		}
		_selected = selected;
		_updateVisiable();
	}
}

void easy2d::Button::setDisabled(Node * disabled)
{
	if (disabled != _normal)
	{
		// 移除旧的
		if (_disabled)
		{
			this->removeChild(_disabled);
		}
		// 添加新的
		if (disabled)
		{
			disabled->setAnchor(0, 0);
			this->addChild(disabled);
		}
		_disabled = disabled;
		_updateVisiable();
	}
}

void easy2d::Button::setEnable(bool enable)
{
	if (_enable != enable)
	{
		_enable = enable;
		_updateVisiable();
	}
}

void easy2d::Button::setClickFunc(const Callback& func)
{
	_func = func;
}

void easy2d::Button::_setState(ButtonState state)
{
	if (_state != state)
	{
		_state = state;
		_updateVisiable();
	}
}

void easy2d::Button::_updateVisiable()
{
	SAFE_SET(_normal, setVisible, false);
	SAFE_SET(_mouseover, setVisible, false);
	SAFE_SET(_selected, setVisible, false);
	SAFE_SET(_disabled, setVisible, false);

	if (_enable)
	{
		if (_state == ButtonState::Selected && _selected)
		{
			_selected->setVisible(true);
		}
		else if (_state == ButtonState::Mouseover && _mouseover)
		{
			_mouseover->setVisible(true);
		}
		else
		{
			if (_normal) _normal->setVisible(true);
		}
	}
	else
	{
		if (_disabled)
		{
			_disabled->setVisible(true);
		}
		else
		{
			if (_normal) _normal->setVisible(true);
		}
	}
}

void easy2d::Button::_runCallback()
{
	if (_func)
	{
		_func();
	}
}

void easy2d::Button::updateStatus(Event* evt)
{
	if (SceneManager::isTransitioning())
		return;

	if (_visible && _normal)
	{
		if (evt->type == Event::Type::MouseMove)
		{
			MouseMoveEvent* mme = dynamic_cast<MouseMoveEvent*>(evt);
			if (!evt->target && containsPoint(Point{ mme->x, mme->y }))
			{
				evt->target = this;

				if (!_isHover)
				{
					// Mouseover
					_isHover = true;

					if (_enable)
					{
						_setState(ButtonState::Mouseover);
						Window::setCursor(Window::Cursor::Hand);
					}
					else
					{
						Window::setCursor(Window::Cursor::No);
					}
				}
			}
			else if (_isHover)
			{
				// Mouseout
				_isHover = false;
				_isPressed = false;

				if (_enable)
				{
					_setState(ButtonState::Normal);
				}
				Window::setCursor(Window::Cursor::Normal);
			}
		}

		if (evt->type == Event::Type::MouseDown && _isHover)
		{
			_isPressed = true;
			evt->target = this;
			if (_enable)
			{
				_setState(ButtonState::Selected);
				Window::setCursor(Window::Cursor::Hand);
			}
		}

		if (evt->type == Event::Type::MouseUp && _isPressed)
		{
			_isPressed = false;
			evt->target = this;

			if (_enable)
			{
				_runCallback();

				// 重置按钮状态
				_setState(ButtonState::Normal);
				Window::setCursor(Window::Cursor::Normal);
			}
		}
	}
}
