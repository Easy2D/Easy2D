#include <easy2d/e2dcommon.h>

static const UINT sc_redShift = 16;
static const UINT sc_greenShift = 8;
static const UINT sc_blueShift = 0;

static const UINT sc_redMask = 0xff << sc_redShift;
static const UINT sc_greenMask = 0xff << sc_greenShift;
static const UINT sc_blueMask = 0xff << sc_blueShift;

easy2d::Color::Color()
	: r(0)
	, g(0)
	, b(0)
	, a(1)
{
}

easy2d::Color::Color(float r, float g, float b)
	: r(float(r))
	, g(float(g))
	, b(float(b))
	, a(float(1))
{
}

easy2d::Color::Color(float r, float g, float b, float alpha)
	: r(float(r))
	, g(float(g))
	, b(float(b))
	, a(float(alpha))
{
}

easy2d::Color::Color(UINT rgb)
	: Color(rgb, 1.f)
{
}

easy2d::Color::Color(UINT rgb, float alpha)
	: r(float((rgb& sc_redMask) >> sc_redShift) / 255.f)
	, g(float((rgb& sc_greenMask) >> sc_greenShift) / 255.f)
	, b(float((rgb& sc_blueMask) >> sc_blueShift) / 255.f)
	, a(alpha)
{
}
