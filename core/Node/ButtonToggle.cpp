#include "..\e2dnode.h"

e2d::ButtonToggle::ButtonToggle()
	: Button()
	, _bState(true)
	, _pNormalOn(nullptr)
	, _pMouseoverOn(nullptr)
	, _pSelectedOn(nullptr)
	, _pDisabledOn(nullptr)
	, _pNormalOff(nullptr)
	, _pMouseoverOff(nullptr)
	, _pSelectedOff(nullptr)
	, _pDisabledOff(nullptr)
{
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, const Function& func)
	: Button()
	, _bState(true)
	, _pNormalOn(nullptr)
	, _pMouseoverOn(nullptr)
	, _pSelectedOn(nullptr)
	, _pDisabledOn(nullptr)
	, _pNormalOff(nullptr)
	, _pMouseoverOff(nullptr)
	, _pSelectedOff(nullptr)
	, _pDisabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _bState(true)
	, _pNormalOn(nullptr)
	, _pMouseoverOn(nullptr)
	, _pSelectedOn(nullptr)
	, _pDisabledOn(nullptr)
	, _pNormalOff(nullptr)
	, _pMouseoverOff(nullptr)
	, _pSelectedOff(nullptr)
	, _pDisabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _bState(true)
	, _pNormalOn(nullptr)
	, _pMouseoverOn(nullptr)
	, _pSelectedOn(nullptr)
	, _pDisabledOn(nullptr)
	, _pNormalOff(nullptr)
	, _pMouseoverOff(nullptr)
	, _pSelectedOff(nullptr)
	, _pDisabledOff(nullptr)
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
	, _bState(true)
	, _pNormalOn(nullptr)
	, _pMouseoverOn(nullptr)
	, _pSelectedOn(nullptr)
	, _pDisabledOn(nullptr)
	, _pNormalOff(nullptr)
	, _pMouseoverOff(nullptr)
	, _pSelectedOff(nullptr)
	, _pDisabledOff(nullptr)
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
	return _bState;
}

void e2d::ButtonToggle::setState(bool bState)
{
	if (_bState != bState)
	{
		_bState = bState;
		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setNormal(Node * normal)
{
	if (normal != _pNormalOn)
	{
		// �Ƴ��ɵ�
		if (_pNormalOn)
		{
			this->removeChild(_pNormalOn);
		}
		// ����µ�
		if (normal)
		{
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		_pNormalOn = normal;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setMouseOver(Node * mouseover)
{
	if (mouseover != _pMouseoverOn)
	{
		// �Ƴ��ɵ�
		if (_pMouseoverOn)
		{
			this->removeChild(_pMouseoverOn);
		}
		// ����µ�
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		_pMouseoverOn = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setSelected(Node * selected)
{
	if (selected != _pSelectedOn)
	{
		// �Ƴ��ɵ�
		if (_pSelectedOn)
		{
			this->removeChild(_pSelectedOn);
		}
		// ����µ�
		if (selected)
		{
			this->addChild(selected);
		}
		_pSelectedOn = selected;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setDisabled(Node * disabled)
{
	if (disabled != _pDisabledOn)
	{
		// �Ƴ��ɵ�
		if (_pDisabledOn)
		{
			this->removeChild(_pDisabledOn);
		}
		// ����µ�
		if (disabled)
		{
			this->addChild(disabled);
		}
		_pDisabledOn = disabled;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setNormalOff(Node * normal)
{
	if (normal != _pNormalOff)
	{
		// �Ƴ��ɵ�
		if (_pNormalOff)
		{
			this->removeChild(_pNormalOff);
		}
		// ����µ�
		if (normal)
		{
			this->addChild(normal);
		}
		_pNormalOff = normal;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setMouseOverOff(Node * mouseover)
{
	if (mouseover != _pMouseoverOff)
	{
		// �Ƴ��ɵ�
		if (_pMouseoverOff)
		{
			this->removeChild(_pMouseoverOff);
		}
		// ����µ�
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		_pMouseoverOff = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setSelectedOff(Node * selected)
{
	if (selected != _pSelectedOff)
	{
		// �Ƴ��ɵ�
		if (_pSelectedOff)
		{
			this->removeChild(_pSelectedOff);
		}
		// ����µ�
		if (selected)
		{
			this->addChild(selected);
		}
		_pSelectedOff = selected;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setDisabledOff(Node * disabled)
{
	if (disabled != _pDisabledOff)
	{
		// �Ƴ��ɵ�
		if (_pDisabledOff)
		{
			this->removeChild(_pDisabledOff);
		}
		// ����µ�
		if (disabled)
		{
			this->addChild(disabled);
		}
		_pDisabledOff = disabled;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::_updateState()
{
	if (_bState)
	{
		_pNormal = _pNormalOn;
		_pMouseover = _pMouseoverOn;
		_pSelected = _pSelectedOn;
		_pDisabled = _pDisabledOn;

		if (_pNormalOff) _pNormalOff->setVisiable(false);
		if (_pMouseoverOff) _pMouseoverOff->setVisiable(false);
		if (_pSelectedOff) _pSelectedOff->setVisiable(false);
		if (_pDisabledOff) _pDisabledOff->setVisiable(false);
	}
	else
	{
		_pNormal = _pNormalOff;
		_pMouseover = _pMouseoverOff;
		_pSelected = _pSelectedOff;
		_pDisabled = _pDisabledOff;

		if (_pNormalOn) _pNormalOn->setVisiable(false);
		if (_pMouseoverOn) _pMouseoverOn->setVisiable(false);
		if (_pSelectedOn) _pSelectedOn->setVisiable(false);
		if (_pDisabledOn) _pDisabledOn->setVisiable(false);
	}
}

void e2d::ButtonToggle::_runCallback()
{
	_bState = !_bState;
	_updateState();

	if (_func)
	{
		_func();
	}
}
