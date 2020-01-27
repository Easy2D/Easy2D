#include <easy2d/e2dshape.h>

easy2d::RoundRectShape::RoundRectShape()
	: _radiusX(0)
	, _radiusY(0)
{
}

easy2d::RoundRectShape::RoundRectShape(Size size, float radiusX, float radiusY)
	: _radiusX(float(radiusX))
	, _radiusY(float(radiusY))
{
	this->setSize(size);
}

easy2d::RoundRectShape::RoundRectShape(Point topLeft, Size size, float radiusX, float radiusY)
	: _radiusX(float(radiusX))
	, _radiusY(float(radiusY))
{
	this->setAnchor(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

easy2d::RoundRectShape::~RoundRectShape()
{
}

float easy2d::RoundRectShape::getRadiusX() const
{
	return _radiusX;
}

float easy2d::RoundRectShape::getRadiusY() const
{
	return _radiusY;
}

void easy2d::RoundRectShape::setRadiusX(float radiusX)
{
	_radiusX = float(radiusX);
}

void easy2d::RoundRectShape::setRadiusY(float radiusY)
{
	_radiusY = float(radiusY);
}

void easy2d::RoundRectShape::_renderLine()
{
	Renderer::getRenderTarget()->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		Renderer::getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void easy2d::RoundRectShape::_renderFill()
{
	Renderer::getRenderTarget()->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		Renderer::getSolidColorBrush()
	);
}
