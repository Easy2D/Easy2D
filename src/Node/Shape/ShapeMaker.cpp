#include <easy2d/e2dshape.h>

easy2d::ShapeMaker::~ShapeMaker()
{
	SafeRelease(_geo);
	SafeRelease(_sink);
}

easy2d::Shape* easy2d::ShapeMaker::combine(Shape* shape1, Shape* shape2, CombineMode mode, const Matrix32* matrix)
{
	ShapeMaker maker;
	maker.openSink();

	if (shape1 && shape2)
	{
		auto geo1 = shape1->_geo;
		auto geo2 = shape2->_geo;
		geo1->CombineWithGeometry(geo2, D2D1_COMBINE_MODE(mode), reinterpret_cast<const D2D1_MATRIX_3X2_F&>(matrix), maker._sink);
	}

	maker.closeSink();
	return maker.getShape();
}

easy2d::Shape* easy2d::ShapeMaker::getShape() const
{
	return gcnew Shape(_geo);
}

ID2D1PathGeometry* easy2d::ShapeMaker::getGeometry() const
{
	return _geo;
}

void easy2d::ShapeMaker::beginPath(const Point& point)
{
	openSink();
	_sink->BeginFigure(reinterpret_cast<const D2D1_POINT_2F&>(point), D2D1_FIGURE_BEGIN_FILLED);
}

void easy2d::ShapeMaker::endPath(bool closed)
{
	_sink->EndFigure(closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
	closeSink();
}

void easy2d::ShapeMaker::addLine(const Point& point)
{
	_sink->AddLine(reinterpret_cast<const D2D1_POINT_2F&>(point));
}

void easy2d::ShapeMaker::addLines(std::initializer_list<Point> points)
{
	addLines(points.begin(), static_cast<int>(points.size()));
}

void easy2d::ShapeMaker::addLines(const Point* points, int count)
{
	_sink->AddLines(reinterpret_cast<const D2D_POINT_2F*>(points), UINT32(count));
}

void easy2d::ShapeMaker::addBezier(const Point& point1, const Point& point2, const Point& point3)
{
	_sink->AddBezier(
		D2D1::BezierSegment(reinterpret_cast<const D2D1_POINT_2F&>(point1), reinterpret_cast<const D2D1_POINT_2F&>(point2), reinterpret_cast<const D2D1_POINT_2F&>(point3))
	);
}

void easy2d::ShapeMaker::addArc(const Point& point, const Size& radius, float rotation, bool clockwise, bool smallSize)
{
	_sink->AddArc(D2D1::ArcSegment(reinterpret_cast<const D2D1_POINT_2F&>(point), reinterpret_cast<const D2D1_SIZE_F&>(radius), rotation,
		clockwise ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
		smallSize ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE));
}

void easy2d::ShapeMaker::openSink()
{
	if (_sink)
	{
		E2D_WARNING("ShapeMaker sink is already opened");
		return;
	}

	if (!_geo)
	{
		HRESULT hr = Renderer::getID2D1Factory()->CreatePathGeometry(&_geo);
		if (FAILED(hr))
		{
			E2D_ERROR_IF_FAILED(hr, "CreatePathGeometry failed");
			return;
		}
	}

	HRESULT hr = _geo->Open(&_sink);
	if (FAILED(hr))
	{
		E2D_ERROR_IF_FAILED(hr, "ID2D1PathGeometry::Open failed");
		return;
	}
}

void easy2d::ShapeMaker::closeSink()
{
	_sink->Close();
	SafeRelease(_sink);
}
