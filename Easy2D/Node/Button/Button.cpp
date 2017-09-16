#include "..\..\Easy2d.h"
#include "..\..\EasyX\easyx.h"


Button::Button() : 
	m_bEnable(true)
{
}

Button::~Button()
{
}

bool Button::_exec(bool active)
{
	// ��ť�Ƿ�����
	if (!m_bEnable)
	{
		return false;
	}
	return MouseNode::_exec(active);
}

void Button::_onDraw()
{
	// ��ť�Ƿ�����
	if (!m_bEnable)
	{
		// δ����ʱ������ Disable ״̬
		_onDisable();
		return;
	}
	MouseNode::_onDraw();
}

bool Button::isEnable()
{
	return m_bEnable;
}

void Button::setEnable(bool enable)
{
	m_bEnable = enable;
}
