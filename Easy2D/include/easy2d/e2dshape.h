#pragma once
#include <easy2d/e2dbase.h>

namespace easy2d
{

class ShapeMaker;
class ShapeNode;
class CanvasBrush;

// ��״
class Shape :
	public Object
{
	friend ShapeMaker;
	friend ShapeNode;
	friend CanvasBrush;

public:
	// ����ֱ��
	static Shape* createLine(Point begin, Point end);

	// ��������
	static Shape* createRect(const Rect& rect);

	// ����Բ�Ǿ���
	static Shape* createRoundedRect(
		const Rect& rect,		// ����
		const Vector2& radius	// ����Բ�ǰ뾶
	);

	// ����Բ��
	static Shape* createCircle(
		const Point& center,	// ԭ��
		float radius			// �뾶
	);

	// ������Բ��
	static Shape* createEllipse(
		const Point& center,	// ԭ��
		const Vector2& radius	// �뾶
	);

	// ���������
	static Shape* createPolygon(
		std::initializer_list<Point> vertices	// ����ζ���
	);

	// ���������
	static Shape* createPolygon(
		const Point* vertices,
		int count
	);

	// ��ȡ���а�Χ��
	Rect getBoundingBox(
		const Matrix32* transform = nullptr	// Ӧ�õ���״�ϵĶ�ά�任
	) const;

	// �ж�ͼ���Ƿ������
	bool containsPoint(
		const Point& point,					// ��
		const Matrix32* transform = nullptr	// Ӧ�õ����ϵĶ�ά�任
	) const;

	// ��ȡͼ��չ����һ��ֱ�ߵĳ���
	float getLength() const;

	// ����ͼ�����
	float computeArea() const;

	// ����ͼ���ϵ��λ�ú���������
	bool computePointAtLength(
		float length,		// ����ͼ���ϵ�λ�ã���Χ [0.0 - 1.0]
		Point& point,		// ���λ��
		Vector2& tangent	// �����������
	) const;

protected:
	Shape(ID2D1Geometry* geo = nullptr);

	virtual ~Shape();

	void resetGeometry(ID2D1Geometry* geo);

protected:
	ID2D1Geometry* _geo;
};

// ��״������
class ShapeMaker : public Object
{
public:
	// ��״�ϲ���ʽ
	enum class CombineMode
	{
		Union,      // ���� (A + B)
		Intersect,  // ���� (A + B)
		Xor,        // �ԳƲ ((A - B) + (B - A))
		Exclude     // � (A - B)
	};

	// �ϲ���״
	static Shape* combine(
		Shape* shape1,
		Shape* shape2,
		CombineMode mode,					// �ϲ���ʽ
		const Matrix32* matrix = nullptr	// Ӧ�õ�������״
	);

	ShapeMaker() = default;

	virtual ~ShapeMaker();

	// ��ȡ���ɵ���״
	Shape* getShape() const;

	// ��ʼ���·��
	void beginPath(
		const Point& point = Point()	// ·����ʼ��
	);

	// ����·��
	void endPath(
		bool closed = false				// ·���Ƿ�պ�
	);

	// ���һ���߶�
	void addLine(const Point& point);

	// ��Ӷ����߶�
	void addLines(std::initializer_list<Point> points);

	// ��Ӷ����߶�
	void addLines(const Point* points, int count);

	// ���һ�����η�����������
	void addBezier(
		const Point& point1,	// ���������ߵĵ�һ�����Ƶ�
		const Point& point2,	// ���������ߵĵڶ������Ƶ�
		const Point& point3		// ���������ߵ��յ�
	);

	// ��ӻ���
	void addArc(
		const Point& point,		// �յ�
		const Size& radius,		// ��Բ�뾶
		float rotation,			// ��Բ��ת�Ƕ�
		bool clockwise = true,	// �Ƿ�˳ʱ��
		bool smallSize = true	// �Ƿ�ȡС�� 180�� �Ļ�
	);

protected:
	void openSink();

	void closeSink();

protected:
	Shape* _shape = nullptr;
	ID2D1GeometrySink* _sink = nullptr;
};

}