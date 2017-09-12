#include "..\..\Easy2d.h"


TextButton::TextButton() :
	m_pNormalText(nullptr),
	m_pMouseInText(nullptr),
	m_pSelectedText(nullptr),
	m_pUnableText(nullptr)
{
	m_nWidth = 0;
	m_nHeight = 0;
}

TextButton::TextButton(Text * text) :
	TextButton()
{
	setNormal(text);	// ���ð�ť������״̬ʱ������
}

TextButton::~TextButton()
{
	// �����ı������ü�����һ
	SAFE_RELEASE(m_pNormalText);
	SAFE_RELEASE(m_pMouseInText);
	SAFE_RELEASE(m_pSelectedText);
	SAFE_RELEASE(m_pUnableText);
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
		SAFE_RELEASE(m_pNormalText);
		// �޸��ı�
		m_pNormalText = text;
		// ���ı����ü�����һ
		m_pNormalText->retain();
		// ���¼����ı�λ��
		resetTextPosition();
	}
}

void TextButton::setMouseIn(Text * text)
{
	if (text)
	{
		SAFE_RELEASE(m_pMouseInText);
		m_pMouseInText = text;
		m_pMouseInText->retain();
		resetTextPosition();
	}
}

void TextButton::setSelected(Text * text)
{
	if (text)
	{
		SAFE_RELEASE(m_pSelectedText);
		m_pSelectedText = text;
		m_pSelectedText->retain();
		resetTextPosition();
	}
}

void TextButton::setUnable(Text * text)
{
	if (text)
	{
		SAFE_RELEASE(m_pUnableText);
		m_pUnableText = text;
		m_pUnableText->retain();
		resetTextPosition();
	}
}

void TextButton::setX(int x)
{
	Node::setX(x);
	resetTextPosition();
}

void TextButton::setY(int y)
{
	Node::setY(y);
	resetTextPosition();
}

void TextButton::setPos(int x, int y)
{
	Node::setPos(x, y);
	resetTextPosition();
}

void TextButton::resetTextPosition()
{
	if (m_pNormalText)
	{
		// �������ֿ���趨��ť��С
		m_nWidth = m_pNormalText->getWidth();
		m_nHeight = m_pNormalText->getHeight();
		// ���ݰ�ťλ�ú����ֿ����������λ�þ�����ʾ
		m_pNormalText->setPos(m_nX , m_nY);
	}
	if (m_pMouseInText)
	{
		m_pMouseInText->setPos(
			m_nX + (m_nWidth - m_pMouseInText->getWidth()) / 2, 
			m_nY + (m_nHeight - m_pMouseInText->getHeight()) / 2);
	}
	if (m_pSelectedText)
	{
		m_pSelectedText->setPos(
			m_nX + (m_nWidth - m_pSelectedText->getWidth()) / 2, 
			m_nY + (m_nHeight - m_pSelectedText->getHeight()) / 2);
	}
	if (m_pUnableText)
	{
		m_pUnableText->setPos(
			m_nX + (m_nWidth - m_pUnableText->getWidth()) / 2, 
			m_nY + (m_nHeight - m_pUnableText->getHeight()) / 2);
	}
}
