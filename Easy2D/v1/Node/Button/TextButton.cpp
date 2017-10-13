#include "..\..\easy2d.h"


TextButton::TextButton() :
	m_pNormalText(nullptr),
	m_pMouseInText(nullptr),
	m_pSelectedText(nullptr),
	m_pUnableText(nullptr)
{
}

TextButton::TextButton(TString text) :
	TextButton()
{
	setNormal(new Text(text));	// ���ð�ť������״̬ʱ������
}

TextButton::TextButton(Text * text) :
	TextButton()
{
	setNormal(text);	// ���ð�ť������״̬ʱ������
}

TextButton::~TextButton()
{
	// �����ı������ü�����һ
	SafeRelease(m_pNormalText);
	SafeRelease(m_pMouseInText);
	SafeRelease(m_pSelectedText);
	SafeRelease(m_pUnableText);
}

void TextButton::_setStatus(Status status)
{
	if (m_eStatus != status)
	{
		if (status == MOUSEIN)
		{
			if (m_pMouseInText) setRect(m_pMouseInText->getRect());
		}
		else if (status == SELECTED)
		{
			if (m_pSelectedText) setRect(m_pSelectedText->getRect());
		}
		else
		{
			setRect(m_pNormalText->getRect());
		}
	}
	MouseNode::_setStatus(status);
}

void TextButton::_onNormal()
{
	if (m_pNormalText)
	{
		m_pNormalText->_onDraw();
	}
}

void TextButton::_onMouseIn()
{
	if (m_pMouseInText)
	{
		m_pMouseInText->_onDraw();
	}
	else
	{
		_onNormal();
	}
}

void TextButton::_onSelected()
{
	if (m_pSelectedText)
	{
		m_pSelectedText->_onDraw();
	}
	else
	{
		_onNormal();
	}
}

void TextButton::_onDisable()
{
	if (m_pUnableText)
	{
		m_pUnableText->_onDraw();
	}
	else
	{
		_onNormal();
	}
}

void TextButton::setNormal(Text * text)
{
	if (text)
	{
		// ԭ�ı����ü�����һ
		SafeRelease(m_pNormalText);
		// �޸��ı�
		m_pNormalText = text;
		// ���ı����ü�����һ
		m_pNormalText->retain();
		// �������ֿ���趨��ť��С
		setSize(m_pNormalText->getSize());
		// ���¼����ı�λ��
		_resetPosition();
	}
}

void TextButton::setMouseIn(Text * text)
{
	if (text)
	{
		SafeRelease(m_pMouseInText);
		m_pMouseInText = text;
		m_pMouseInText->retain();
		_resetPosition();
	}
}

void TextButton::setSelected(Text * text)
{
	if (text)
	{
		SafeRelease(m_pSelectedText);
		m_pSelectedText = text;
		m_pSelectedText->retain();
		_resetPosition();
	}
}

void TextButton::setUnable(Text * text)
{
	if (text)
	{
		SafeRelease(m_pUnableText);
		m_pUnableText = text;
		m_pUnableText->retain();
		_resetPosition();
	}
}

void TextButton::_resetPosition()
{
	if (m_pNormalText)
	{
		// ���ݰ�ťλ�ú����ֿ����������λ�þ�����ʾ
		m_pNormalText->setPos(getX() , getY());
	}
	if (m_pMouseInText)
	{
		m_pMouseInText->setPos(
			getX() + (getWidth() - m_pMouseInText->getWidth()) / 2,
			getY() + (getHeight() - m_pMouseInText->getHeight()) / 2);
	}
	if (m_pSelectedText)
	{
		m_pSelectedText->setPos(
			getX() + (getWidth() - m_pSelectedText->getWidth()) / 2,
			getY() + (getHeight() - m_pSelectedText->getHeight()) / 2);
	}
	if (m_pUnableText)
	{
		m_pUnableText->setPos(
			getX() + (getWidth() - m_pUnableText->getWidth()) / 2,
			getY() + (getHeight() - m_pUnableText->getHeight()) / 2);
	}
}
