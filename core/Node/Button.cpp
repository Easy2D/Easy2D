#include "..\e2dnode.h"
#include "..\e2dmanager.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }


e2d::Button::Button()
	: _func(nullptr)
	, _state(ButtonState::Normal)
	, _enable(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
}

e2d::Button::Button(Node * normal, const Function& func)
	: _func(nullptr)
	, _state(ButtonState::Normal)
	, _enable(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->setNormal(normal);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * selected, const Function& func)
	: _func(nullptr)
	, _state(ButtonState::Normal)
	, _enable(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->setNormal(normal);
	this->setSelected(selected);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, const Function& func)
	: _func(nullptr)
	, _state(ButtonState::Normal)
	, _enable(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Node * disabled, const Function& func)
	: _func(nullptr)
	, _state(ButtonState::Normal)
	, _enable(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setClickFunc(func);
}

bool e2d::Button::isEnable() const
{
	return _enable;
}

void e2d::Button::setNormal(Node * normal)
{
	if (normal != _normal)
	{
		// �Ƴ��ɵ�
		if (_normal)
		{
			this->removeChild(_normal);
		}
		// ����µ�
		if (normal)
		{
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		_normal = normal;

		_updateVisiable();
	}
}

void e2d::Button::setMouseOver(Node * mouseover)
{
	if (mouseover != _normal)
	{
		// �Ƴ��ɵ�
		if (_mouseover)
		{
			this->removeChild(_mouseover);
		}
		// ����µ�
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		_mouseover = mouseover;
		_updateVisiable();
	}
}

void e2d::Button::setSelected(Node * selected)
{
	if (selected != _normal)
	{
		// �Ƴ��ɵ�
		if (_selected)
		{
			this->removeChild(_selected);
		}
		// ����µ�
		if (selected)
		{
			this->addChild(selected);
		}
		_selected = selected;
		_updateVisiable();
	}
}

void e2d::Button::setDisabled(Node * disabled)
{
	if (disabled != _normal)
	{
		// �Ƴ��ɵ�
		if (_disabled)
		{
			this->removeChild(_disabled);
		}
		// ����µ�
		if (disabled)
		{
			this->addChild(disabled);
		}
		_disabled = disabled;
		_updateVisiable();
	}
}

void e2d::Button::setEnable(bool enable)
{
	if (_enable != enable)
	{
		_enable = enable;
		_updateVisiable();
	}
}

void e2d::Button::setClickFunc(const Function& func)
{
	WARN_IF(_normal == nullptr, "Button cannot work without anything to show. Please set its normal displayed.");
	
	_func = func;
}

void e2d::Button::_fixedUpdate()
{
	if (SceneManager::isTransitioning())
		return;

	if (_enable && _visiable && _normal)
	{
		if (Input::isRelease(Input::Mouse::Left))
		{
			// ������̧��ʱ���ж���������Ƿ��ڰ�ť�ڲ�
			if (_isSelected &&
				_normal->containsPoint(Input::getMousePos()))
			{
				_runCallback();
			}
			// ��� _isSelected Ϊ false
			_isSelected = false;
		}

		if (Input::isPress(Input::Mouse::Left))
		{
			if (_normal->containsPoint(Input::getMousePos()))
			{
				// ���������£���λ�ڰ�ť��ʱ����� _isSelected Ϊ true
				_isSelected = true;
				return;
			}
		}

		if (_isSelected && Input::isDown(Input::Mouse::Left))
		{
			if (_normal->containsPoint(Input::getMousePos()))
			{
				_setState(ButtonState::Selected);
				Window::setCursor(Window::Cursor::Hand);
				return;
			}
		}
		else if (_normal->containsPoint(Input::getMousePos()))
		{
			_setState(ButtonState::Mouseover);
			Window::setCursor(Window::Cursor::Hand);
			return;
		}

		_setState(ButtonState::Normal);
	}

	if (_visiable && !_enable && _normal && _normal->containsPoint(Input::getMousePos()))
	{
		Window::setCursor(Window::Cursor::No);
	}
}

void e2d::Button::_setState(ButtonState state)
{
	if (_state != state)
	{
		_state = state;
		_updateVisiable();
	}
}

void e2d::Button::_updateVisiable()
{
	SAFE_SET(_normal, setVisiable, false);
	SAFE_SET(_mouseover, setVisiable, false);
	SAFE_SET(_selected, setVisiable, false);
	SAFE_SET(_disabled, setVisiable, false);

	if (_enable)
	{
		if (_state == ButtonState::Selected && _selected)
		{
			_selected->setVisiable(true);
		}
		else if (_state == ButtonState::Mouseover && _mouseover)
		{
			_mouseover->setVisiable(true);
		}
		else
		{
			if (_normal) _normal->setVisiable(true);
		}
	}
	else
	{
		if (_disabled)
		{
			_disabled->setVisiable(true);
		}
		else
		{
			if (_normal) _normal->setVisiable(true);
		}
	}
}

void e2d::Button::_runCallback()
{
	if (_func)
	{
		_func();
	}
}
