#include <easy2d/e2dnode.h>

easy2d::ToggleButton::ToggleButton()
	: Button()
	, _toggle(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
}

easy2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, const Callback& func)
	: Button()
	, _toggle(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setClickFunc(func);
}

easy2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, const Callback& func)
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

easy2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, const Callback& func)
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

easy2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Node * toggleOnDisabled, Node * toggleOffDisabled, const Callback& func)
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

bool easy2d::ToggleButton::getState() const
{
	return _toggle;
}

void easy2d::ToggleButton::setState(bool bState)
{
	if (_toggle != bState)
	{
		_toggle = bState;
		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setNormal(Node * normal)
{
	if (normal != _normalOn)
	{
		// �Ƴ��ɵ�
		if (_normalOn)
		{
			this->removeChild(_normalOn);
		}
		// ����µ�
		if (normal)
		{
			normal->setAnchor(0, 0);
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		_normalOn = normal;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setMouseOver(Node * mouseover)
{
	if (mouseover != _mouseoverOn)
	{
		// �Ƴ��ɵ�
		if (_mouseoverOn)
		{
			this->removeChild(_mouseoverOn);
		}
		// ����µ�
		if (mouseover)
		{
			mouseover->setAnchor(0, 0);
			this->addChild(mouseover);
		}
		_mouseoverOn = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setSelected(Node * selected)
{
	if (selected != _selectedOn)
	{
		// �Ƴ��ɵ�
		if (_selectedOn)
		{
			this->removeChild(_selectedOn);
		}
		// ����µ�
		if (selected)
		{
			selected->setAnchor(0, 0);
			this->addChild(selected);
		}
		_selectedOn = selected;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setDisabled(Node * disabled)
{
	if (disabled != _disabledOn)
	{
		// �Ƴ��ɵ�
		if (_disabledOn)
		{
			this->removeChild(_disabledOn);
		}
		// ����µ�
		if (disabled)
		{
			disabled->setAnchor(0, 0);
			this->addChild(disabled);
		}
		_disabledOn = disabled;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setNormalOff(Node * normal)
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
			normal->setAnchor(0, 0);
			this->addChild(normal);
		}
		_normalOff = normal;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setMouseOverOff(Node * mouseover)
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
			mouseover->setAnchor(0, 0);
			this->addChild(mouseover);
		}
		_mouseoverOff = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setSelectedOff(Node * selected)
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
			selected->setAnchor(0, 0);
			this->addChild(selected);
		}
		_selectedOff = selected;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::setDisabledOff(Node * disabled)
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
			disabled->setAnchor(0, 0);
			this->addChild(disabled);
		}
		_disabledOff = disabled;

		_updateState();
		_updateVisiable();
	}
}

void easy2d::ToggleButton::_updateState()
{
	if (_toggle)
	{
		_normal = _normalOn;
		_mouseover = _mouseoverOn;
		_selected = _selectedOn;
		_disabled = _disabledOn;

		if (_normalOff) _normalOff->setVisible(false);
		if (_mouseoverOff) _mouseoverOff->setVisible(false);
		if (_selectedOff) _selectedOff->setVisible(false);
		if (_disabledOff) _disabledOff->setVisible(false);
	}
	else
	{
		_normal = _normalOff;
		_mouseover = _mouseoverOff;
		_selected = _selectedOff;
		_disabled = _disabledOff;

		if (_normalOn) _normalOn->setVisible(false);
		if (_mouseoverOn) _mouseoverOn->setVisible(false);
		if (_selectedOn) _selectedOn->setVisible(false);
		if (_disabledOn) _disabledOn->setVisible(false);
	}
}

void easy2d::ToggleButton::_runCallback()
{
	_toggle = !_toggle;
	_updateState();

	if (_func)
	{
		_func();
	}
}
