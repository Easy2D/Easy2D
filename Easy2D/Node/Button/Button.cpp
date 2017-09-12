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
	if (!m_bEnable || !m_bDisplay)
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

void Button::_judge()
{
	// �жϰ�ť��ǰ��״̬
	// �����λ���ڰ�ť���ڵľ���������
	if (MouseMsg::getMsg().x >= m_nX && MouseMsg::getMsg().x <= m_nX + m_nWidth &&
		MouseMsg::getMsg().y >= m_nY && MouseMsg::getMsg().y <= m_nY + m_nHeight)
	{
		_setMouseIn();

		// ������ڰ�ť�ϣ�������������
		if (MouseMsg::isLButtonDown())
		{
			_setSelected();
		}
	}
	else
	{
		_setNormal();
	}
}

bool Button::isEnable()
{
	return m_bEnable;
}

void Button::setEnable(bool enable)
{
	m_bEnable = enable;
}
