#include <e2dcommon.h>

easy2d::Rect::Rect(void)
{
	setRect(0.0, 0.0, 0.0, 0.0);
}

easy2d::Rect::Rect(float x, float y, float width, float height)
{
	setRect(x, y, width, height);
}

easy2d::Rect::Rect(const Point& pos, const Size& size)
{
	setRect(pos.x, pos.y, size.width, size.height);
}

easy2d::Rect::Rect(const Rect& other)
{
	setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

easy2d::Rect& easy2d::Rect::operator= (const Rect& other)
{
	setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
	return *this;
}

bool easy2d::Rect::operator==(const Rect & rect) const
{
	return (origin == rect.origin) && (size == rect.size);
}

void easy2d::Rect::setRect(float x, float y, float width, float height)
{
	origin.x = x;
	origin.y = y;

	size.width = width;
	size.height = height;
}

bool easy2d::Rect::containsPoint(const Point& point) const
{
	if (point.x >= origin.x && point.x <= (origin.x + size.width)
		&& point.y >= origin.y && point.y <= (origin.y + size.height))
	{
		return true;
	}
	return false;
}

bool easy2d::Rect::intersects(const Rect& rect) const
{
	return !((origin.x + size.width)			< rect.origin.x ||
			(rect.origin.x + rect.size.width)	<      origin.x ||
			(origin.y + size.height)			< rect.origin.y ||
			(rect.origin.y + rect.size.height)	<      origin.y);
}
