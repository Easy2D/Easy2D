#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"


Text::Text() : 
	m_sText(_T("")),
	m_color(Color::white),
	m_pFontStyle(FontStyle::getDefault())
{
	m_pFontStyle->retain();		// �������ü�����һ
}

Text::Text(tstring text, COLORREF color, FontStyle * font) : 
	m_sText(text),
	m_color(color),
	m_pFontStyle(font)
{
	m_pFontStyle->retain();		// �������ü�����һ
}

Text::Text(int x, int y, tstring text, COLORREF color, FontStyle * font) :
	Node(x, y),
	m_sText(text),
	m_color(color),
	m_pFontStyle(font)
{
	m_pFontStyle->retain();		// �������ü�����һ
}

Text::~Text()
{
	SAFE_RELEASE(m_pFontStyle);	// �������ü�����һ
}

void Text::_onDraw()
{
	// �� display ����Ϊ false������������ı�
	if (!m_bDisplay) 
	{
		return;
	}
	// ��������
	settextstyle(&m_pFontStyle->m_font);
	// �����ı���ɫ
	settextcolor(m_color);
	// �������
	outtextxy(m_nX, m_nY, m_sText.c_str());
}

COLORREF Text::getColor() const
{
	return m_color;
}

tstring Text::getText() const
{
	return m_sText;
}

FontStyle * Text::getFontStyle()
{
	return m_pFontStyle;
}

int Text::getWidth()
{
	// ���������壬Ȼ���ȡ���ı��ڸ������µĿ��
	settextstyle(&m_pFontStyle->m_font);
	return textwidth(getText().c_str());
}

int Text::getHeight()
{
	// ���������壬Ȼ���ȡ���ı��ڸ������µĸ߶�
	settextstyle(&m_pFontStyle->m_font);
	return textheight(getText().c_str());
}

bool Text::isEmpty() const
{
	return m_sText.empty();	// �ı��Ƿ�Ϊ��
}

void Text::setText(tstring text)
{
	m_sText = text;
}

void Text::setColor(COLORREF color)
{
	m_color = color;
}

void Text::setFontStyle(FontStyle * style)
{
	SAFE_RELEASE(m_pFontStyle);	// ԭ�������ü�����һ
	m_pFontStyle = style;		// �޸�����
	m_pFontStyle->retain();		// ���������ü�����һ
}
