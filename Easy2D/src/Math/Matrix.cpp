#include <easy2d/e2dmath.h>

easy2d::Matrix32::Matrix32()
	: _11(1.f), _12(0.f)
	, _21(0.f), _22(1.f)
	, _31(0.f), _32(0.f)
{
}

easy2d::Matrix32::Matrix32(float _11, float _12, float _21, float _22, float _31, float _32)
	: _11(_11), _12(_12), _21(_21), _22(_22), _31(_31), _32(_32)
{
}

easy2d::Matrix32::Matrix32(Matrix32 const& other)
	: _11(other._11), _12(other._12)
	, _21(other._21), _22(other._22)
	, _31(other._31), _32(other._32)
{
}

float easy2d::Matrix32::operator [](unsigned int index) const
{
	return m[index];
}

void easy2d::Matrix32::identity()
{
	_11 = 1.f; _12 = 0.f;
	_21 = 0.f; _12 = 1.f;
	_31 = 0.f; _32 = 0.f;
}

easy2d::Vector2 easy2d::Matrix32::transform(const Vector2& v) const
{
	return Vector2(
		v.x * _11 + v.y * _21 + _31,
		v.x * _12 + v.y * _22 + _32
	);
}

easy2d::Rect easy2d::Matrix32::transform(const Rect& rect) const
{
	Vector2 top_left = transform(rect.getLeftTop());
	Vector2 top_right = transform(rect.getRightTop());
	Vector2 bottom_left = transform(rect.getLeftBottom());
	Vector2 bottom_right = transform(rect.getRightBottom());

	float left = min(min(top_left.x, top_right.x), min(bottom_left.x, bottom_right.x));
	float right = max(max(top_left.x, top_right.x), max(bottom_left.x, bottom_right.x));
	float top = min(min(top_left.y, top_right.y), min(bottom_left.y, bottom_right.y));
	float bottom = max(max(top_left.y, top_right.y), max(bottom_left.y, bottom_right.y));

	return Rect{ left, top, (right - left), (bottom - top) };
}

void easy2d::Matrix32::translate(float x, float y)
{
	_31 += _11 * x + _21 * y;
	_32 += _12 * x + _22 * y;
}

void easy2d::Matrix32::translate(const Vector2& v)
{
	translate(v.x, v.y);
}

float easy2d::Matrix32::determinant() const
{
	return (_11 * _22) - (_12 * _21);
}

bool easy2d::Matrix32::isIdentity() const
{
	return	_11 == 1.f && _12 == 0.f &&
		_21 == 0.f && _22 == 1.f &&
		_31 == 0.f && _32 == 0.f;
}

bool easy2d::Matrix32::isInvertible() const
{
	return 0 != determinant();
}

D2D1::Matrix3x2F const& easy2d::Matrix32::toD2DMatrix() const
{
	return reinterpret_cast<D2D1::Matrix3x2F const&>(*this);
}

easy2d::Matrix32 easy2d::Matrix32::translation(
	float x,
	float y)
{
	return easy2d::Matrix32::Matrix32(
		1.f, 0.f,
		0.f, 1.f,
		x, y
	);
}

easy2d::Matrix32 easy2d::Matrix32::scaling(
	float x,
	float y,
	const Point& center)
{
	return easy2d::Matrix32::Matrix32(
		x, 0.f,
		0.f, y,
		center.x - x * center.x,
		center.y - y * center.y
	);
}

easy2d::Matrix32 easy2d::Matrix32::rotation(
	float angle,
	const Point& center)
{
	float s = math::Sin(angle);
	float c = math::Cos(angle);
	return easy2d::Matrix32::Matrix32(
		c, s,
		-s, c,
		center.x * (1 - c) + center.y * s,
		center.y * (1 - c) - center.x * s
	);
}

easy2d::Matrix32 easy2d::Matrix32::skewing(
	float angle_x,
	float angle_y,
	const Point& center)
{
	float tx = math::Tan(angle_x);
	float ty = math::Tan(angle_y);
	return easy2d::Matrix32::Matrix32(
		1.f, -ty,
		-tx, 1.f,
		center.y * tx, center.x * ty
	);
}

easy2d::Matrix32 easy2d::Matrix32::invert(Matrix32 const& matrix)
{
	float det = 1.f / matrix.determinant();

	return easy2d::Matrix32::Matrix32(
		det * matrix._22,
		-det * matrix._12,
		-det * matrix._21,
		det * matrix._11,
		det * (matrix._21 * matrix._32 - matrix._22 * matrix._31),
		det * (matrix._12 * matrix._31 - matrix._11 * matrix._32)
	);
}
