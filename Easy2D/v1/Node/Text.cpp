#include "..\easy2d.h"
#include "..\EasyX\easyx.h"


Text::Text() : 
	m_sText(_T("")),
	m_color(Color::white),
	m_pFontStyle(FontStyle::getDefault())
{
	m_pFontStyle->retain();		// �������ü�����һ
}

Text::Text(TString text, COLORREF color, FontStyle * font) : 
	m_color(color),
	m_pFontStyle(font)
{
	setText(text);
	m_pFontStyle->retain();		// �������ü�����һ
}

Text::Text(int x, int y, TString text, COLORREF color, FontStyle * font) :
	m_color(color),
	m_pFontStyle(font)
{
	setText(text);
	setPos(x, y);
	m_pFontStyle->retain();		// �������ü�����һ
}

Text::~Text()
{
	SafeRelease(m_pFontStyle);	// �������ü�����һ
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
	outtextxy(getX(), getY(), m_sText.c_str());
}

COLORREF Text::getColor() const
{
	return m_color;
}

TString Text::getText() const
{
	return m_sText;
}

FontStyle * Text::getFontStyle()
{
	return m_pFontStyle;
}

bool Text::isEmpty() const
{
	return m_sText.empty();	// �ı��Ƿ�Ϊ��
}

void Text::setText(TString text)
{
	m_sText = text;
	// ���������壬Ȼ���ȡ���ı��ڸ������µĿ�Ⱥ͸߶�
	settextstyle(&m_pFontStyle->m_font);
	setSize(textwidth(getText().c_str()), textheight(getText().c_str()));
}

void Text::setColor(COLORREF color)
{
	m_color = color;
}

void Text::setFontStyle(FontStyle * style)
{
	SafeRelease(m_pFontStyle);	// ԭ�������ü�����һ
	m_pFontStyle = style;		// �޸�����
	m_pFontStyle->retain();		// ���������ü�����һ
	// ���������壬Ȼ���ȡ���ı��ڸ������µĿ�Ⱥ͸߶�
	settextstyle(&m_pFontStyle->m_font);
	setSize(textwidth(getText().c_str()), textheight(getText().c_str()));
}
