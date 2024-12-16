#pragma once
#include <easy2d/e2dbase.h>
#include <easy2d/e2dshape.h>
#include <easy2d/e2dtext.h>
#include <easy2d/e2dlistener.h>
#include <vector>

namespace easy2d 
{

class Action;
class KeyFrame;
class Transition;
class SceneManager;
class Renderer;
class Scene;

// 身体形状关系
enum class BodyRelation
{
	Disjoint,		// 完全不相交
	IsContained,	// 被对方包含
	Contains,		// 包含对方
	Overlap,		// 相交但不包含
};

class Node :
	public Object
{
	friend class Transition;
	friend class SceneManager;
	friend class Renderer;

public:
	// 节点属性
	struct Property
	{
		bool visable;		// 可见性
		Point pos;			// 坐标
		Size size;			// 宽高
		float opacity;		// 透明度
		Point anchor;		// 锚点
		Vector2 scale;		// 缩放
		float rotation;		// 旋转角度
		Vector2 skewAngle;	// 倾斜角度
	};

public:
	Node();

	virtual ~Node();

	// 更新节点
	virtual void onUpdate() {}

	// 渲染节点
	virtual void onRender() {}

	// 获取节点显示状态
	bool isVisible() const;

	// 获取节点绘图顺序
	int getOrder() const;

	// 获取节点横坐标
	float getPosX() const;

	// 获取节点纵坐标
	float getPosY() const;

	// 获取节点坐标
	Point getPos() const;

	// 获取节点宽度
	float getWidth() const;

	// 获取节点高度
	float getHeight() const;

	// 获取节点宽度（不考虑缩放）
	float getRealWidth() const;

	// 获取节点高度（不考虑缩放）
	float getRealHeight() const;

	// 获取节点大小（不考虑缩放）
	Size getRealSize() const;

	// 获取节点的锚点
	float getAnchorX() const;

	// 获取节点的锚点
	float getAnchorY() const;

	// 获取节点大小
	Size getSize() const;

	// 获取节点横向缩放比例
	float getScaleX() const;

	// 获取节点纵向缩放比例
	float getScaleY() const;

	// 获取节点横向倾斜角度
	float getSkewX() const;

	// 获取节点纵向倾斜角度
	float getSkewY() const;

	// 获取节点旋转角度
	float getRotation() const;

	// 获取节点透明度
	float getOpacity() const;

	// 获取节点属性
	Property getProperty() const;

	// 获取边框
	virtual Rect getBounds() const;

	// 获取外切包围盒
	virtual Rect getBoundingBox() const;

	// 获取二维变换矩阵
	Matrix32 getTransform() const;

	// 获取二维变换逆矩阵
	Matrix32 getInverseTransform() const;

	// 获取父节点
	Node * getParent() const;

	// 获取节点所在场景
	Scene * getParentScene() const;

	// 是否包含点坐标
	virtual bool containsPoint(Point const& point) const;

	// 设置节点是否显示
	void setVisible(
		bool value
	);

	// 开启或禁用 onUpdate 函数
	void setAutoUpdate(
		bool autoUpdate
	);

	// 设置节点横坐标
	void setPosX(
		float x
	);

	// 设置节点纵坐标
	void setPosY(
		float y
	);

	// 设置节点坐标
	void setPos(
		const Point & point
	);

	// 设置节点坐标
	void setPos(
		float x,
		float y
	);

	// 移动节点
	void movePosX(
		float x
	);

	// 移动节点
	void movePosY(
		float y
	);

	// 移动节点
	void movePos(
		float x,
		float y
	);

	// 移动节点
	void movePos(
		const Vector2 & v
	);

	// 设置节点绘图顺序，数字大的会显示在其他兄弟节点上方
	// 默认为 0
	void setOrder(
		int order
	);

	// 设置横向缩放比例
	// 默认为 1.0f
	void setScaleX(
		float scaleX
	);

	// 设置纵向缩放比例
	// 默认为 1.0f
	void setScaleY(
		float scaleY
	);

	// 设置缩放比例
	// 默认为 (1.0f, 1.0f)
	void setScale(
		float scaleX,
		float scaleY
	);

	// 设置缩放比例
	// 默认为 1.0f
	void setScale(
		float scale
	);

	// 设置横向倾斜角度
	// 默认为 0
	void setSkewX(
		float angleX
	);

	// 设置纵向倾斜角度
	// 默认为 0
	void setSkewY(
		float angleY
	);

	// 设置倾斜角度
	// 默认为 (0, 0)
	void setSkew(
		float angleX,
		float angleY
	);

	// 设置旋转角度
	// 默认为 0
	void setRotation(
		float rotation
	);

	// 设置透明度
	// 默认为 1.0f, 范围 [0, 1]
	void setOpacity(
		float opacity
	);

	// 设置锚点的横向位置
	// 默认为 0, 范围 [0, 1]
	void setAnchorX(
		float anchorX
	);

	// 设置锚点的纵向位置
	// 默认为 0, 范围 [0, 1]
	void setAnchorY(
		float anchorY
	);

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	void setAnchor(
		float anchorX,
		float anchorY
	);

	// 修改节点宽度
	void setWidth(
		float width
	);

	// 修改节点高度
	void setHeight(
		float height
	);

	// 修改节点大小
	void setSize(
		float width,
		float height
	);

	// 修改节点大小
	void setSize(
		Size size
	);

	// 设置节点属性
	void setProperty(
		Property prop
	);

	// 添加子节点
	void addChild(
		Node * child
	);

	// 添加子节点
	void addChild(
		Node* child,
		int order		/* 渲染顺序，数字大的会显示在其他兄弟节点上方 */
	);

	// 添加多个子节点
	void addChildren(
		std::initializer_list<Node*> nodes	/* 节点数组 */
	);

	// 添加多个子节点
	void addChildren(
		std::initializer_list<Node*> nodes,	/* 节点数组 */
		int order							/* 渲染顺序，数字大的会显示在其他兄弟节点上方 */
	);

	// 获取所有名称相同的子节点
	std::vector<Node*> getChildren(
		const String& name
	) const;

	// 获取名称相同的子节点
	Node* getChild(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& getAllChildren() const;

	// 获取子节点数量
	int getChildrenCount() const;

	// 移除子节点
	bool removeChild(
		Node* child
	);

	// 移除所有名称相同的子节点
	void removeChildren(
		const String& childName
	);

	// 移除所有节点
	void removeAllChildren();

	// 从父节点移除自身（在下一次更新时执行）
	void removeSelfInNextUpdate();

	// 执行动作
	void runAction(
		Action * action
	);

	// 继续动作
	void resumeAction(
		const String& name
	);

	// 暂停动作
	void pauseAction(
		const String& name
	);

	// 停止动作
	void stopAction(
		const String& name
	);

	// 继续所有暂停动作
	void resumeAllActions();

	// 暂停所有动作
	void pauseAllActions();

	// 停止所有动作
	void stopAllActions();

	// 添加输入监听
	Listener* addListener(
		const Listener::Callback& func,	/* 监听到用户输入时的执行函数 */
		const String& name = {},		/* 监听器名称 */
		bool paused = false				/* 是否暂停 */
	);

	// 添加碰撞监听
	void addListener(
		ListenerBase* listener		/* 监听器 */
	);

	// 移除监听器
	void removeListener(
		ListenerBase* listener		/* 监听器 */
	);

	// 启动输入监听
	void startListener(
		const String& name
	);

	// 停止输入监听
	void stopListener(
		const String& name
	);

	// 移除输入监听
	void removeListener(
		const String& name
	);

	// 启动所有监听器
	void startAllListeners();

	// 停止所有监听器
	void stopAllListeners();

	// 移除所有监听器
	void removeAllListeners();

	// 修改节点的默认锚点位置
	static void setDefaultAnchor(
		float defaultAnchorX,
		float defaultAnchorY
	);

	// 获取身体形状
	Shape* getBodyShape() const;

	// 设置身体形状
	void setBodyShape(Shape* shape);

	// 渲染身体形状
	void showBodyShape(bool enabled = true);

	// 判断和另一节点的身体形状关系
	BodyRelation compareWithBody(Node* other) const;

protected:
	// 更新节点
	void _update();

	// 渲染节点
	void _render();

	void _renderBodyShape();

	// 设置节点所在场景
	void _setParentScene(
		Scene * scene
	);

	// 更新二维变换矩阵
	void _updateTransform() const;

	// 更新二维变换逆矩阵
	void _updateInverseTransform() const;

	// 子节点排序
	void _sortChildren();

	// 更新节点透明度
	void _updateOpacity();

private:
	// 更新监听器
	void __updateListeners();

	void __dispatchEvent(Event* evt);

	// 清空监听器
	void __clearListeners();

	void __clearParents();

protected:
	bool		_visible;
	bool		_autoUpdate;
	bool		_needSort;
	bool		_showBodyShape;
	bool		_removed;
	int			_order;
	float		_rotation;
	float		_displayOpacity;
	float		_realOpacity;
	Point		_pos;
	Size		_size;
	Vector2		_scale;
	Vector2		_skewAngle;
	Point		_anchor;
	Scene*		_parentScene;
	Node*		_parent;
	Shape*		_body;

	std::vector<Node*>	_children;
	std::vector<ListenerBase*> _listeners;

	mutable bool		_dirtyTransform;
	mutable Matrix32	_transform;
	mutable bool		_dirtyInverseTransform;
	mutable Matrix32	_inverseTransform;
};


// 场景
class Scene :
	public Node
{
public:
	Scene();

	virtual ~Scene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

	// 重写这个函数，它将在关闭窗口时执行（返回 false 将阻止窗口关闭）
	virtual bool onCloseWindow() { return true; }
};


// 精灵
class Sprite :
	public Node
{
public:
	Sprite();

	explicit Sprite(
		Image * image
	);

	explicit Sprite(
		const String& filePath	/* 图片文件路径 */
	);

	explicit Sprite(
		const Resource& res		/* 图片资源 */
	);

	explicit Sprite(
		KeyFrame* frame			/* 关键帧 */
	);

	Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	Sprite(
		const String& filePath,	/* 图片文件路径 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	Sprite(
		const Resource& res,	/* 图片资源 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType,	/* 图片资源类型 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	virtual ~Sprite();

	// 加载图片文件
	bool open(
		const String& filePath
	);

	// 加载图片资源
	bool open(
		const Resource& res		/* 图片资源 */
	);

	// 加载图片资源
	bool open(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 获取裁剪矩形
	Rect getCropRect() const;

	// 将图片裁剪为矩形
	void crop(
		const Rect& cropRect	/* 裁剪矩形，传空矩形则不裁剪 */
	);

	// 获取图片
	Image* getImage() const;

	// 设置图片
	void setImage(
		Image* image,
		bool resetCropRect = true	/* 重置裁剪矩形 */
	);

	// 设置关键帧
	void setKeyFrame(
		KeyFrame* frame
	);

	// 获取像素插值方式
	InterpolationMode getInterpolationMode() const;

	// 设置像素插值方式
	void setInterpolationMode(InterpolationMode mode);

	// 渲染精灵
	virtual void onRender() override;

protected:
	Image*	_image;
	Rect	_cropRect;
	InterpolationMode _interpolationMode;
};


// 文本节点
class Text
	: public Node
{
public:
	Text();

	explicit Text(
		const String& text,									/* 文字内容 */
		const TextStyle& textStyle = TextStyle(),			/* 文本样式 */
		const DrawingStyle& drawingStyle = DrawingStyle()	/* 绘图样式 */
	);

	explicit Text(
		TextLayout* layout,									/* 文字布局 */
		const DrawingStyle& drawingStyle = DrawingStyle()	/* 绘图样式 */
	);

	virtual ~Text();

	// 获取文本布局
	TextLayout* getLayout() const;

	// 获取文本
	String getText() const;

	// 获取字体
	Font getFont() const;

	// 获取文本样式
	TextStyle getTextStyle() const;

	// 获取渲染样式
	DrawingStyle getDrawingStyle() const;

	// 获取文本显示行数
	int getLineCount() const;

	// 设置文本布局
	void setTextLayout(
		TextLayout* layout
	);

	// 设置文本
	void setText(
		const String& text
	);

	// 设置文本样式
	void setTextStyle(
		const TextStyle& style
	);

	// 设置渲染样式
	void setDrawingStyle(
		DrawingStyle style
	);

	// 设置字体
	void setFont(
		const Font& font
	);

	// 设置字体族
	void setFontFamily(
		const String& family
	);

	// 设置字号
	void setFontSize(
		float size
	);

	// 设置字体粗细值
	void setFontWeight(
		UINT weight
	);

	// 设置文字斜体
	void setItalic(
		bool italic
	);

	// 打开或关闭文本自动换行
	void setWrapping(
		bool wrapping
	);

	// 设置文本自动换行的宽度
	void setWrappingWidth(
		float wrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		float lineSpacing
	);

	// 设置对齐方式
	void setAlignment(
		TextAlign align
	);

	// 设置下划线
	void setUnderline(
		bool hasUnderline
	);

	// 设置删除线
	void setStrikethrough(
		bool hasStrikethrough
	);

	// 设置文字填充颜色
	void setFillColor(
		Color color
	);

	// 设置描边颜色
	void setStrokeColor(
		Color strokeColor
	);

	// 设置描边线宽
	void setStrokeWidth(
		float strokeWidth
	);

	// 设置描边线相交样式
	void setLineJoin(
		LineJoin lineJoin
	);

	// 渲染文字
	virtual void onRender() override;

protected:
	void updateLayout();

protected:
	TextLayout*	_layout;
	DrawingStyle _style;
};


// 形状节点
class ShapeNode :
	public Node
{
public:
	// 创建直线节点
	ShapeNode(
		Shape::LineType,
		Point begin,
		Point end,
		DrawingStyle style = DrawingStyle{}
	);

	// 创建矩形节点
	ShapeNode(
		Shape::RectType,
		const Size& size,		// 矩形宽高
		DrawingStyle style = DrawingStyle{}
	);

	// 创建圆角矩形节点
	ShapeNode(
		Shape::RoundedRectType,
		const Size& size,		// 矩形宽高
		const Vector2& radius,	// 矩形圆角半径
		DrawingStyle style = DrawingStyle{}
	);

	// 创建圆形节点
	ShapeNode(
		Shape::CircleType,
		float radius,			// 半径
		DrawingStyle style = DrawingStyle{}
	);

	// 创建椭圆形节点
	ShapeNode(
		Shape::EllipseType,
		const Vector2& radius,	// 半径
		DrawingStyle style = DrawingStyle{}
	);

	// 创建多边形节点
	ShapeNode(
		Shape::PolygonType,
		std::initializer_list<Point> vertices,	// 多边形顶点
		DrawingStyle style = DrawingStyle{}
	);

	// 创建多边形节点
	ShapeNode(
		Shape::PolygonType,
		const Point* vertices,
		int count,
		DrawingStyle style = DrawingStyle{}
	);

	ShapeNode(
		Shape* shape = nullptr,
		DrawingStyle style = DrawingStyle{}
	);

	virtual ~ShapeNode();

	// 获取形状
	Shape* getShape() const;

	// 设置形状
	void setShape(Shape* shape);

	// 获取填充颜色
	Color getFillColor() const;

	// 设置填充颜色
	void setFillColor(
		Color fillColor
	);

	// 获取线条颜色
	Color getStrokeColor() const;

	// 设置线条颜色
	void setStrokeColor(
		Color strokeColor
	);

	// 获取线条宽度
	float getStrokeWidth() const;

	// 设置线条宽度
	void setStrokeWidth(
		float strokeWidth
	);

	// 设置线条相交样式
	void setLineJoin(
		LineJoin lineJoin
	);

	// 获取绘图模式
	DrawingStyle::Mode getDrawingMode() const;

	// 设置绘图模式
	void setDrawingMode(DrawingStyle::Mode mode);

	// 获取绘图样式
	DrawingStyle getDrawingStyle() const;

	// 设置绘图样式
	void setDrawingStyle(DrawingStyle style);

	virtual Rect getBounds() const override;

	virtual Rect getBoundingBox() const override;

	virtual bool containsPoint(Point const& point) const override;

	virtual void onRender() override;

protected:
	Shape*			_shape;
	Rect			_bounds;
	DrawingStyle	_style;
};


class Canvas;

// 画布画笔
class CanvasBrush
	: public Object
{
	friend Canvas;

public:
	// 绘制形状
	void drawShape(
		Shape* shape
	);

	// 绘制图片
	void drawImage(
		Image* image,					/* 图像 */
		const Point& pos = Point(),		/* 绘制位置 */
		const Rect& cropRect = Rect{}	/* 裁剪矩形 */
	);

	// 绘制图片
	void drawImage(
		Image* image,					/* 图像 */
		const Rect& destRect,			/* 绘制区域 */
		const Rect& cropRect = Rect{}	/* 裁剪矩形 */
	);

	// 绘制关键帧
	void drawImage(
		KeyFrame* frame,				/* 关键帧 */
		const Point& pos = Point()		/* 绘制位置 */
	);

	// 绘制关键帧
	void drawImage(
		KeyFrame* frame,				/* 关键帧 */
		const Rect& destRect			/* 绘制区域 */
	);

	// 绘制文本
	void drawText(
		TextLayout* layout,
		const Point& pos = Point()
	);

	// 绘制文本
	void drawText(
		const String& text,
		const Point& pos = Point(),
		const TextStyle& style = TextStyle()
	);

	// 获取填充颜色
	Color getFillColor() const;

	// 设置填充颜色
	void setFillColor(
		Color fillColor
	);

	// 获取线条颜色
	Color getStrokeColor() const;

	// 设置线条颜色
	void setStrokeColor(
		Color strokeColor
	);

	// 获取线条宽度
	float getStrokeWidth() const;

	// 设置线条宽度
	void setStrokeWidth(
		float strokeWidth
	);

	// 设置线条相交样式
	void setLineJoin(
		LineJoin lineJoin
	);

	// 获取绘图模式
	DrawingStyle::Mode getDrawingMode() const;

	// 设置绘图模式
	void setDrawingMode(DrawingStyle::Mode mode);

	// 获取绘图样式
	DrawingStyle getDrawingStyle() const;

	// 设置绘图样式
	void setDrawingStyle(DrawingStyle style);

	// 获取透明度
	float getOpacity() const;

	// 设置透明度（范围：0 ~ 1）
	void setOpacity(float opacity);

	// 获取坐标
	Point getPos() const;

	// 设置坐标
	void setPos(
		const Point& point
	);

	// 移动画笔
	void movePos(
		const Vector2& point
	);

	// 获取旋转角度
	float getRotation() const;

	// 设置旋转角度
	void setRotation(
		float rotation
	);

	// 获取缩放比例
	Vector2 getScale() const;

	// 设置缩放比例
	void setScale(
		const Vector2& scale
	);

	// 获取节点倾斜角度
	Vector2 getSkew() const;

	// 设置倾斜角度
	void setSkew(
		const Vector2& skew
	);

	// 获取画笔变换矩阵
	Matrix32 getTransform() const;

	// 获取绘制图像时的像素插值方式
	InterpolationMode getInterpolationMode() const;

	// 设置绘制图像时的像素插值方式
	void setInterpolationMode(InterpolationMode mode);

	// 清空画布
	void clear();

	// 指定颜色清空画布
	void clear(const Color& color);

	virtual ~CanvasBrush();

protected:
	CanvasBrush(
		ID2D1RenderTarget* rt,
		ID2D1SolidColorBrush* brush
	);

	void _updateTransform();

protected:
	bool _dirtyTransform;
	ID2D1RenderTarget* _rt;
	ID2D1SolidColorBrush* _brush;
	ID2D1DrawingStateBlock* _state;
	InterpolationMode _interpolationMode;
	float _opacity;
	Point _pos;
	float _rotation;
	Vector2 _scale;
	Vector2 _skew;
	DrawingStyle _style;
};


// 画布
class Canvas
	: public Node
{
public:
	Canvas(const Size& size);

	virtual ~Canvas();

	// 画图
	void draw(const Function<void(CanvasBrush*)>& drawing);

	// 重新绘制上次内容
	void redraw();

	// 清空画布
	void clear();

	// 获取像素插值方式
	InterpolationMode getInterpolationMode() const;

	// 设置像素插值方式
	void setInterpolationMode(InterpolationMode mode);

	// 获取画布图像
	Image* getImage() const;

	virtual void onRender() override;

protected:
	// 画图
	virtual void draw(CanvasBrush* brush) = 0;

private:
	void _initialize();

	void _discardResources();

private:
	Image* _image;
	ID2D1RenderTarget* _rt;
	ID2D1SolidColorBrush* _brush;
	InterpolationMode _interpolationMode;
	Function<void(CanvasBrush*)> _drawing;
};

}