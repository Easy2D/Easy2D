#include <easy2d/e2dcommon.h>

easy2d::Rect::Rect(void)
{
}

easy2d::Rect::Rect(const Point& lt, const Point& rb)
	: leftTop(lt)
	, rightBottom(rb)
{
}

easy2d::Rect::Rect(const Point& pos, const Size& size)
{
	setRect(pos, size);
}

easy2d::Rect::Rect(const Rect& other)
	: leftTop(other.leftTop)
	, rightBottom(other.rightBottom)
{
}

easy2d::Rect& easy2d::Rect::operator= (const Rect& other)
{
	this->leftTop = other.leftTop;
	this->rightBottom = other.rightBottom;
	return *this;
}

bool easy2d::Rect::operator==(const Rect & rect) const
{
	return (leftTop == rect.leftTop) && (rightBottom == rect.rightBottom);
}

void easy2d::Rect::setRect(const Point& pos, const Size& size)
{
	this->leftTop = pos;
	this->rightBottom = Point{ pos.x + size.width, pos.y + size.height };
}

bool easy2d::Rect::containsPoint(const Point& point) const
{
	return point.x >= leftTop.x && point.x <= rightBottom.x && point.y >= leftTop.y && point.y <= rightBottom.y;
}

bool easy2d::Rect::intersects(const Rect& rect) const
{
	return !(rightBottom.x < rect.leftTop.x || rect.rightBottom.x < leftTop.x || rightBottom.y < rect.leftTop.y || rect.rightBottom.y < leftTop.y);
}
