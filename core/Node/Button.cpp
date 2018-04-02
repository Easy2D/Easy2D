#include "..\enode.h"
#include "..\emanager.h"

#define SAFE_SETTER(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }


e2d::Button::Button()
	: m_Callback(nullptr)
	, m_eBtnState(Button::NORMAL)
	, m_bEnable(true)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
{
}

e2d::Button::Button(Node * normal, Function func)
	: m_Callback(nullptr)
	, m_eBtnState(Button::NORMAL)
	, m_bEnable(true)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * selected, Function func)
	: m_Callback(nullptr)
	, m_eBtnState(Button::NORMAL)
	, m_bEnable(true)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setSelected(selected);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Function func)
	: m_Callback(nullptr)
	, m_eBtnState(Button::NORMAL)
	, m_bEnable(true)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setClickFunc(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Node * disabled, Function func)
	: m_Callback(nullptr)
	, m_eBtnState(Button::NORMAL)
	, m_bEnable(true)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setClickFunc(func);
}

bool e2d::Button::isEnable() const
{
	return m_bEnable;
}

void e2d::Button::setNormal(Node * normal)
{
	if (normal != m_pNormal)
	{
		// �Ƴ��ɵ�
		if (m_pNormal)
		{
			this->removeChild(m_pNormal);
		}
		// ����µ�
		if (normal)
		{
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		m_pNormal = normal;

		_updateVisiable();
	}
}

void e2d::Button::setMouseOver(Node * mouseover)
{
	if (mouseover != m_pNormal)
	{
		// �Ƴ��ɵ�
		if (m_pMouseover)
		{
			this->removeChild(m_pMouseover);
		}
		// ����µ�
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		m_pMouseover = mouseover;
		_updateVisiable();
	}
}

void e2d::Button::setSelected(Node * selected)
{
	if (selected != m_pNormal)
	{
		// �Ƴ��ɵ�
		if (m_pSelected)
		{
			this->removeChild(m_pSelected);
		}
		// ����µ�
		if (selected)
		{
			this->addChild(selected);
		}
		m_pSelected = selected;
		_updateVisiable();
	}
}

void e2d::Button::setDisabled(Node * disabled)
{
	if (disabled != m_pNormal)
	{
		// �Ƴ��ɵ�
		if (m_pDisabled)
		{
			this->removeChild(m_pDisabled);
		}
		// ����µ�
		if (disabled)
		{
			this->addChild(disabled);
		}
		m_pDisabled = disabled;
		_updateVisiable();
	}
}

void e2d::Button::setEnable(bool bEnable)
{
	if (m_bEnable != bEnable)
	{
		m_bEnable = bEnable;
		_updateVisiable();
	}
}

void e2d::Button::setClickFunc(Function func)
{
	WARN_IF(m_pNormal == nullptr, "Button cannot work without anything to show. Please set its normal displayed.");
	
	m_Callback = func;
}

void e2d::Button::onFixedUpdate()
{
	if (SceneManager::isTransitioning())
		return;

	if (m_bEnable && m_bVisiable && m_pNormal)
	{
		if (Input::isMouseLButtonRelease())
		{
			// ������̧��ʱ���ж���������Ƿ��ڰ�ť�ڲ�
			if (m_bIsSelected &&
				m_pNormal->isPointIn(Input::getMousePos()))
			{
				_runCallback();
			}
			// ��� m_bIsSelected Ϊ false
			m_bIsSelected = false;
		}

		if (Input::isMouseLButtonPress())
		{
			if (m_pNormal->isPointIn(Input::getMousePos()))
			{
				// ���������£���λ�ڰ�ť��ʱ����� m_bIsSelected Ϊ true
				m_bIsSelected = true;
				return;
			}
		}

		if (m_bIsSelected && Input::isMouseLButtonDown())
		{
			if (m_pNormal->isPointIn(Input::getMousePos()))
			{
				_setState(Button::SELECTED);
				return;
			}
		}
		else if (m_pNormal->isPointIn(Input::getMousePos()))
		{
			_setState(Button::MOUSEOVER);
			return;
		}

		_setState(Button::NORMAL);
	}
}

void e2d::Button::_setState(BTN_STATE state)
{
	if (m_eBtnState != state)
	{
		m_eBtnState = state;
		_updateVisiable();
	}
}

void e2d::Button::_updateVisiable()
{
	SAFE_SETTER(m_pNormal, setVisiable, false);
	SAFE_SETTER(m_pMouseover, setVisiable, false);
	SAFE_SETTER(m_pSelected, setVisiable, false);
	SAFE_SETTER(m_pDisabled, setVisiable, false);

	if (m_bEnable)
	{
		if (m_eBtnState == Button::SELECTED && m_pSelected)
		{
			m_pSelected->setVisiable(true);
		}
		else if (m_eBtnState == Button::MOUSEOVER && m_pMouseover)
		{
			m_pMouseover->setVisiable(true);
		}
		else
		{
			if (m_pNormal) m_pNormal->setVisiable(true);
		}
	}
	else
	{
		if (m_pDisabled)
		{
			m_pDisabled->setVisiable(true);
		}
		else
		{
			if (m_pNormal) m_pNormal->setVisiable(true);
		}
	}
}

void e2d::Button::_runCallback()
{
	if (m_Callback)
	{
		m_Callback();
	}
}
