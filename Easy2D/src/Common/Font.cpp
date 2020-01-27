#include <easy2d/e2dcommon.h>

easy2d::Font::Font()
	: family(L"")
	, size(22)
	, weight(Font::Weight::Normal)
	, italic(false)
{
}

easy2d::Font::Font(const String & family, float size, UINT weight, bool italic)
	: family(family)
	, size(size)
	, weight(weight)
	, italic(italic)
{
}