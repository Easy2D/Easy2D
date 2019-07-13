#include <e2dshape.h>

easy2d::CircleShape::CircleShape()
	: _radius(0)
{
	this->setPivot(0.5, 0.5);
}

easy2d::CircleShape::CircleShape(double radius)
{
	this->setRadius(radius);
	this->setPivot(0.5, 0.5);
}

easy2d::CircleShape::CircleShape(Point center, double radius)
{
	this->setRadius(radius);
	this->setPos(center);
	this->setPivot(0.5, 0.5);
}

easy2d::CircleShape::~CircleShape()
{
}

double easy2d::CircleShape::getRadius() const
{
	return _radius;
}

void easy2d::CircleShape::setRadius(double radius)
{
	_radius = float(radius);
	Node::setSize(radius * 2, radius * 2);
}

void easy2d::CircleShape::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radius, _radius), _radius, _radius),
		Renderer::getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void easy2d::CircleShape::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radius, _radius), _radius, _radius),
		Renderer::getSolidColorBrush()
	);
}
