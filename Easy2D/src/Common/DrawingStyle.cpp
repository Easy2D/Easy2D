#include <easy2d/e2dcommon.h>

easy2d::DrawingStyle::DrawingStyle(Mode mode, Color fillColor, Color strokeColor, float strokeWidth, LineJoin lineJoin)
	: mode(mode)
	, fillColor(fillColor)
	, strokeColor(strokeColor)
	, strokeWidth(strokeWidth)
	, lineJoin(lineJoin)
{
}

easy2d::DrawingStyle::DrawingStyle(Color fillColor)
	: DrawingStyle(Mode::Solid, fillColor)
{
}
