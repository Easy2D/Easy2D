#include "..\e2dnode.h"
#include "..\e2dmanager.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }


e2d::Button::Button()
	: _Callback(nullptr)
	, _eBtnState(ButtonState::NORMAL)
	, _bEnable(true)
	, _bIsSelected(false)
	, _pNormal(nullptr)
	, _pMouseover(nullptr)
	, _pSelected(nullptr)
	, _pDisabled(nullptr)
{
}

e2d::Button::Button(Node * normal, const Function& func)
	: _Callback(nullptr)
	, _eBtnState(ButtonState::NORMAL)
	, _bEnable(true)
	, _bIsSelected(false)
	, _pNormal(nullptr)
	, _pMouseover(nullptr)
	, _pSelected(nullptr)
	, _pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * selected, const Function& func)
	: _Callback(nullptr)
	, _eBtnState(ButtonState::NORMAL)
	, _bEnable(true)
	, _bIsSelected(false)
	, _pNormal(nullptr)
	, _pMouseover(nullptr)
	, _pSelected(nullptr)
	, _pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setSelected(selected);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, const Function& func)
	: _Callback(nullptr)
	, _eBtnState(ButtonState::NORMAL)
	, _bEnable(true)
	, _bIsSelected(false)
	, _pNormal(nullptr)
	, _pMouseover(nullptr)
	, _pSelected(nullptr)
	, _pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Node * disabled, const Function& func)
	: _Callback(nullptr)
	, _eBtnState(ButtonState::NORMAL)
	, _bEnable(true)
	, _bIsSelected(false)
	, _pNormal(nullptr)
	, _pMouseover(nullptr)
	, _pSelected(nullptr)
	, _pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setClickFunc(func);
}

bool e2d::Button::isEnable() const
{
	return _bEnable;
}

void e2d::Button::setNormal(Node * normal)
{
	if (normal != _pNormal)
	{
		// �Ƴ��ɵ�
		if (_pNormal)
		{
			this->removeChild(_pNormal);
		}
		// ����µ�
		if (normal)
		{
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		_pNormal = normal;

		_updateVisiable();
	}
}

void e2d::Button::setMouseOver(Node * mouseover)
{
	if (mouseover != _pNormal)
	{
		// �Ƴ��ɵ�
		if (_pMouseover)
		{
			this->removeChild(_pMouseover);
		}
		// ����µ�
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		_pMouseover = mouseover;
		_updateVisiable();
	}
}

void e2d::Button::setSelected(Node * selected)
{
	if (selected != _pNormal)
	{
		// �Ƴ��ɵ�
		if (_pSelected)
		{
			this->removeChild(_pSelected);
		}
		// ����µ�
		if (selected)
		{
			this->addChild(selected);
		}
		_pSelected = selected;
		_updateVisiable();
	}
}

void e2d::Button::setDisabled(Node * disabled)
{
	if (disabled != _pNormal)
	{
		// �Ƴ��ɵ�
		if (_pDisabled)
		{
			this->removeChild(_pDisabled);
		}
		// ����µ�
		if (disabled)
		{
			this->addChild(disabled);
		}
		_pDisabled = disabled;
		_updateVisiable();
	}
}

void e2d::Button::setEnable(bool enable)
{
	if (_bEnable != enable)
	{
		_bEnable = enable;
		_updateVisiable();
	}
}

void e2d::Button::setClickFunc(const Function& func)
{
	WARN_IF(_pNormal == nullptr, "Button cannot work without anything to show. Please set its normal displayed.");
	
	_Callback = func;
}

void e2d::Button::onFixedUpdate()
{
	if (SceneManager::isTransitioning())
		return;

	if (_bEnable && _bVisiable && _pNormal)
	{
		if (Input::isMouseLButtonRelease())
		{
			// ������̧��ʱ���ж���������Ƿ��ڰ�ť�ڲ�
			if (_bIsSelected &&
				_pNormal->isPointIn(Input::getMousePos()))
			{
				_runCallback();
			}
			// ��� _bIsSelected Ϊ false
			_bIsSelected = false;
		}

		if (Input::isMouseLButtonPress())
		{
			if (_pNormal->isPointIn(Input::getMousePos()))
			{
				// ���������£���λ�ڰ�ť��ʱ����� _bIsSelected Ϊ true
				_bIsSelected = true;
				return;
			}
		}

		if (_bIsSelected && Input::isMouseLButtonDown())
		{
			if (_pNormal->isPointIn(Input::getMousePos()))
			{
				_setState(ButtonState::SELECTED);
				Window::setCursor(Cursor::HAND);
				return;
			}
		}
		else if (_pNormal->isPointIn(Input::getMousePos()))
		{
			_setState(ButtonState::MOUSEOVER);
			Window::setCursor(Cursor::HAND);
			return;
		}

		_setState(ButtonState::NORMAL);
	}

	if (_bVisiable && !_bEnable && _pNormal && _pNormal->isPointIn(Input::getMousePos()))
	{
		Window::setCursor(Cursor::NO);
	}
}

void e2d::Button::_setState(ButtonState state)
{
	if (_eBtnState != state)
	{
		_eBtnState = state;
		_updateVisiable();
	}
}

void e2d::Button::_updateVisiable()
{
	SAFE_SET(_pNormal, setVisiable, false);
	SAFE_SET(_pMouseover, setVisiable, false);
	SAFE_SET(_pSelected, setVisiable, false);
	SAFE_SET(_pDisabled, setVisiable, false);

	if (_bEnable)
	{
		if (_eBtnState == ButtonState::SELECTED && _pSelected)
		{
			_pSelected->setVisiable(true);
		}
		else if (_eBtnState == ButtonState::MOUSEOVER && _pMouseover)
		{
			_pMouseover->setVisiable(true);
		}
		else
		{
			if (_pNormal) _pNormal->setVisiable(true);
		}
	}
	else
	{
		if (_pDisabled)
		{
			_pDisabled->setVisiable(true);
		}
		else
		{
			if (_pNormal) _pNormal->setVisiable(true);
		}
	}
}

void e2d::Button::_runCallback()
{
	if (_Callback)
	{
		_Callback();
	}
}
