#include "..\e2dnode.h"

e2d::ButtonToggle::ButtonToggle()
	: Button()
	, _toggle(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Node * toggleOnDisabled, Node * toggleOffDisabled, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setDisabled(toggleOnDisabled);
	this->setDisabledOff(toggleOffDisabled);
	this->setClickFunc(func);
}

bool e2d::ButtonToggle::getState() const
{
	return _toggle;
}

void e2d::ButtonToggle::setState(bool bState)
{
	if (_toggle != bState)
	{
		_toggle = bState;
		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setNormal(Node * normal)
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

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setMouseOver(Node * mouseover)
{
	if (mouseover != _mouseover)
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

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setSelected(Node * selected)
{
	if (selected != _selected)
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

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setDisabled(Node * disabled)
{
	if (disabled != _disabled)
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

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setNormalOff(Node * normal)
{
	if (normal != _normalOff)
	{
		// �Ƴ��ɵ�
		if (_normalOff)
		{
			this->removeChild(_normalOff);
		}
		// ����µ�
		if (normal)
		{
			this->addChild(normal);
		}
		_normalOff = normal;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setMouseOverOff(Node * mouseover)
{
	if (mouseover != _mouseoverOff)
	{
		// �Ƴ��ɵ�
		if (_mouseoverOff)
		{
			this->removeChild(_mouseoverOff);
		}
		// ����µ�
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		_mouseoverOff = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setSelectedOff(Node * selected)
{
	if (selected != _selectedOff)
	{
		// �Ƴ��ɵ�
		if (_selectedOff)
		{
			this->removeChild(_selectedOff);
		}
		// ����µ�
		if (selected)
		{
			this->addChild(selected);
		}
		_selectedOff = selected;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setDisabledOff(Node * disabled)
{
	if (disabled != _disabledOff)
	{
		// �Ƴ��ɵ�
		if (_disabledOff)
		{
			this->removeChild(_disabledOff);
		}
		// ����µ�
		if (disabled)
		{
			this->addChild(disabled);
		}
		_disabledOff = disabled;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::_updateState()
{
	if (_toggle)
	{
		_normal = _normal;
		_mouseover = _mouseover;
		_selected = _selected;
		_disabled = _disabled;

		if (_normalOff) _normalOff->setVisiable(false);
		if (_mouseoverOff) _mouseoverOff->setVisiable(false);
		if (_selectedOff) _selectedOff->setVisiable(false);
		if (_disabledOff) _disabledOff->setVisiable(false);
	}
	else
	{
		_normal = _normalOff;
		_mouseover = _mouseoverOff;
		_selected = _selectedOff;
		_disabled = _disabledOff;

		if (_normal) _normal->setVisiable(false);
		if (_mouseover) _mouseover->setVisiable(false);
		if (_selected) _selected->setVisiable(false);
		if (_disabled) _disabled->setVisiable(false);
	}
}

void e2d::ButtonToggle::_runCallback()
{
	_toggle = !_toggle;
	_updateState();

	if (_func)
	{
		_func();
	}
}
