#pragma once
#include <easy2d/e2dnode.h>

namespace easy2d
{

class ShapeMaker;

// ��״
class Shape :
	public Object
{
public:
	static Shape* NewMaker();

	static Shape* NewLine(Point begin, Point end);

	static Shape* NewRectangle(Point begin, Point end);

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
	Shape() = default;

	virtual ~Shape();

protected:
	ID2D1Geometry* geo_ = nullptr;
};

/// \~chinese
/// @brief ��״������
class ShapeMaker : public Object
{
public:
	/// \~chinese
	/// @brief ��״�ϲ���ʽ
	enum class CombineMode
	{
		Union,      ///< ���� (A + B)
		Intersect,  ///< ���� (A + B)
		Xor,        ///< �ԳƲ ((A - B) + (B - A))
		Exclude     ///< � (A - B)
	};

	// �ϲ���״
	static Shape* combine(
		Shape* shape1,
		Shape* shape2,
		CombineMode mode,					// �ϲ���ʽ
		const Matrix32* matrix = nullptr	// Ӧ�õ�������״
	);

	ShapeMaker();

	virtual ~ShapeMaker();

	// ��ȡ���ɵ���״
	Shape* getShape();

	// ��ʼ���·��
	void beginPath(
		const Point& begin_pos = Point()	// ·����ʼ��
	);

	// ����·��
	void endPath(
		bool closed = false					// ·���Ƿ�պ�
	);

	// ���һ���߶�
	void addLine(const Point& point);

	// ��Ӷ����߶�
	void addLines(std::initializer_list<Point> points);

	// ��Ӷ����߶�
	void addLines(Point* points, int count);

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
		bool clockwise = true,	// ˳ʱ�� or ��ʱ��
		bool is_small = true	// �Ƿ�ȡС�� 180�� �Ļ�
	);

	// ���ͼ��
	void clear();

private:
	void openSink();

	void closeSink();

	bool isSinkOpened() const;

private:
	Shape* shape_;
};

// ��״
class ShapeNode :
	public Node
{
public:
	// ��״��ʽ
	enum class Style
	{
		Solid,		/* ��� */
		Round,		/* ���� */
		Fill,		/* ���� + ��� */
	};

public:
	ShapeNode();

	virtual ~ShapeNode();

	// ��ȡ��ʽ
	Style getStyle() const;

	// ��ȡ�����ɫ
	Color getFillColor() const;

	// ��ȡ������ɫ
	Color getLineColor() const;

	// ��ȡ�������
	float getStrokeWidth() const;

	// ���������ɫ
	void setFillColor(
		Color fillColor
	);

	// ����������ɫ
	void setLineColor(
		Color lineColor
	);

	// �����������
	void setStrokeWidth(
		float strokeWidth
	);

	// ������ʽ
	void setStyle(Style style);

	// ���������ཻ��ʽ
	void setLineJoin(
		LineJoin lineJoin
	);

	// ��Ⱦ��״
	virtual void onRender() override;

protected:
	// ��Ⱦ����
	virtual void _renderLine() = 0;

	// ��Ⱦ���ɫ
	virtual void _renderFill() = 0;

protected:
	Style	_style;
	float	_strokeWidth;
	Color	_lineColor;
	Color	_fillColor;
	ID2D1StrokeStyle * _strokeStyle;
};


// ����
class RectShape :
	public ShapeNode
{
public:
	RectShape();

	explicit RectShape(
		Size size		/* ��Ⱥ͸߶� */
	);

	explicit RectShape(
		Point topLeft,	/* ���Ͻ����� */
		Size size		/* ��Ⱥ͸߶� */
	);

	virtual ~RectShape();

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;
};


// Բ�Ǿ���
class RoundRectShape :
	public ShapeNode
{
public:
	RoundRectShape();

	explicit RoundRectShape(
		Size size,		/* ��Ⱥ͸߶� */
		float radiusX,	/* Բ�ǰ뾶 */
		float radiusY	/* Բ�ǰ뾶 */
	);

	explicit RoundRectShape(
		Point topLeft,	/* ���Ͻ����� */
		Size size,		/* ��Ⱥ͸߶� */
		float radiusX,	/* Բ�ǰ뾶 */
		float radiusY	/* Բ�ǰ뾶 */
	);

	virtual ~RoundRectShape();

	// ��ȡԲ�ǰ뾶
	float getRadiusX() const;

	// ��ȡԲ�ǰ뾶
	float getRadiusY() const;

	// ����Բ�ǰ뾶
	virtual void setRadiusX(
		float radiusX
	);

	// ����Բ�ǰ뾶
	virtual void setRadiusY(
		float radiusY
	);

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float _radiusX;
	float _radiusY;
};


// Բ��
class CircleShape :
	public ShapeNode
{
public:
	CircleShape();

	explicit CircleShape(
		float radius	/* �뾶 */
	);

	explicit CircleShape(
		Point center,	/* Բ������ */
		float radius	/* �뾶 */
	);

	virtual ~CircleShape();

	// ��ȡ�뾶
	float getRadius() const;

	// ���ð뾶
	virtual void setRadius(
		float radius
	);

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float _radius;
};


// ��Բ��
class EllipseShape :
	public ShapeNode
{
public:
	EllipseShape();

	explicit EllipseShape(
		float radiusX,	/* ����뾶 */
		float radiusY	/* ����뾶 */
	);

	explicit EllipseShape(
		Point center,	/* Բ������ */
		float radiusX,	/* ����뾶 */
		float radiusY	/* ����뾶 */
	);

	virtual ~EllipseShape();

	// ��ȡ����뾶
	float getRadiusX() const;

	// ��ȡ����뾶
	float getRadiusY() const;

	// ���ú���뾶
	virtual void setRadiusX(
		float radiusX
	);

	// ��������뾶
	virtual void setRadiusY(
		float radiusY
	);

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float _radiusX;
	float _radiusY;
};

}