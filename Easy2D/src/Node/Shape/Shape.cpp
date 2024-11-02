#include <easy2d/e2dshape.h>

easy2d::Shape::LineType easy2d::Shape::Line;
easy2d::Shape::RectType easy2d::Shape::Rect;
easy2d::Shape::RoundedRectType easy2d::Shape::RoundedRect;
easy2d::Shape::CircleType easy2d::Shape::Circle;
easy2d::Shape::EllipseType easy2d::Shape::Ellipse;
easy2d::Shape::PolygonType easy2d::Shape::Polygon;

easy2d::Shape::Shape(ID2D1Geometry* geo)
	: _geo(geo)
{
	if (_geo)
	{
		_geo->AddRef();
	}
}

easy2d::Shape::~Shape()
{
	SafeRelease(_geo);
}

void easy2d::Shape::resetGeometry(ID2D1Geometry* geo)
{
	SafeRelease(_geo);
	_geo = geo;
	if (_geo)
	{
		_geo->AddRef();
	}
}

easy2d::Shape::Shape(LineType, Point begin, Point end)
{
	ShapeMaker maker;
	maker.beginPath(begin);
	maker.addLine(end);
	maker.endPath(false);
	resetGeometry(maker.getGeometry());
}

easy2d::Shape::Shape(RectType, const easy2d::Rect& rect)
{
	ID2D1RectangleGeometry* geo = nullptr;
	HRESULT hr = Renderer::getID2D1Factory()->CreateRectangleGeometry(reinterpret_cast<const D2D1_RECT_F&>(rect), &geo);
	if (FAILED(hr))
	{
		E2D_ERROR_IF_FAILED(hr, "CreateRectangleGeometry failed");
		return;
	}
	_geo = geo;
}

easy2d::Shape::Shape(RoundedRectType, const easy2d::Rect& rect, const Vector2& radius)
{
	ID2D1RoundedRectangleGeometry* geo = nullptr;
	HRESULT hr = Renderer::getID2D1Factory()->CreateRoundedRectangleGeometry(D2D1::RoundedRect(reinterpret_cast<const D2D1_RECT_F&>(rect), radius.x, radius.y), &geo);
	if (FAILED(hr))
	{
		E2D_ERROR_IF_FAILED(hr, "CreateRoundedRectangleGeometry failed");
		return;
	}
	_geo = geo;
}

easy2d::Shape::Shape(CircleType, const Point& center, float radius)
	: Shape(Shape::Ellipse, center, Vector2{ radius, radius })
{
}

easy2d::Shape::Shape(EllipseType, const Point& center, const Vector2& radius)
{
	ID2D1EllipseGeometry* geo = nullptr;
	HRESULT hr = Renderer::getID2D1Factory()->CreateEllipseGeometry(D2D1::Ellipse(reinterpret_cast<const D2D1_POINT_2F&>(center), radius.x, radius.y), &geo);
	if (FAILED(hr))
	{
		E2D_ERROR_IF_FAILED(hr, "CreateEllipseGeometry failed");
		return;
	}
	_geo = geo;
}

easy2d::Shape::Shape(PolygonType, std::initializer_list<Point> vertices)
	: Shape(Polygon, vertices.begin(), static_cast<int>(vertices.size()))
{
}

easy2d::Shape::Shape(PolygonType, const Point* vertices, int count)
{
	if (count > 1)
	{
		ShapeMaker maker;
		maker.beginPath(vertices[0]);
		maker.addLines(&vertices[1], count - 1);
		maker.endPath(true);
		resetGeometry(maker.getGeometry());
	}
}

easy2d::Rect easy2d::Shape::getBoundingBox(const Matrix32* transform) const
{
	easy2d::Rect bounds;
	if (_geo)
	{
		// no matter it failed or not
		_geo->GetBounds(
			reinterpret_cast<const D2D1_MATRIX_3X2_F*>(transform),
			reinterpret_cast<D2D1_RECT_F*>(&bounds)
		);
	}
	return bounds;
}

bool easy2d::Shape::containsPoint(const Point& point, const Matrix32* transform) const
{
	if (_geo)
	{
		BOOL ret = 0;
		// no matter it failed or not
		_geo->FillContainsPoint(
			reinterpret_cast<const D2D_POINT_2F&>(point),
			reinterpret_cast<const D2D1_MATRIX_3X2_F*>(transform),
			D2D1_DEFAULT_FLATTENING_TOLERANCE,
			&ret
		);
		return !!ret;
	}
	return false;
}

float easy2d::Shape::getLength() const
{
	float length = 0.f;
	if (_geo)
	{
		// no matter it failed or not
		_geo->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
	}
	return length;
}

float easy2d::Shape::computeArea() const
{
	if (_geo)
	{
		float area = 0.f;
		// no matter it failed or not
		_geo->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
	}
	return 0.f;
}

bool easy2d::Shape::computePointAtLength(float length, Point& point, Vector2& tangent) const
{
	if (_geo)
	{
		HRESULT hr = _geo->ComputePointAtLength(
			length,
			D2D1::Matrix3x2F::Identity(),
			reinterpret_cast<D2D_POINT_2F*>(&point),
			reinterpret_cast<D2D_POINT_2F*>(&tangent));
		return SUCCEEDED(hr);
	}
	return false;
}
