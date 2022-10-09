#pragma once
#include <easy2d/e2dnode.h>

namespace easy2d
{

class ShapeMaker;

// 形状
class Shape :
	public Object
{
public:
	static Shape* NewMaker();

	static Shape* NewLine(Point begin, Point end);

	static Shape* NewRectangle(Point begin, Point end);

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
	Shape() = default;

	virtual ~Shape();

protected:
	ID2D1Geometry* geo_ = nullptr;
};

/// \~chinese
/// @brief 形状生成器
class ShapeMaker : public Object
{
public:
	/// \~chinese
	/// @brief 形状合并方式
	enum class CombineMode
	{
		Union,      ///< 并集 (A + B)
		Intersect,  ///< 交集 (A + B)
		Xor,        ///< 对称差集 ((A - B) + (B - A))
		Exclude     ///< 差集 (A - B)
	};

	// 合并形状
	static Shape* combine(
		Shape* shape1,
		Shape* shape2,
		CombineMode mode,					// 合并方式
		const Matrix32* matrix = nullptr	// 应用到输入形状
	);

	ShapeMaker();

	virtual ~ShapeMaker();

	// 获取生成的形状
	Shape* getShape();

	// 开始添加路径
	void beginPath(
		const Point& begin_pos = Point()	// 路径起始点
	);

	// 结束路径
	void endPath(
		bool closed = false					// 路径是否闭合
	);

	// 添加一条线段
	void addLine(const Point& point);

	// 添加多条线段
	void addLines(std::initializer_list<Point> points);

	// 添加多条线段
	void addLines(Point* points, int count);

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
		bool clockwise = true,	// 顺时针 or 逆时针
		bool is_small = true	// 是否取小于 180° 的弧
	);

	// 清空图形
	void clear();

private:
	void openSink();

	void closeSink();

	bool isSinkOpened() const;

private:
	Shape* shape_;
};

// 形状
class ShapeNode :
	public Node
{
public:
	// 形状样式
	enum class Style
	{
		Solid,		/* 填充 */
		Round,		/* 轮廓 */
		Fill,		/* 轮廓 + 填充 */
	};

public:
	ShapeNode();

	virtual ~ShapeNode();

	// 获取样式
	Style getStyle() const;

	// 获取填充颜色
	Color getFillColor() const;

	// 获取线条颜色
	Color getLineColor() const;

	// 获取线条宽度
	float getStrokeWidth() const;

	// 设置填充颜色
	void setFillColor(
		Color fillColor
	);

	// 设置线条颜色
	void setLineColor(
		Color lineColor
	);

	// 设置线条宽度
	void setStrokeWidth(
		float strokeWidth
	);

	// 设置样式
	void setStyle(Style style);

	// 设置线条相交样式
	void setLineJoin(
		LineJoin lineJoin
	);

	// 渲染形状
	virtual void onRender() override;

protected:
	// 渲染轮廓
	virtual void _renderLine() = 0;

	// 渲染填充色
	virtual void _renderFill() = 0;

protected:
	Style	_style;
	float	_strokeWidth;
	Color	_lineColor;
	Color	_fillColor;
	ID2D1StrokeStyle * _strokeStyle;
};


// 矩形
class RectShape :
	public ShapeNode
{
public:
	RectShape();

	explicit RectShape(
		Size size		/* 宽度和高度 */
	);

	explicit RectShape(
		Point topLeft,	/* 左上角坐标 */
		Size size		/* 宽度和高度 */
	);

	virtual ~RectShape();

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;
};


// 圆角矩形
class RoundRectShape :
	public ShapeNode
{
public:
	RoundRectShape();

	explicit RoundRectShape(
		Size size,		/* 宽度和高度 */
		float radiusX,	/* 圆角半径 */
		float radiusY	/* 圆角半径 */
	);

	explicit RoundRectShape(
		Point topLeft,	/* 左上角坐标 */
		Size size,		/* 宽度和高度 */
		float radiusX,	/* 圆角半径 */
		float radiusY	/* 圆角半径 */
	);

	virtual ~RoundRectShape();

	// 获取圆角半径
	float getRadiusX() const;

	// 获取圆角半径
	float getRadiusY() const;

	// 设置圆角半径
	virtual void setRadiusX(
		float radiusX
	);

	// 设置圆角半径
	virtual void setRadiusY(
		float radiusY
	);

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float _radiusX;
	float _radiusY;
};


// 圆形
class CircleShape :
	public ShapeNode
{
public:
	CircleShape();

	explicit CircleShape(
		float radius	/* 半径 */
	);

	explicit CircleShape(
		Point center,	/* 圆心坐标 */
		float radius	/* 半径 */
	);

	virtual ~CircleShape();

	// 获取半径
	float getRadius() const;

	// 设置半径
	virtual void setRadius(
		float radius
	);

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float _radius;
};


// 椭圆形
class EllipseShape :
	public ShapeNode
{
public:
	EllipseShape();

	explicit EllipseShape(
		float radiusX,	/* 横轴半径 */
		float radiusY	/* 纵轴半径 */
	);

	explicit EllipseShape(
		Point center,	/* 圆心坐标 */
		float radiusX,	/* 横轴半径 */
		float radiusY	/* 纵轴半径 */
	);

	virtual ~EllipseShape();

	// 获取横轴半径
	float getRadiusX() const;

	// 获取纵轴半径
	float getRadiusY() const;

	// 设置横轴半径
	virtual void setRadiusX(
		float radiusX
	);

	// 设置纵轴半径
	virtual void setRadiusY(
		float radiusY
	);

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float _radiusX;
	float _radiusY;
};

}