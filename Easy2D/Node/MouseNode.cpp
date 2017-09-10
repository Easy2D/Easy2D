#include "..\easy2d.h"


MouseNode::MouseNode() :
	m_bBlock(true),
	m_bTarget(false),
	m_callback([]() {})
{
}

MouseNode::~MouseNode()
{
}

bool MouseNode::_exec(bool active)
{
	// ��������ȡ�ý��㣬�� display ����Ϊ false���˳�����
	if (!active || !m_bDisplay)
	{
		return false;
	}
	// �жϽڵ�״̬
	_judge();
	// ����ڽڵ��ϣ���ѡ��ʱ���Ҳ�ڽڵ��ϣ�
	if (m_eStatus == MOUSEIN || m_eStatus == SELECTED)
	{
		// �ڵ㱻���ѡ�У���������̧��
		if (m_bTarget && MouseMsg::getLButtonUpMsg())
		{
			onClicked();	// ִ�лص�����
		}
		// ���ڵ㲻���������Ϣ����ȡ�û��潹��
		if (!m_bBlock) return true;
	}
	return false;
}

void MouseNode::_onDraw()
{
	// �ڵ��Ƿ���ʾ
	if (!m_bDisplay)
	{
		return;
	}
	// �ڵ��Ƿ�ѡ��
	if (m_eStatus == SELECTED)
	{
		_onSelected();
	}
	else
	{
		// ����Ƿ��ڽڵ���
		if (m_eStatus == MOUSEIN)
		{
			_onMouseIn();
		}
		else
		{
			_onNormal();
		}
	}
}

void MouseNode::_setNormal()
{
	m_bTarget = false;		// ʧȥ������
	m_eStatus = NORMAL;
}

void MouseNode::_setMouseIn()
{
	m_eStatus = MOUSEIN;
}

void MouseNode::_setSelected()
{
	m_bTarget = true;		// ȡ�ý�����
	m_eStatus = SELECTED;
}

void MouseNode::onClicked()
{
	m_callback();
}

bool MouseNode::isMouseIn()
{
	return m_eStatus == MOUSEIN || m_eStatus == SELECTED;
}

bool MouseNode::isSelected()
{
	return m_eStatus == SELECTED;
}

void MouseNode::setOnMouseClicked(const CLICK_CALLBACK & callback)
{
	m_callback = callback;
}

void MouseNode::reset()
{
	m_eStatus = NORMAL;
}

void MouseNode::setBlock(bool block)
{
	m_bBlock = block;
}
