#include <easy2d/e2dshape.h>

easy2d::RectShape::RectShape()
{
}

easy2d::RectShape::RectShape(Size size)
{
	this->setSize(size);
}

easy2d::RectShape::RectShape(Point topLeft, Size size)
{
	this->setAnchor(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

easy2d::RectShape::~RectShape()
{
}

void easy2d::RectShape::_renderLine()
{
	Renderer::getRenderTarget()->DrawRectangle(
		D2D1::RectF(0, 0, _width, _height),
		Renderer::getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void easy2d::RectShape::_renderFill()
{
	Renderer::getRenderTarget()->FillRectangle(
		D2D1::RectF(0, 0, _width, _height),
		Renderer::getSolidColorBrush()
	);
}
