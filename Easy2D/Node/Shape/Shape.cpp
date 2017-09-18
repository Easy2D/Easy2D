#include "..\..\Easy2d.h"
#include "..\..\EasyX\easyx.h"

Shape::Shape() :
	lineColor(Color::black),
	fillColor(Color::white),
	m_eStyle(SOLID)
{
}

Shape::~Shape()
{
}

void Shape::_onDraw()
{
	// ��״�Ƿ���ʾ
	if (!m_bDisplay) 
	{
		return;
	}
	// ���������������ɫ
	setlinecolor(lineColor);
	setfillcolor(fillColor);

	// ������״����ʽ���в�ͬ�Ļ���
	if (m_eStyle == Shape::STYLE::ROUND) 
	{
		roundShape();
	}
	else if (m_eStyle == Shape::STYLE::SOLID) 
	{
		solidShape();
	}
	else if (m_eStyle == Shape::STYLE::FILL) 
	{
		fillShape();
	}
}

inline COLORREF Shape::getFillColor() const
{
	return fillColor;
}

inline COLORREF Shape::getLineColor() const
{
	return lineColor;
}

void Shape::setFillColor(COLORREF color)
{
	fillColor = color;
}

void Shape::setLineColor(COLORREF color)
{
	lineColor = color;
}

void Shape::setStyle(STYLE style)
{
	m_eStyle = style;
}
