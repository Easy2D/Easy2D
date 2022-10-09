#include <easy2d/e2dshape.h>

easy2d::Shape::~Shape()
{
    SafeRelease(geo_);
}

easy2d::Shape* easy2d::Shape::NewLine(Point begin, Point end)
{
    return nullptr;
}

easy2d::Rect easy2d::Shape::getBoundingBox(const Matrix32* transform) const
{
    Rect bounds;
    if (geo_)
    {
        // no matter it failed or not
        geo_->GetBounds(
             reinterpret_cast<const D2D1_MATRIX_3X2_F*>(transform),
            reinterpret_cast<D2D1_RECT_F*>(&bounds)
        );
    }
    return bounds;
}

bool easy2d::Shape::containsPoint(const Point& point, const Matrix32* transform) const
{
    if (geo_)
    {
        BOOL ret = 0;
        // no matter it failed or not
        geo_->FillContainsPoint(
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
    if (geo_)
    {
        // no matter it failed or not
        geo_->ComputeLength(D2D1::Matrix3x2F::Identity(), &length);
    }
    return length;
}

float easy2d::Shape::computeArea() const
{
    if (geo_)
    {
        float area = 0.f;
        // no matter it failed or not
        geo_->ComputeArea(D2D1::Matrix3x2F::Identity(), &area);
    }
    return 0.f;
}

bool easy2d::Shape::computePointAtLength(float length, Point& point, Vector2& tangent) const
{
    if (geo_)
    {
        HRESULT hr = geo_->ComputePointAtLength(
            length,
            D2D1::Matrix3x2F::Identity(),
            reinterpret_cast<D2D_POINT_2F*>(&point),
            reinterpret_cast<D2D_POINT_2F*>(&tangent));
        return SUCCEEDED(hr);
    }
    return false;
}
