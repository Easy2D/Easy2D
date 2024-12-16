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

// ������״��ϵ
enum class BodyRelation
{
	Disjoint,		// ��ȫ���ཻ
	IsContained,	// ���Է�����
	Contains,		// �����Է�
	Overlap,		// �ཻ��������
};

class Node :
	public Object
{
	friend class Transition;
	friend class SceneManager;
	friend class Renderer;

public:
	// �ڵ�����
	struct Property
	{
		bool visable;		// �ɼ���
		Point pos;			// ����
		Size size;			// ���
		float opacity;		// ͸����
		Point anchor;		// ê��
		Vector2 scale;		// ����
		float rotation;		// ��ת�Ƕ�
		Vector2 skewAngle;	// ��б�Ƕ�
	};

public:
	Node();

	virtual ~Node();

	// ���½ڵ�
	virtual void onUpdate() {}

	// ��Ⱦ�ڵ�
	virtual void onRender() {}

	// ��ȡ�ڵ���ʾ״̬
	bool isVisible() const;

	// ��ȡ�ڵ��ͼ˳��
	int getOrder() const;

	// ��ȡ�ڵ������
	float getPosX() const;

	// ��ȡ�ڵ�������
	float getPosY() const;

	// ��ȡ�ڵ�����
	Point getPos() const;

	// ��ȡ�ڵ���
	float getWidth() const;

	// ��ȡ�ڵ�߶�
	float getHeight() const;

	// ��ȡ�ڵ��ȣ����������ţ�
	float getRealWidth() const;

	// ��ȡ�ڵ�߶ȣ����������ţ�
	float getRealHeight() const;

	// ��ȡ�ڵ��С�����������ţ�
	Size getRealSize() const;

	// ��ȡ�ڵ��ê��
	float getAnchorX() const;

	// ��ȡ�ڵ��ê��
	float getAnchorY() const;

	// ��ȡ�ڵ��С
	Size getSize() const;

	// ��ȡ�ڵ�������ű���
	float getScaleX() const;

	// ��ȡ�ڵ��������ű���
	float getScaleY() const;

	// ��ȡ�ڵ������б�Ƕ�
	float getSkewX() const;

	// ��ȡ�ڵ�������б�Ƕ�
	float getSkewY() const;

	// ��ȡ�ڵ���ת�Ƕ�
	float getRotation() const;

	// ��ȡ�ڵ�͸����
	float getOpacity() const;

	// ��ȡ�ڵ�����
	Property getProperty() const;

	// ��ȡ�߿�
	virtual Rect getBounds() const;

	// ��ȡ���а�Χ��
	virtual Rect getBoundingBox() const;

	// ��ȡ��ά�任����
	Matrix32 getTransform() const;

	// ��ȡ��ά�任�����
	Matrix32 getInverseTransform() const;

	// ��ȡ���ڵ�
	Node * getParent() const;

	// ��ȡ�ڵ����ڳ���
	Scene * getParentScene() const;

	// �Ƿ����������
	virtual bool containsPoint(Point const& point) const;

	// ���ýڵ��Ƿ���ʾ
	void setVisible(
		bool value
	);

	// ��������� onUpdate ����
	void setAutoUpdate(
		bool autoUpdate
	);

	// ���ýڵ������
	void setPosX(
		float x
	);

	// ���ýڵ�������
	void setPosY(
		float y
	);

	// ���ýڵ�����
	void setPos(
		const Point & point
	);

	// ���ýڵ�����
	void setPos(
		float x,
		float y
	);

	// �ƶ��ڵ�
	void movePosX(
		float x
	);

	// �ƶ��ڵ�
	void movePosY(
		float y
	);

	// �ƶ��ڵ�
	void movePos(
		float x,
		float y
	);

	// �ƶ��ڵ�
	void movePos(
		const Vector2 & v
	);

	// ���ýڵ��ͼ˳�����ִ�Ļ���ʾ�������ֵܽڵ��Ϸ�
	// Ĭ��Ϊ 0
	void setOrder(
		int order
	);

	// ���ú������ű���
	// Ĭ��Ϊ 1.0f
	void setScaleX(
		float scaleX
	);

	// �����������ű���
	// Ĭ��Ϊ 1.0f
	void setScaleY(
		float scaleY
	);

	// �������ű���
	// Ĭ��Ϊ (1.0f, 1.0f)
	void setScale(
		float scaleX,
		float scaleY
	);

	// �������ű���
	// Ĭ��Ϊ 1.0f
	void setScale(
		float scale
	);

	// ���ú�����б�Ƕ�
	// Ĭ��Ϊ 0
	void setSkewX(
		float angleX
	);

	// ����������б�Ƕ�
	// Ĭ��Ϊ 0
	void setSkewY(
		float angleY
	);

	// ������б�Ƕ�
	// Ĭ��Ϊ (0, 0)
	void setSkew(
		float angleX,
		float angleY
	);

	// ������ת�Ƕ�
	// Ĭ��Ϊ 0
	void setRotation(
		float rotation
	);

	// ����͸����
	// Ĭ��Ϊ 1.0f, ��Χ [0, 1]
	void setOpacity(
		float opacity
	);

	// ����ê��ĺ���λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	void setAnchorX(
		float anchorX
	);

	// ����ê�������λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	void setAnchorY(
		float anchorY
	);

	// ����ê��λ��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	void setAnchor(
		float anchorX,
		float anchorY
	);

	// �޸Ľڵ���
	void setWidth(
		float width
	);

	// �޸Ľڵ�߶�
	void setHeight(
		float height
	);

	// �޸Ľڵ��С
	void setSize(
		float width,
		float height
	);

	// �޸Ľڵ��С
	void setSize(
		Size size
	);

	// ���ýڵ�����
	void setProperty(
		Property prop
	);

	// ����ӽڵ�
	void addChild(
		Node * child
	);

	// ����ӽڵ�
	void addChild(
		Node* child,
		int order		/* ��Ⱦ˳�����ִ�Ļ���ʾ�������ֵܽڵ��Ϸ� */
	);

	// ��Ӷ���ӽڵ�
	void addChildren(
		std::initializer_list<Node*> nodes	/* �ڵ����� */
	);

	// ��Ӷ���ӽڵ�
	void addChildren(
		std::initializer_list<Node*> nodes,	/* �ڵ����� */
		int order							/* ��Ⱦ˳�����ִ�Ļ���ʾ�������ֵܽڵ��Ϸ� */
	);

	// ��ȡ����������ͬ���ӽڵ�
	std::vector<Node*> getChildren(
		const String& name
	) const;

	// ��ȡ������ͬ���ӽڵ�
	Node* getChild(
		const String& name
	) const;

	// ��ȡ�����ӽڵ�
	const std::vector<Node*>& getAllChildren() const;

	// ��ȡ�ӽڵ�����
	int getChildrenCount() const;

	// �Ƴ��ӽڵ�
	bool removeChild(
		Node* child
	);

	// �Ƴ�����������ͬ���ӽڵ�
	void removeChildren(
		const String& childName
	);

	// �Ƴ����нڵ�
	void removeAllChildren();

	// �Ӹ��ڵ��Ƴ���������һ�θ���ʱִ�У�
	void removeSelfInNextUpdate();

	// ִ�ж���
	void runAction(
		Action * action
	);

	// ��������
	void resumeAction(
		const String& name
	);

	// ��ͣ����
	void pauseAction(
		const String& name
	);

	// ֹͣ����
	void stopAction(
		const String& name
	);

	// ����������ͣ����
	void resumeAllActions();

	// ��ͣ���ж���
	void pauseAllActions();

	// ֹͣ���ж���
	void stopAllActions();

	// ����������
	Listener* addListener(
		const Listener::Callback& func,	/* �������û�����ʱ��ִ�к��� */
		const String& name = {},		/* ���������� */
		bool paused = false				/* �Ƿ���ͣ */
	);

	// �����ײ����
	void addListener(
		ListenerBase* listener		/* ������ */
	);

	// �Ƴ�������
	void removeListener(
		ListenerBase* listener		/* ������ */
	);

	// �����������
	void startListener(
		const String& name
	);

	// ֹͣ�������
	void stopListener(
		const String& name
	);

	// �Ƴ��������
	void removeListener(
		const String& name
	);

	// �������м�����
	void startAllListeners();

	// ֹͣ���м�����
	void stopAllListeners();

	// �Ƴ����м�����
	void removeAllListeners();

	// �޸Ľڵ��Ĭ��ê��λ��
	static void setDefaultAnchor(
		float defaultAnchorX,
		float defaultAnchorY
	);

	// ��ȡ������״
	Shape* getBodyShape() const;

	// ����������״
	void setBodyShape(Shape* shape);

	// ��Ⱦ������״
	void showBodyShape(bool enabled = true);

	// �жϺ���һ�ڵ��������״��ϵ
	BodyRelation compareWithBody(Node* other) const;

protected:
	// ���½ڵ�
	void _update();

	// ��Ⱦ�ڵ�
	void _render();

	void _renderBodyShape();

	// ���ýڵ����ڳ���
	void _setParentScene(
		Scene * scene
	);

	// ���¶�ά�任����
	void _updateTransform() const;

	// ���¶�ά�任�����
	void _updateInverseTransform() const;

	// �ӽڵ�����
	void _sortChildren();

	// ���½ڵ�͸����
	void _updateOpacity();

private:
	// ���¼�����
	void __updateListeners();

	void __dispatchEvent(Event* evt);

	// ��ռ�����
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


// ����
class Scene :
	public Node
{
public:
	Scene();

	virtual ~Scene();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter() {}

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit() {}

	// ��д��������������ڹرմ���ʱִ�У����� false ����ֹ���ڹرգ�
	virtual bool onCloseWindow() { return true; }
};


// ����
class Sprite :
	public Node
{
public:
	Sprite();

	explicit Sprite(
		Image * image
	);

	explicit Sprite(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	explicit Sprite(
		const Resource& res		/* ͼƬ��Դ */
	);

	explicit Sprite(
		KeyFrame* frame			/* �ؼ�֡ */
	);

	Sprite(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	Sprite(
		const String& filePath,	/* ͼƬ�ļ�·�� */
		const Rect& cropRect	/* �ü����� */
	);

	Sprite(
		const Resource& res,	/* ͼƬ��Դ */
		const Rect& cropRect	/* �ü����� */
	);

	Sprite(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType,	/* ͼƬ��Դ���� */
		const Rect& cropRect	/* �ü����� */
	);

	virtual ~Sprite();

	// ����ͼƬ�ļ�
	bool open(
		const String& filePath
	);

	// ����ͼƬ��Դ
	bool open(
		const Resource& res		/* ͼƬ��Դ */
	);

	// ����ͼƬ��Դ
	bool open(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	// ��ȡ�ü�����
	Rect getCropRect() const;

	// ��ͼƬ�ü�Ϊ����
	void crop(
		const Rect& cropRect	/* �ü����Σ����վ����򲻲ü� */
	);

	// ��ȡͼƬ
	Image* getImage() const;

	// ����ͼƬ
	void setImage(
		Image* image,
		bool resetCropRect = true	/* ���òü����� */
	);

	// ���ùؼ�֡
	void setKeyFrame(
		KeyFrame* frame
	);

	// ��ȡ���ز�ֵ��ʽ
	InterpolationMode getInterpolationMode() const;

	// �������ز�ֵ��ʽ
	void setInterpolationMode(InterpolationMode mode);

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	Image*	_image;
	Rect	_cropRect;
	InterpolationMode _interpolationMode;
};


// �ı��ڵ�
class Text
	: public Node
{
public:
	Text();

	explicit Text(
		const String& text,									/* �������� */
		const TextStyle& textStyle = TextStyle(),			/* �ı���ʽ */
		const DrawingStyle& drawingStyle = DrawingStyle()	/* ��ͼ��ʽ */
	);

	explicit Text(
		TextLayout* layout,									/* ���ֲ��� */
		const DrawingStyle& drawingStyle = DrawingStyle()	/* ��ͼ��ʽ */
	);

	virtual ~Text();

	// ��ȡ�ı�����
	TextLayout* getLayout() const;

	// ��ȡ�ı�
	String getText() const;

	// ��ȡ����
	Font getFont() const;

	// ��ȡ�ı���ʽ
	TextStyle getTextStyle() const;

	// ��ȡ��Ⱦ��ʽ
	DrawingStyle getDrawingStyle() const;

	// ��ȡ�ı���ʾ����
	int getLineCount() const;

	// �����ı�����
	void setTextLayout(
		TextLayout* layout
	);

	// �����ı�
	void setText(
		const String& text
	);

	// �����ı���ʽ
	void setTextStyle(
		const TextStyle& style
	);

	// ������Ⱦ��ʽ
	void setDrawingStyle(
		DrawingStyle style
	);

	// ��������
	void setFont(
		const Font& font
	);

	// ����������
	void setFontFamily(
		const String& family
	);

	// �����ֺ�
	void setFontSize(
		float size
	);

	// ���������ϸֵ
	void setFontWeight(
		UINT weight
	);

	// ��������б��
	void setItalic(
		bool italic
	);

	// �򿪻�ر��ı��Զ�����
	void setWrapping(
		bool wrapping
	);

	// �����ı��Զ����еĿ��
	void setWrappingWidth(
		float wrappingWidth
	);

	// �����м�ࣨĬ��Ϊ 0��
	void setLineSpacing(
		float lineSpacing
	);

	// ���ö��뷽ʽ
	void setAlignment(
		TextAlign align
	);

	// �����»���
	void setUnderline(
		bool hasUnderline
	);

	// ����ɾ����
	void setStrikethrough(
		bool hasStrikethrough
	);

	// �������������ɫ
	void setFillColor(
		Color color
	);

	// ���������ɫ
	void setStrokeColor(
		Color strokeColor
	);

	// ��������߿�
	void setStrokeWidth(
		float strokeWidth
	);

	// ����������ཻ��ʽ
	void setLineJoin(
		LineJoin lineJoin
	);

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	void updateLayout();

protected:
	TextLayout*	_layout;
	DrawingStyle _style;
};


// ��״�ڵ�
class ShapeNode :
	public Node
{
public:
	// ����ֱ�߽ڵ�
	ShapeNode(
		Shape::LineType,
		Point begin,
		Point end,
		DrawingStyle style = DrawingStyle{}
	);

	// �������νڵ�
	ShapeNode(
		Shape::RectType,
		const Size& size,		// ���ο��
		DrawingStyle style = DrawingStyle{}
	);

	// ����Բ�Ǿ��νڵ�
	ShapeNode(
		Shape::RoundedRectType,
		const Size& size,		// ���ο��
		const Vector2& radius,	// ����Բ�ǰ뾶
		DrawingStyle style = DrawingStyle{}
	);

	// ����Բ�νڵ�
	ShapeNode(
		Shape::CircleType,
		float radius,			// �뾶
		DrawingStyle style = DrawingStyle{}
	);

	// ������Բ�νڵ�
	ShapeNode(
		Shape::EllipseType,
		const Vector2& radius,	// �뾶
		DrawingStyle style = DrawingStyle{}
	);

	// ��������νڵ�
	ShapeNode(
		Shape::PolygonType,
		std::initializer_list<Point> vertices,	// ����ζ���
		DrawingStyle style = DrawingStyle{}
	);

	// ��������νڵ�
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

	// ��ȡ��״
	Shape* getShape() const;

	// ������״
	void setShape(Shape* shape);

	// ��ȡ�����ɫ
	Color getFillColor() const;

	// ���������ɫ
	void setFillColor(
		Color fillColor
	);

	// ��ȡ������ɫ
	Color getStrokeColor() const;

	// ����������ɫ
	void setStrokeColor(
		Color strokeColor
	);

	// ��ȡ�������
	float getStrokeWidth() const;

	// �����������
	void setStrokeWidth(
		float strokeWidth
	);

	// ���������ཻ��ʽ
	void setLineJoin(
		LineJoin lineJoin
	);

	// ��ȡ��ͼģʽ
	DrawingStyle::Mode getDrawingMode() const;

	// ���û�ͼģʽ
	void setDrawingMode(DrawingStyle::Mode mode);

	// ��ȡ��ͼ��ʽ
	DrawingStyle getDrawingStyle() const;

	// ���û�ͼ��ʽ
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

// ��������
class CanvasBrush
	: public Object
{
	friend Canvas;

public:
	// ������״
	void drawShape(
		Shape* shape
	);

	// ����ͼƬ
	void drawImage(
		Image* image,					/* ͼ�� */
		const Point& pos = Point(),		/* ����λ�� */
		const Rect& cropRect = Rect{}	/* �ü����� */
	);

	// ����ͼƬ
	void drawImage(
		Image* image,					/* ͼ�� */
		const Rect& destRect,			/* �������� */
		const Rect& cropRect = Rect{}	/* �ü����� */
	);

	// ���ƹؼ�֡
	void drawImage(
		KeyFrame* frame,				/* �ؼ�֡ */
		const Point& pos = Point()		/* ����λ�� */
	);

	// ���ƹؼ�֡
	void drawImage(
		KeyFrame* frame,				/* �ؼ�֡ */
		const Rect& destRect			/* �������� */
	);

	// �����ı�
	void drawText(
		TextLayout* layout,
		const Point& pos = Point()
	);

	// �����ı�
	void drawText(
		const String& text,
		const Point& pos = Point(),
		const TextStyle& style = TextStyle()
	);

	// ��ȡ�����ɫ
	Color getFillColor() const;

	// ���������ɫ
	void setFillColor(
		Color fillColor
	);

	// ��ȡ������ɫ
	Color getStrokeColor() const;

	// ����������ɫ
	void setStrokeColor(
		Color strokeColor
	);

	// ��ȡ�������
	float getStrokeWidth() const;

	// �����������
	void setStrokeWidth(
		float strokeWidth
	);

	// ���������ཻ��ʽ
	void setLineJoin(
		LineJoin lineJoin
	);

	// ��ȡ��ͼģʽ
	DrawingStyle::Mode getDrawingMode() const;

	// ���û�ͼģʽ
	void setDrawingMode(DrawingStyle::Mode mode);

	// ��ȡ��ͼ��ʽ
	DrawingStyle getDrawingStyle() const;

	// ���û�ͼ��ʽ
	void setDrawingStyle(DrawingStyle style);

	// ��ȡ͸����
	float getOpacity() const;

	// ����͸���ȣ���Χ��0 ~ 1��
	void setOpacity(float opacity);

	// ��ȡ����
	Point getPos() const;

	// ��������
	void setPos(
		const Point& point
	);

	// �ƶ�����
	void movePos(
		const Vector2& point
	);

	// ��ȡ��ת�Ƕ�
	float getRotation() const;

	// ������ת�Ƕ�
	void setRotation(
		float rotation
	);

	// ��ȡ���ű���
	Vector2 getScale() const;

	// �������ű���
	void setScale(
		const Vector2& scale
	);

	// ��ȡ�ڵ���б�Ƕ�
	Vector2 getSkew() const;

	// ������б�Ƕ�
	void setSkew(
		const Vector2& skew
	);

	// ��ȡ���ʱ任����
	Matrix32 getTransform() const;

	// ��ȡ����ͼ��ʱ�����ز�ֵ��ʽ
	InterpolationMode getInterpolationMode() const;

	// ���û���ͼ��ʱ�����ز�ֵ��ʽ
	void setInterpolationMode(InterpolationMode mode);

	// ��ջ���
	void clear();

	// ָ����ɫ��ջ���
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


// ����
class Canvas
	: public Node
{
public:
	Canvas(const Size& size);

	virtual ~Canvas();

	// ��ͼ
	void draw(const Function<void(CanvasBrush*)>& drawing);

	// ���»����ϴ�����
	void redraw();

	// ��ջ���
	void clear();

	// ��ȡ���ز�ֵ��ʽ
	InterpolationMode getInterpolationMode() const;

	// �������ز�ֵ��ʽ
	void setInterpolationMode(InterpolationMode mode);

	// ��ȡ����ͼ��
	Image* getImage() const;

	virtual void onRender() override;

protected:
	// ��ͼ
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