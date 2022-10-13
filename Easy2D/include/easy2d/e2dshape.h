#pragma once
#include <easy2d/e2dbase.h>

namespace easy2d
{

class ShapeMaker;
class ShapeNode;
class CanvasBrush;

// 形状
class Shape :
	public Object
{
	friend ShapeMaker;
	friend ShapeNode;
	friend CanvasBrush;

public:
	// 创建直线
	static Shape* createLine(Point begin, Point end);

	// 创建矩形
	static Shape* createRect(const Rect& rect);

	// 创建圆角矩形
	static Shape* createRoundedRect(
		const Rect& rect,		// 矩形
		const Vector2& radius	// 矩形圆角半径
	);

	// 创建圆形
	static Shape* createCircle(
		const Point& center,	// 原点
		float radius			// 半径
	);

	// 创建椭圆形
	static Shape* createEllipse(
		const Point& center,	// 原点
		const Vector2& radius	// 半径
	);

	// 创建多边形
	static Shape* createPolygon(
		std::initializer_list<Point> vertices	// 多边形顶点
	);

	// 创建多边形
	static Shape* createPolygon(
		const Point* vertices,
		int count
	);

	// 获取外切包围盒
	Rect getBoundingBox(
		const Matrix32* transform = nullptr	// 应用到形状上的二维变换
	) const;

	// 判断图形是否包含点
	bool containsPoint(
		const Point& point,					// 点
		const Matrix32* transform = nullptr	// 应用到点上的二维变换
	) const;

	// 获取图形展开成一条直线的长度
	float getLength() const;

	// 计算图形面积
	float computeArea() const;

	// 计算图形上点的位置和切线向量
	bool computePointAtLength(
		float length,		// 点在图形上的位置，范围 [0.0 - 1.0]
		Point& point,		// 点的位置
		Vector2& tangent	// 点的切线向量
	) const;

protected:
	Shape(ID2D1Geometry* geo = nullptr);

	virtual ~Shape();

	void resetGeometry(ID2D1Geometry* geo);

protected:
	ID2D1Geometry* _geo;
};

// 形状生成器
class ShapeMaker : public Object
{
public:
	// 形状合并方式
	enum class CombineMode
	{
		Union,      // 并集 (A + B)
		Intersect,  // 交集 (A + B)
		Xor,        // 对称差集 ((A - B) + (B - A))
		Exclude     // 差集 (A - B)
	};

	// 合并形状
	static Shape* combine(
		Shape* shape1,
		Shape* shape2,
		CombineMode mode,					// 合并方式
		const Matrix32* matrix = nullptr	// 应用到输入形状
	);

	ShapeMaker() = default;

	virtual ~ShapeMaker();

	// 获取生成的形状
	Shape* getShape() const;

	// 开始添加路径
	void beginPath(
		const Point& point = Point()	// 路径起始点
	);

	// 结束路径
	void endPath(
		bool closed = false				// 路径是否闭合
	);

	// 添加一条线段
	void addLine(const Point& point);

	// 添加多条线段
	void addLines(std::initializer_list<Point> points);

	// 添加多条线段
	void addLines(const Point* points, int count);

	// 添加一条三次方贝塞尔曲线
	void addBezier(
		const Point& point1,	// 贝塞尔曲线的第一个控制点
		const Point& point2,	// 贝塞尔曲线的第二个控制点
		const Point& point3		// 贝塞尔曲线的终点
	);

	// 添加弧线
	void addArc(
		const Point& point,		// 终点
		const Size& radius,		// 椭圆半径
		float rotation,			// 椭圆旋转角度
		bool clockwise = true,	// 是否顺时针
		bool smallSize = true	// 是否取小于 180° 的弧
	);

protected:
	void openSink();

	void closeSink();

protected:
	Shape* _shape = nullptr;
	ID2D1GeometrySink* _sink = nullptr;
};

}