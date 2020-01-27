#include <easy2d/e2dshape.h>

easy2d::CircleShape::CircleShape()
	: _radius(0)
{
	this->setAnchor(0.5, 0.5);
}

easy2d::CircleShape::CircleShape(float radius)
{
	this->setRadius(radius);
	this->setAnchor(0.5, 0.5);
}

easy2d::CircleShape::CircleShape(Point center, float radius)
{
	this->setRadius(radius);
	this->setPos(center);
	this->setAnchor(0.5, 0.5);
}

easy2d::CircleShape::~CircleShape()
{
}

float easy2d::CircleShape::getRadius() const
{
	return _radius;
}

void easy2d::CircleShape::setRadius(float radius)
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
