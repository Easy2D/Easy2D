#include "..\easy2d.h"


MouseNode::MouseNode() :
	m_bBlock(true),
	m_bTarget(false),
	m_ClickCallback([]() {}),
	m_OnMouseInCallback([]() {}),
	m_OnMouseOutCallback([]() {}),
	m_OnSelectCallback([]() {}),
	m_OnUnselectCallback([]() {})
{
}

MouseNode::~MouseNode()
{
}

bool MouseNode::_exec(bool active)
{
	// �� display ����Ϊ false���˳�����
	if (!m_bDisplay)
	{
		return false;
	}
	// ��������ȡ�ý��㣬���ð�ť���Բ��˳�
	if (!active)
	{
		reset();
		return false;
	}
	// �жϽڵ㵱ǰ��״̬
	// ���ڵ�δȡ�ý��㣬�������жϽڵ�״̬
	if (!m_bTarget)
	{
		// �����λ���ڽڵ����ڵľ���������
		if (_isMouseIn())
		{
			// ״̬��Ϊ MOUSEIN
			_setStatus(MOUSEIN);
			// ����ʱ����������
			if (MouseMsg::getMsg() == MouseMsg::LBUTTON_DOWN)
			{
				m_bTarget = true;		// ȡ�ý�����
				_setStatus(SELECTED);	// ״̬��Ϊ SELECTED
			}
			// ���ڵ����������Ϣ����ȡ�û��潹��
			if (m_bBlock) return true;
		}
		else
		{
			reset();		// �ָ�Ĭ��״̬
		}
	}
	else
	{
		// �ڵ�ȡ�ý���ʱ������̧��
		if (MouseMsg::getMsg() == MouseMsg::LBUTTON_UP)
		{
			// �����̧��ʱ������ڽڵ���
			if (_isMouseIn())
			{
				m_ClickCallback();	// ִ�лص�����
			}
			reset();				// �ָ�Ĭ��״̬
		}
		// ���ڵ����������Ϣ����ȡ�û��潹��
		if (m_bBlock) return true;
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

bool MouseNode::_isMouseIn()
{
	return isPointIn(MouseMsg::getPos());
}

void MouseNode::_setStatus(Status status)
{
	if (m_eStatus != status)
	{
		// �˳�ĳ��״̬�Ļص�����
		if (m_eStatus == MOUSEIN)
		{
			m_OnMouseOutCallback();
		}
		else if (m_eStatus == SELECTED)
		{
			m_OnUnselectCallback();
		}
		// ����ĳ��״̬�Ļص�����
		if (status == MOUSEIN)
		{
			m_OnMouseInCallback();
		}
		else if (status == SELECTED)
		{
			m_OnSelectCallback();
		}
		m_eStatus = status;
	}
}

bool MouseNode::isMouseIn()
{
	return m_eStatus == MOUSEIN || m_eStatus == SELECTED;
}

bool MouseNode::isSelected()
{
	return m_eStatus == SELECTED;
}

void MouseNode::setClickedCallback(const CLICK_CALLBACK & callback)
{
	m_ClickCallback = callback;
}

void MouseNode::setMouseInCallback(const CLICK_CALLBACK & callback)
{
	m_OnMouseInCallback = callback;
}

void MouseNode::setMouseOutCallback(const CLICK_CALLBACK & callback)
{
	m_OnMouseOutCallback = callback;
}

void MouseNode::setSelectCallback(const CLICK_CALLBACK & callback)
{
	m_OnSelectCallback = callback;
}

void MouseNode::setUnselectCallback(const CLICK_CALLBACK & callback)
{
	m_OnUnselectCallback = callback;
}

void MouseNode::reset()
{
	m_bTarget = false;		// ʧȥ������
	_setStatus(NORMAL);		// �ָ�Ĭ��״̬
}

void MouseNode::setBlock(bool block)
{
	m_bBlock = block;
}
