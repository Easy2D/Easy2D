#include "..\enodes.h"
#include "..\elisteners.h"
#include "..\emanagers.h"
#include "..\Win\winbase.h"

e2d::EButton::EButton()
	: m_Callback((const BUTTON_CLICK_CALLBACK &)nullptr)
	, m_eStatus(STATUS::NORMAL)
	, m_bEnable(true)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
	, m_pListener(nullptr)
{
}

e2d::EButton::EButton(ENode * normal, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setCallback(callback);
}

e2d::EButton::EButton(ENode * normal, ENode * selected, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setSelected(selected);
	this->setCallback(callback);
}

e2d::EButton::EButton(ENode * normal, ENode * mouseover, ENode * selected, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setCallback(callback);
}

e2d::EButton::EButton(ENode * normal, ENode * mouseover, ENode * selected, ENode * disabled, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setCallback(callback);
}

bool e2d::EButton::isEnable() const
{
	return m_bEnable;
}

void e2d::EButton::setNormal(ENode * normal)
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
		}
		m_pNormal = normal;
		_updateVisiable();
	}
}

void e2d::EButton::setMouseOver(ENode * mouseover)
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

void e2d::EButton::setSelected(ENode * selected)
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

void e2d::EButton::setDisabled(ENode * disabled)
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

void e2d::EButton::setEnable(bool bEnable)
{
	if (m_bEnable != bEnable)
	{
		m_bEnable = bEnable;
		_updateStatus();
	}
}

void e2d::EButton::setCallback(const BUTTON_CLICK_CALLBACK & callback)
{
	WARN_IF(m_pNormal == nullptr, "EButton cannot work without something to show. Please set its normal displayed.");
	
	m_Callback = callback;

	if (m_pListener == nullptr)
	{
		m_pListener = new EListenerMouse(std::bind(&EButton::_updateStatus, this));
		m_pListener->setAlwaysWorking(true);
		EMsgManager::bindListener(m_pListener, this);
	}
}

void e2d::EButton::_setStatus(STATUS status)
{
	if (m_eStatus != status)
	{
		m_eStatus = status;
		_updateVisiable();
	}
}

void e2d::EButton::_updateVisiable()
{
	if (m_pNormal) m_pNormal->setVisiable(false);
	if (m_pMouseover) m_pMouseover->setVisiable(false);
	if (m_pSelected) m_pSelected->setVisiable(false);
	if (m_pDisabled) m_pDisabled->setVisiable(false);

	if (m_bEnable)
	{
		if (m_eStatus == STATUS::SELECTED && m_pSelected)
		{
			m_pSelected->setVisiable(true);
		}
		else if (m_eStatus == STATUS::MOUSEOVER && m_pMouseover)
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

void e2d::EButton::_updateStatus()
{
	if (m_bEnable && m_pNormal)
	{
		ENode * pMouseover = m_pMouseover ? m_pMouseover : m_pNormal;
		ENode * pSelected = m_pSelected ? m_pSelected : m_pNormal;

		if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_UP)
		{
			// ������̧��ʱ���ж���������Ƿ��ڰ�ť�ڲ�
			if (m_bIsSelected &&
				pSelected->isPointIn(EMouseMsg::getPos()))
			{
				_runCallback();
			}
			// ��� m_bIsSelected Ϊ false
			m_bIsSelected = false;
		}

		if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN)
		{
			if (pMouseover->isPointIn(EMouseMsg::getPos()))
			{
				// ���������£���λ�ڰ�ť��ʱ����� m_bIsSelected Ϊ true
				m_bIsSelected = true;
				_setStatus(STATUS::SELECTED);
				return;
			}
		}

		if (m_bIsSelected && EMouseMsg::isLButtonDown())
		{
			if (pSelected->isPointIn(EMouseMsg::getPos()))
			{
				_setStatus(STATUS::SELECTED);
				return;
			}
		}
		else if (m_pNormal->isPointIn(EMouseMsg::getPos()))
		{
			_setStatus(STATUS::MOUSEOVER);
			return;
		}

		_setStatus(STATUS::NORMAL);
	}
}

void e2d::EButton::_runCallback()
{
	if (m_Callback)
	{
		m_Callback();
	}
}
