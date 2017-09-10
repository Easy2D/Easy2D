#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"

// ������ɫֵ�Ķ���
const COLORREF Color::black = BLACK;
const COLORREF Color::blue = BLUE;
const COLORREF Color::green = GREEN;
const COLORREF Color::cyan = CYAN;
const COLORREF Color::red = RED;
const COLORREF Color::magenta = MAGENTA;
const COLORREF Color::brown = BROWN;
const COLORREF Color::lightgray = LIGHTGRAY;
const COLORREF Color::darkgray = DARKGRAY;
const COLORREF Color::lightblue = LIGHTBLUE;
const COLORREF Color::lightgreen = LIGHTGREEN;
const COLORREF Color::lightcyan = LIGHTCYAN;
const COLORREF Color::lightred = LIGHTRED;
const COLORREF Color::lightmagenta = LIGHTMAGENTA;
const COLORREF Color::yellow = YELLOW;
const COLORREF Color::white = WHITE;


COLORREF Color::getFromRGB(BYTE r, BYTE g, BYTE b)
{
	return RGB(r, g, b);	// �� (r, g, b) ��ɫֵת��Ϊ��ɫ
}

COLORREF Color::getFromHSL(float H, float S, float L)
{
	return HSLtoRGB(H, S, L);
}

COLORREF Color::getFromHSV(float H, float S, float V)
{
	return HSVtoRGB(H, S, V);
}

BYTE Color::getRValue(COLORREF color)
{
	return GetRValue(color);	// ������ɫ�еĺ�ɫֵ
}

BYTE Color::getGValue(COLORREF color)
{
	return GetGValue(color);	// ������ɫ�е���ɫֵ
}

BYTE Color::getBValue(COLORREF color)
{
	return GetBValue(color);	// ������ɫ�е���ɫֵ
}

COLORREF Color::getGray(COLORREF color)
{
	return RGBtoGRAY(color);	// ��ȡ��ɫ�еĻҶ�ֵ
}
