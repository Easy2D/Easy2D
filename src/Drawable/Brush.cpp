#include <easy2d/e2ddrawable.h>

easy2d::Brush::Brush(Color fillColor)
	: Brush(Mode::Solid, fillColor, Color::Transparent, 0.0f, LineJoin::None)
{
}

easy2d::Brush::Brush(Mode mode, Color fillColor, Color strokeColor, float strokeWidth, LineJoin lineJoin)
	: mode(mode)
	, fillColor(fillColor)
	, strokeColor(strokeColor)
	, strokeWidth(strokeWidth)
	, lineJoin(lineJoin)
{
}
