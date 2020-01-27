#include <easy2d/e2dshape.h>

easy2d::EllipseShape::EllipseShape()
	: _radiusX(0)
	, _radiusY(0)
{
	this->setAnchor(0.5, 0.5);
}

easy2d::EllipseShape::EllipseShape(float radiusX, float radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setAnchor(0.5, 0.5);
}

easy2d::EllipseShape::EllipseShape(Point center, float radiusX, float radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setPos(center);
	this->setAnchor(0.5, 0.5);
}

easy2d::EllipseShape::~EllipseShape()
{
}

float easy2d::EllipseShape::getRadiusX() const
{
	return _radiusX;
}

float easy2d::EllipseShape::getRadiusY() const
{
	return _radiusY;
}

void easy2d::EllipseShape::setRadiusX(float radiusX)
{
	_radiusX = float(radiusX);
	Node::setWidth(radiusX * 2);
}

void easy2d::EllipseShape::setRadiusY(float radiusY)
{
	_radiusY = float(radiusY);
	Node::setHeight(radiusY * 2);
}

void easy2d::EllipseShape::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radiusX, _radiusY), _radiusX, _radiusY),
		Renderer::getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void easy2d::EllipseShape::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radiusX, _radiusY), _radiusX, _radiusY),
		Renderer::getSolidColorBrush()
	);
}
