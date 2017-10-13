#include "..\..\easy2d.h"
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

void Button::setX(int x)
{
	MouseNode::setX(x);
	_resetPosition();
}

void Button::setY(int y)
{
	MouseNode::setY(y);
	_resetPosition();
}

void Button::setPos(int x, int y)
{
	MouseNode::setPos(x, y);
	_resetPosition();
}

void Button::setPos(CPoint p)
{
	MouseNode::setPos(p);
	_resetPosition();
}

void Button::move(int x, int y)
{
	MouseNode::move(x, y);
	_resetPosition();
}

void Button::move(CVector v)
{
	MouseNode::move(v);
	_resetPosition();
}
