#include <easy2d/e2dnode.h>

easy2d::ShapeNode::ShapeNode(Shape::LineType, Point begin, Point end, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::Line, begin, end), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape::RectType, const Size& size, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::Rect, Rect(Point(), size)), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape::RoundedRectType, const Size& size, const Vector2& radius, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::RoundedRect, Rect(Point(), size), radius), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape::CircleType, float radius, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::Circle, Point(radius, radius), radius), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape::EllipseType, const Vector2& radius, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::Ellipse, radius, radius), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape::PolygonType, std::initializer_list<Point> vertices, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::Polygon, vertices), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape::PolygonType, const Point* vertices, int count, DrawingStyle style)
	: ShapeNode(gcnew Shape(Shape::Polygon, vertices, count), style)
{
}

easy2d::ShapeNode::ShapeNode(Shape* shape, DrawingStyle style)
	: _shape(nullptr)
	, _style(style)
	, _bounds()
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

easy2d::DrawingStyle easy2d::ShapeNode::getDrawingStyle() const
{
	return _style;
}

void easy2d::ShapeNode::setDrawingStyle(DrawingStyle style)
{
	_style = style;
}

easy2d::DrawingStyle::Mode easy2d::ShapeNode::getDrawingMode() const
{
	return _style.mode;
}

void easy2d::ShapeNode::setDrawingMode(DrawingStyle::Mode mode)
{
	_style.mode = mode;
}

void easy2d::ShapeNode::onRender()
{
	if (!_shape)
		return;

	auto pBrush = Renderer::getSolidColorBrush();
	pBrush->SetOpacity(_displayOpacity);

	if (_style.mode == DrawingStyle::Mode::Fill || _style.mode == DrawingStyle::Mode::Round)
	{
		ID2D1StrokeStyle* strokeStyle = nullptr;
		switch (_style.lineJoin)
		{
		case LineJoin::Miter:
			strokeStyle = Renderer::getMiterID2D1StrokeStyle();
			break;
		case LineJoin::Bevel:
			strokeStyle = Renderer::getBevelID2D1StrokeStyle();
			break;
		case LineJoin::Round:
			strokeStyle = Renderer::getRoundID2D1StrokeStyle();
			break;
		}
		pBrush->SetColor(reinterpret_cast<const D2D1_COLOR_F&>(_style.strokeColor));
		Renderer::getRenderTarget()->DrawGeometry(_shape->_geo, pBrush, _style.strokeWidth, strokeStyle);
	}

	if (_style.mode == DrawingStyle::Mode::Fill || _style.mode == DrawingStyle::Mode::Solid)
	{
		pBrush->SetColor(reinterpret_cast<const D2D1_COLOR_F&>(_style.fillColor));
		Renderer::getRenderTarget()->FillGeometry(_shape->_geo, pBrush);
	}
}

easy2d::Color easy2d::ShapeNode::getFillColor() const
{
	return _style.fillColor;
}

easy2d::Color easy2d::ShapeNode::getStrokeColor() const
{
	return _style.strokeColor;
}

float easy2d::ShapeNode::getStrokeWidth() const
{
	return _style.strokeWidth;
}

void easy2d::ShapeNode::setFillColor(Color fillColor)
{
	_style.fillColor = fillColor;
}

void easy2d::ShapeNode::setStrokeColor(Color strokeColor)
{
	_style.strokeColor = strokeColor;
}

void easy2d::ShapeNode::setStrokeWidth(float strokeWidth)
{
	_style.strokeWidth = strokeWidth;
}

void easy2d::ShapeNode::setLineJoin(LineJoin lineJoin)
{
	_style.lineJoin = lineJoin;
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
