#include <easy2d/e2dnode.h>

easy2d::ShapeNode::ShapeNode(Shape* shape)
	: _style(Style::Fill)
	, _strokeWidth(2)
	, _strokeColor(0x78B7D0U)
	, _fillColor(0x6090A0U)
	, _bounds()
	, _shape(nullptr)
	, _strokeStyle(nullptr)
{
	setShape(shape);
}

easy2d::ShapeNode::~ShapeNode()
{
	GC::release(_shape);
}

easy2d::Shape* easy2d::ShapeNode::getShape() const
{
	return _shape;
}

void easy2d::ShapeNode::setShape(Shape* shape)
{
	GC::release(_shape);
	_shape = shape;
	GC::retain(_shape);

	if (_shape)
	{
		_bounds = _shape->getBoundingBox();
		setSize(_bounds.getSize());
	}
	else
	{
		_bounds = Rect{};
		setSize(0.f, 0.f);
	}
}

void easy2d::ShapeNode::onRender()
{
	if (!_shape)
		return;

	auto pBrush = Renderer::getSolidColorBrush();
	pBrush->SetOpacity(_displayOpacity);

	if (_style == Style::Fill || _style == Style::Round)
	{
		pBrush->SetColor(reinterpret_cast<const D2D1_COLOR_F&>(_strokeColor));
		Renderer::getRenderTarget()->DrawGeometry(_shape->_geo, pBrush, _strokeWidth, _strokeStyle);
	}

	if (_style == Style::Fill || _style == Style::Solid)
	{
		pBrush->SetColor(reinterpret_cast<const D2D1_COLOR_F&>(_fillColor));
		Renderer::getRenderTarget()->FillGeometry(_shape->_geo, pBrush);
	}
}

easy2d::Color easy2d::ShapeNode::getFillColor() const
{
	return _fillColor;
}

easy2d::Color easy2d::ShapeNode::getStrokeColor() const
{
	return _strokeColor;
}

float easy2d::ShapeNode::getStrokeWidth() const
{
	return _strokeWidth;
}

easy2d::ShapeNode::Style easy2d::ShapeNode::getStyle() const
{
	return _style;
}

void easy2d::ShapeNode::setFillColor(Color fillColor)
{
	_fillColor = fillColor;
}

void easy2d::ShapeNode::setStrokeColor(Color strokeColor)
{
	_strokeColor = strokeColor;
}

void easy2d::ShapeNode::setStrokeWidth(float strokeWidth)
{
	_strokeWidth = float(strokeWidth) * 2;
}

void easy2d::ShapeNode::setStyle(Style style)
{
	_style = style;
}

void easy2d::ShapeNode::setLineJoin(LineJoin lineJoin)
{
	switch (lineJoin)
	{
	case LineJoin::Miter:
		_strokeStyle = Renderer::getMiterID2D1StrokeStyle();
		break;
	case LineJoin::Bevel:
		_strokeStyle = Renderer::getBevelID2D1StrokeStyle();
		break;
	case LineJoin::Round:
		_strokeStyle = Renderer::getRoundID2D1StrokeStyle();
		break;
	default:
		_strokeStyle = nullptr;
		break;
	}
}

easy2d::Rect easy2d::ShapeNode::getBounds() const
{
	return _bounds;
}

easy2d::Rect easy2d::ShapeNode::getBoundingBox() const
{
	if (_shape)
	{
		return _shape->getBoundingBox(&getTransform());
	}
	return Rect();
}

bool easy2d::ShapeNode::containsPoint(Point const& point) const
{
	if (_shape)
	{
		return _shape->containsPoint(point, &getTransform());
	}
	return false;
}
