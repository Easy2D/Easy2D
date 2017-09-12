#include "..\..\Easy2d.h"
#include "..\..\EasyX\easyx.h"

Shape::Shape() :
	lineColor(Color::black),
	fillColor(Color::white)
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
	if (_style == Shape::STYLE::round) 
	{
		roundShape();
	}
	else if (_style == Shape::STYLE::solid) 
	{
		solidShape();
	}
	else if (_style == Shape::STYLE::fill) 
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
