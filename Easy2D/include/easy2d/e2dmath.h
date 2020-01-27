#pragma once
#include <easy2d/e2dmacros.h>

namespace easy2d
{
	namespace math
	{
		namespace constants
		{
			const auto PI_F = 3.141592653589793f;
			const auto PI_F_2 = 1.570796326794896f;
			const auto PI_F_X_2 = 6.283185307179586f;

			const auto PI_D = 3.14159265358979323846;
			const auto PI_D_2 = 1.57079632679489661923;
			const auto PI_D_X_2 = 6.28318530717958647692;
		}

		inline int Abs(int val) { return ::abs(val); }

		inline float Abs(float val) { return ::fabsf(val); }

		inline double Abs(double val) { return ::fabs(val); }

		inline float Sqrt(float val) { return ::sqrtf(val); }

		inline double Sqrt(double val) { return ::sqrt(val); }

		inline float Pow(float base, float exponent) { return ::powf(base, exponent); }

		inline double Pow(double base, double exponent) { return ::pow(base, exponent); }

		inline int Sign(int val) { return val < 0 ? -1 : 1; }

		inline float Sign(float val) { return val < 0 ? -1.f : 1.f; }

		inline double Sign(double val) { return val < 0 ? -1.0 : 1.0; }

		inline float Sin(float val) { return ::sinf(val * constants::PI_F / 180.f); }

		inline double Sin(double val) { return ::sin(val * constants::PI_D / 180.0); }

		inline float Cos(float val) { return ::cosf(val * constants::PI_F / 180.f); }

		inline double Cos(double val) { return ::cos(val * constants::PI_D / 180.0); }

		inline float Tan(float val) { return ::tanf(val * constants::PI_F / 180.f); }

		inline double Tan(double val) { return ::tan(val * constants::PI_D / 180.0); }

		inline float Asin(float val) { return ::asinf(val) * 180.f / constants::PI_F; }

		inline double Asin(double val) { return ::asin(val) * 180.f / constants::PI_F; }

		inline float Acos(float val) { return ::acosf(val) * 180.f / constants::PI_F; }

		inline double Acos(double val) { return ::acos(val) * 180.f / constants::PI_F; }

		inline float Atan(float val) { return ::atanf(val) * 180.f / constants::PI_F; }

		inline double Atan(double val) { return ::atan(val) * 180.f / constants::PI_F; }

		inline float Ceil(float val) { return ::ceil(val); }

		inline double Ceil(double val) { return ::ceil(val); }

		inline float Floor(float val) { return ::floor(val); }

		inline double Floor(double val) { return ::floor(val); }
	}

	class Size;

	// ����
	class Point
	{
	public:
		float x;	// X ����
		float y;	// Y ����

	public:
		Point();

		Point(float x, float y);

		Point(const Point& other);

		Point operator + (Point const& point) const;
		Point operator - (Point const& point) const;
		Point operator * (float const& point) const;
		Point operator / (float const& point) const;
		Point operator - () const;
		bool operator== (const Point& point) const;

		operator easy2d::Size() const;

		// �ж���������
		static float distance(const Point&, const Point&);
	};


	// ��ά����
	using Vector2 = Point;

	// ��С
	class Size
	{
	public:
		float width;	// ���
		float height;	// �߶�

	public:
		Size();

		Size(float width, float height);

		Size(const Size& other);

		Size operator + (Size const& size) const;
		Size operator - (Size const& size) const;
		Size operator * (float const& size) const;
		Size operator / (float const& size) const;
		Size operator - () const;
		bool operator== (const Size& size) const;

		operator easy2d::Point() const;
	};


	// ����
	class Rect
	{
	public:
		Point origin;	// ԭ������
		Size  size;		// ��Ⱥ͸߶�

	public:
		Rect();

		Rect(float x, float y, float width, float height);

		Rect(const Point& pos, const Size& size);

		Rect(const Rect& other);

		Rect& operator= (const Rect& other);

		bool operator== (const Rect& rect) const;

		// ���þ���
		void setRect(
			float x,
			float y,
			float width,
			float height
		);

		// �жϵ��Ƿ��ھ�����
		bool containsPoint(
			const Point& point
		) const;

		// �ж��������Ƿ��ཻ
		bool intersects(
			const Rect& rect
		) const;

		inline Vector2 getCenter() const { return Vector2{ origin.x + size.width / 2, origin.y + size.height / 2 }; }

		inline Vector2 getLeftTop() const { return origin; }

		inline Vector2 getRightBottom() const { return Vector2{ getRight(), getBottom() }; }

		inline Vector2 getRightTop() const { return Vector2{ getRight(), getTop() }; }

		inline Vector2 getLeftBottom() const { return Vector2{ getLeft(), getBottom() }; }

		inline float getLeft() const { return origin.x; }

		inline float getTop() const { return origin.y; }

		inline float getRight() const { return origin.x + size.width; }

		inline float getBottom() const { return origin.y + size.height; }
	};


	// ��ά�任����
	template <typename _Lty, typename _Rty>
	struct MatrixMultiply;

	struct Matrix32
	{
		union
		{
			struct
			{
				float m[6];  // m[3][2]
			};

			struct
			{
				float
					_11, _12,
					_21, _22,
					_31, _32;
			};
		};

		Matrix32();

		Matrix32(float _11, float _12, float _21, float _22, float _31, float _32);

		Matrix32(Matrix32 const& other);

		template <typename T>
		Matrix32(T const& other)
		{
			for (int i = 0; i < 6; i++)
				m[i] = other[i];
		}

		template <typename _Lty, typename _Rty>
		inline Matrix32& operator= (MatrixMultiply<_Lty, _Rty> const& other)
		{
			Matrix32 result(other);
			(*this) = result;
			return *this;
		}

		float operator [](unsigned int index) const;


		void identity();

		Vector2 transform(const Vector2& v) const;

		Rect transform(const Rect& rect) const;

		void translate(float x, float y);

		void translate(const Vector2& v);

		float determinant() const;

		bool isIdentity() const;

		bool isInvertible() const;

		D2D1::Matrix3x2F const& toD2DMatrix() const;

		static Matrix32 translation(
			float x,
			float y);

		static Matrix32 scaling(
			float x,
			float y,
			const Point& center = Point());

		static Matrix32 rotation(
			float angle,
			const Point& center = Point());

		static Matrix32 skewing(
			float angle_x,
			float angle_y,
			const Point& center = Point());

		static Matrix32 invert(Matrix32 const& matrix);
	};


	// ʹ��ģ����ʽ�Ż�����˷�
	template <typename _Lty, typename _Rty>
	struct MatrixMultiply
	{
		_Lty const& lhs;
		_Rty const& rhs;

		MatrixMultiply(_Lty const& lhs, _Rty const& rhs)
			: lhs(lhs)
			, rhs(rhs)
		{}

		inline float operator [](unsigned int index) const
		{
			switch (index)
			{
			case 0:
				return lhs[0] * rhs[0] + lhs[1] * rhs[2];
			case 1:
				return lhs[0] * rhs[1] + lhs[1] * rhs[3];
			case 2:
				return lhs[2] * rhs[0] + lhs[3] * rhs[2];
			case 3:
				return lhs[2] * rhs[1] + lhs[3] * rhs[3];
			case 4:
				return lhs[4] * rhs[0] + lhs[5] * rhs[2] + rhs[4];
			case 5:
				return lhs[4] * rhs[1] + lhs[5] * rhs[3] + rhs[5];
			default:
				return 0.f;
			}
		}
	};

	inline
	MatrixMultiply<Matrix32, Matrix32>
	operator *(Matrix32 const& lhs, Matrix32 const& rhs)
	{
		return MatrixMultiply<Matrix32, Matrix32>(lhs, rhs);
	}

	template <typename _Lty, typename _Rty>
	inline
	MatrixMultiply<MatrixMultiply<_Lty, _Rty>, Matrix32>
	operator *(MatrixMultiply<_Lty, _Rty> const& lhs, Matrix32 const& rhs)
	{
		return MatrixMultiply<MatrixMultiply<_Lty, _Rty>, Matrix32>(lhs, rhs);
	}
}