#include <e2dcommon.h>

easy2d::Size::Size()
{
	width = 0;
	height = 0;
}

easy2d::Size::Size(double width, double height)
{
	this->width = width;
	this->height = height;
}

easy2d::Size::Size(const Size & other)
{
	width = other.width;
	height = other.height;
}

easy2d::Size easy2d::Size::operator+(Size const & size) const
{
	return Size(width + size.width, height + size.height);
}

easy2d::Size easy2d::Size::operator-(Size const & size) const
{
	return Size(width - size.width, height - size.height);
}

easy2d::Size easy2d::Size::operator*(double const & value) const
{
	return Size(width * value, height * value);
}

easy2d::Size easy2d::Size::operator/(double const & value) const
{
	return Size(width / value, height / value);
}

easy2d::Size::operator easy2d::Point() const
{
	return Point(width, height);
}

easy2d::Size easy2d::Size::operator-() const
{
	return Size(-width, -height);
}

bool easy2d::Size::operator==(const Size & size) const
{
	return (width == size.width) && (height == size.height);
}
