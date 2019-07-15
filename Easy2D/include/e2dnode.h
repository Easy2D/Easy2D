#pragma once
#include <e2dbase.h>
#include <e2dcollider.h>

namespace easy2d 
{


class Action;
class Transition;
class ColliderManager;

class Node :
	public Object
{
	friend class Scene;
	friend class Collider;
	friend class Transition;
	friend class ColliderManager;

public:
	// �ڵ�����
	struct Property
	{
		bool visable;		// �ɼ���
		double posX;		// X ����
		double posY;		// Y ����
		double width;		// ���
		double height;		// �߶�
		double opacity;		// ͸����
		double pivotX;		// ���ĵ� X ����
		double pivotY;		// ���ĵ� Y ����
		double scaleX;		// ��������
		double scaleY;		// ��������
		double rotation;	// ��ת�Ƕ�
		double skewAngleX;	// ������б�Ƕ�
		double skewAngleY;	// ������б�Ƕ�
	};

public:
	Node();

	virtual ~Node();

	// ���½ڵ�
	virtual void onUpdate() {}

	// ��Ⱦ�ڵ�
	virtual void onRender() {}

	// ��ȡ�ڵ���ʾ״̬
	virtual bool isVisiable() const;

	// �жϵ��Ƿ��ڽڵ���
	virtual bool containsPoint(
		const Point& point
	) const;

	// �ж��������Ƿ��ཻ
	virtual bool intersects(
		Node * node
	) const;

	// ��ȡ�ڵ�����
	virtual String getName() const;

	// ��ȡ�ڵ����Ƶ� Hash ֵ
	virtual size_t getHashName() const;

	// ��ȡ�ڵ��ͼ˳��
	virtual int getOrder() const;

	// ��ȡ�ڵ������
	virtual double getPosX() const;

	// ��ȡ�ڵ�������
	virtual double getPosY() const;

	// ��ȡ�ڵ�����
	virtual Point getPos() const;

	// ��ȡ�ڵ���
	virtual double getWidth() const;

	// ��ȡ�ڵ�߶�
	virtual double getHeight() const;

	// ��ȡ�ڵ��ȣ����������ţ�
	virtual double getRealWidth() const;

	// ��ȡ�ڵ�߶ȣ����������ţ�
	virtual double getRealHeight() const;

	// ��ȡ�ڵ��С�����������ţ�
	virtual Size getRealSize() const;

	// ��ȡ�ڵ�����ĵ�
	virtual double getPivotX() const;

	// ��ȡ�ڵ�����ĵ�
	virtual double getPivotY() const;

	// ��ȡ�ڵ��С
	virtual Size getSize() const;

	// ��ȡ�ڵ�������ű���
	virtual double getScaleX() const;

	// ��ȡ�ڵ��������ű���
	virtual double getScaleY() const;

	// ��ȡ�ڵ������б�Ƕ�
	virtual double getSkewX() const;

	// ��ȡ�ڵ�������б�Ƕ�
	virtual double getSkewY() const;

	// ��ȡ�ڵ���ת�Ƕ�
	virtual double getRotation() const;

	// ��ȡ�ڵ�͸����
	virtual double getOpacity() const;

	// ��ȡ�ڵ�����
	virtual Property getProperty() const;

	// ��ȡ�ڵ���ײ��
	virtual Collider * getCollider() const;

	// ��ȡ���ڵ�
	virtual Node * getParent() const;

	// ��ȡ�ڵ����ڳ���
	virtual Scene * getParentScene() const;

	// ��ȡ����������ͬ���ӽڵ�
	virtual std::vector<Node*> getChildren(
		const String& name
	) const;

	// ��ȡ������ͬ���ӽڵ�
	virtual Node* getChild(
		const String& name
	) const;

	// ��ȡ�����ӽڵ�
	virtual const std::vector<Node*>& getAllChildren() const;

	// ��ȡ�ӽڵ�����
	virtual int getChildrenCount() const;

	// �Ƴ��ӽڵ�
	virtual bool removeChild(
		Node * child
	);

	// �Ƴ�����������ͬ���ӽڵ�
	virtual void removeChildren(
		const String& childName
	);

	// �Ӹ��ڵ��Ƴ�
	virtual void removeFromParent();

	// �Ƴ����нڵ�
	virtual void clearAllChildren();

	// ���ýڵ��Ƿ���ʾ
	virtual void setVisiable(
		bool value
	);

	// ��������� onUpdate ����
	virtual void setAutoUpdate(
		bool bAutoUpdate
	);

	// ���ýڵ�����
	virtual void setName(
		const String& name
	);

	// ���ýڵ������
	virtual void setPosX(
		double x
	);

	// ���ýڵ�������
	virtual void setPosY(
		double y
	);

	// ���ýڵ�����
	virtual void setPos(
		const Point & point
	);

	// ���ýڵ�����
	virtual void setPos(
		double x,
		double y
	);

	// �ڵ�����̶�
	virtual void setPosFixed(
		bool fixed
	);

	// �ƶ��ڵ�
	virtual void movePosX(
		double x
	);

	// �ƶ��ڵ�
	virtual void movePosY(
		double y
	);

	// �ƶ��ڵ�
	virtual void movePos(
		double x,
		double y
	);

	// �ƶ��ڵ�
	virtual void movePos(
		const Vector & v
	);

	// ���ýڵ��ͼ˳��
	// Ĭ��Ϊ 0
	virtual void setOrder(
		int order
	);

	// ���ú������ű���
	// Ĭ��Ϊ 1.0f
	virtual void setScaleX(
		double scaleX
	);

	// �����������ű���
	// Ĭ��Ϊ 1.0f
	virtual void setScaleY(
		double scaleY
	);

	// �������ű���
	// Ĭ��Ϊ (1.0f, 1.0f)
	virtual void setScale(
		double scaleX,
		double scaleY
	);

	// �������ű���
	// Ĭ��Ϊ 1.0f
	virtual void setScale(
		double scale
	);

	// ���ú�����б�Ƕ�
	// Ĭ��Ϊ 0
	virtual void setSkewX(
		double angleX
	);

	// ����������б�Ƕ�
	// Ĭ��Ϊ 0
	virtual void setSkewY(
		double angleY
	);

	// ������б�Ƕ�
	// Ĭ��Ϊ (0, 0)
	virtual void setSkew(
		double angleX,
		double angleY
	);

	// ������ת�Ƕ�
	// Ĭ��Ϊ 0
	virtual void setRotation(
		double rotation
	);

	// ����͸����
	// Ĭ��Ϊ 1.0f, ��Χ [0, 1]
	virtual void setOpacity(
		double opacity
	);

	// �������ĵ�ĺ���λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotX(
		double pivotX
	);

	// �������ĵ������λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotY(
		double pivotY
	);

	// �������ĵ�λ��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	virtual void setPivot(
		double pivotX,
		double pivotY
	);

	// �޸Ľڵ���
	virtual void setWidth(
		double width
	);

	// �޸Ľڵ�߶�
	virtual void setHeight(
		double height
	);

	// �޸Ľڵ��С
	virtual void setSize(
		double width,
		double height
	);

	// �޸Ľڵ��С
	virtual void setSize(
		Size size
	);

	// ���ýڵ�����
	virtual void setProperty(
		Property prop
	);

	// ������ײ��
	virtual void setCollider(
		Collider::Type type
	);

	// ������ײ��
	virtual void setCollider(
		Collider * pCollider
	);

	// ����ӽڵ�
	virtual void addChild(
		Node * child,
		int order = 0	/* ��Ⱦ˳�� */
	);

	// ��Ӷ���ӽڵ�
	virtual void addChild(
		const std::vector<Node*>& nodes,	/* �ڵ����� */
		int order = 0						/* ��Ⱦ˳�� */
	);

	// ִ�ж���
	virtual void runAction(
		Action * action
	);

	// ��������
	virtual void resumeAction(
		const String& name
	);

	// ��ͣ����
	virtual void pauseAction(
		const String& name
	);

	// ֹͣ����
	virtual void stopAction(
		const String& name
	);

	// ����������ͣ����
	virtual void resumeAllActions();

	// ��ͣ���ж���
	virtual void pauseAllActions();

	// ֹͣ���ж���
	virtual void stopAllActions();

	// �޸Ľڵ��Ĭ�����ĵ�λ��
	static void setDefaultPiovt(
		double defaultPiovtX,
		double defaultPiovtY
	);

	// ���ýڵ��Ĭ����ײ�����ͣ�Ĭ���ޣ�
	static void setDefaultCollider(
		Collider::Type type
	);

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ���½ڵ�
	void _update();

	// ��Ⱦ�ڵ�
	void _render();

	// ��Ⱦͼ��
	void _drawCollider();

	// ���ýڵ����ڳ���
	void _setParentScene(
		Scene * scene
	);

	// ���½ڵ��ά����
	void _updateTransform();

	// �ӽڵ�����
	void _sortChildren();

	// ���½ڵ�͸����
	void _updateOpacity();

	// �ṩ������ĸ��º���
	virtual void _fixedUpdate() {}

protected:
	String		_name;
	size_t		_hashName;
	float		_posX;
	float		_posY;
	float		_width;
	float		_height;
	float		_scaleX;
	float		_scaleY;
	float		_rotation;
	float		_skewAngleX;
	float		_skewAngleY;
	float		_displayOpacity;
	float		_realOpacity;
	float		_pivotX;
	float		_pivotY;
	int			_nOrder;
	bool		_visiable;
	bool		_autoUpdate;
	bool		_needSort;
	bool		_needTransform;
	bool		_positionFixed;
	Collider *	_collider;
	Scene *		_parentScene;
	Node *		_parent;
	D2D1::Matrix3x2F	_initialMatri;
	D2D1::Matrix3x2F	_finalMatri;
	std::vector<Node*>	_children;
};


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
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	explicit Sprite(
		const String& filePath,	/* ͼƬ�ļ�·�� */
		const Rect& cropRect	/* �ü����� */
	);

	explicit Sprite(
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
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);
	
	// ����ͼƬ
	bool open(
		Image * image
	);

	// ��ͼƬ�ü�Ϊ����
	void crop(
		const Rect& cropRect	/* �ü����� */
	);

	// ��ȡ Image ����
	virtual Image * getImage() const;

	// ��Ⱦ����
	virtual void onRender() override;

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	Image * _image;
};


// �ı�
class Text :
	public Node
{
public:
	// �ı����뷽ʽ
	enum class Align
	{
		Left,		/* ����� */
		Right,		/* �Ҷ��� */
		Center		/* ���ж��� */
	};

	// �ı���ʽ
	class Style
	{
	public:
		Color		color;				// ��ɫ
		Align		alignment;			// ���뷽ʽ
		bool		wrapping;			// ���Զ�����
		double		wrappingWidth;		// �Զ����п��
		double		lineSpacing;		// �м��
		bool		hasUnderline;		// �»���
		bool		hasStrikethrough;	// ɾ����
		bool		hasOutline;			// ��ʾ���
		Color		outlineColor;		// �����ɫ
		double		outlineWidth;		// ����߿�
		LineJoin	outlineJoin;		// ������ཻ��ʽ

	public:
		Style();

		Style(
			Color color,
			Align alignment = Align::Left,
			bool wrapping = false,
			double wrappingWidth = 0.0,
			double lineSpacing = 0.0,
			bool hasUnderline = false,
			bool hasStrikethrough = false,
			bool hasOutline = true,
			Color outlineColor = Color(Color::Black, 0.5),
			double outlineWidth = 1.0,
			LineJoin outlineJoin = LineJoin::Round
		);
	};

public:
	Text();

	explicit Text(
		const String& text,						/* �������� */
		const Font& font = Font(),				/* ���� */
		const Style& style = Style()			/* �ı���ʽ */
	);

	virtual ~Text();

	// ��ȡ�ı�
	String getText() const;

	// ��ȡ����
	Font getFont() const;

	// ��ȡ�ı���ʽ
	Style getStyle() const;

	// ��ȡ������
	String getFontFamily() const;

	// ��ȡ��ǰ�ֺ�
	double getFontSize() const;

	// ��ȡ��ǰ�����ϸֵ
	UINT getFontWeight() const;

	// ��ȡ������ɫ
	Color getColor() const;

	// ��ȡ�����ɫ
	Color getOutlineColor() const;

	// ��ȡ����߿�
	double getOutlineWidth() const;

	// ��ȡ������ཻ��ʽ
	LineJoin getOutlineJoin() const;

	// ��ȡ�ı���ʾ����
	int getLineCount() const;

	// �Ƿ���б��
	bool isItalic() const;

	// �Ƿ���ʾɾ����
	bool hasStrikethrough() const;

	// �Ƿ���ʾ�»���
	bool hasUnderline() const;

	// �Ƿ���ʾ���
	bool hasOutline() const;

	// �����ı�
	void setText(
		const String& text
	);

	// �����ı���ʽ
	void setStyle(
		const Style& style
	);

	// ��������
	void setFont(
		const Font& font
	);

	// ����������
	void setFontFamily(
		const String& family
	);

	// �����ֺţ�Ĭ��ֵΪ 22��
	void setFontSize(
		double size
	);

	// ���������ϸֵ��Ĭ��ֵΪ Text::Font::Weight::Normal��
	void setFontWeight(
		UINT weight
	);

	// ����������ɫ��Ĭ��ֵΪ Color::WHITE��
	void setColor(
		Color color
	);

	// ��������б�壨Ĭ��ֵΪ false��
	void setItalic(
		bool value
	);

	// �򿪻�ر��ı��Զ����У�Ĭ��Ϊ�رգ�
	void setWrapping(
		bool wrapping
	);

	// �����ı��Զ����еĿ�ȣ�Ĭ��Ϊ 0��
	void setWrappingWidth(
		double wrappingWidth
	);

	// �����м�ࣨĬ��Ϊ 0��
	void setLineSpacing(
		double lineSpacing
	);

	// ���ö��뷽ʽ��Ĭ��Ϊ Align::Left��
	void setAlignment(
		Align align
	);

	// �����»��ߣ�Ĭ��ֵΪ false��
	void setUnderline(
		bool hasUnderline
	);

	// ����ɾ���ߣ�Ĭ��ֵΪ false��
	void setStrikethrough(
		bool hasStrikethrough
	);

	// �����Ƿ���ʾ���
	void setOutline(
		bool hasOutline
	);

	// ���������ɫ
	void setOutlineColor(
		Color outlineColor
	);

	// ��������߿�
	void setOutlineWidth(
		double outlineWidth
	);

	// ����������ཻ��ʽ
	void setOutlineJoin(
		LineJoin outlineJoin
	);

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	// �����Ű�����
	void _reset();

	// �������ָ�ʽ��
	void _createFormat();

	// �������ֲ���
	void _createLayout();

protected:
	String	_text;
	Font	_font;
	Style	_style;
	IDWriteTextFormat * _textFormat;
	IDWriteTextLayout * _textLayout;
};


class Button :
	public Node
{
public:
	Button();

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * selected,				/* ��갴��״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * mouseover,				/* �������״̬ */
		Node * selected,				/* ��갴��״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * mouseover,				/* �������״̬ */
		Node * selected,				/* �������״̬ */
		Node * disabled,				/* ��ť����״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	// ��ȡ��ť״̬�����û��ǽ���
	bool isEnable() const;

	// ���ð�ť���û����
	void setEnable(
		bool enable
	);

	// ����һ���������ʾ�İ�ť
	virtual void setNormal(
		Node * normal
	);

	// ����������밴ťʱ��ʾ�İ�ť
	virtual void setMouseOver(
		Node * mouseover
	);

	// ������갴�°�ťʱ��ʾ�İ�ť
	virtual void setSelected(
		Node * selected
	);

	// ���ð�ť������ʱ��ʾ�İ�ť
	virtual void setDisabled(
		Node * disabled
	);

	// ���ð�ť������ִ�к���
	void setClickFunc(
		const Function<void()>& func
	);

protected:
	// ��ť״̬ö��
	enum class ButtonState { Normal, Mouseover, Selected };

	// ���ð�ť״̬
	virtual void _setState(ButtonState state);

	// ˢ�°�ť��ʾ
	virtual void _updateVisiable();

	// ���°�ť״̬
	virtual void _fixedUpdate() override;

	// ִ�а�ť��������
	virtual void _runCallback();

protected:
	Node *		_normal;
	Node *		_mouseover;
	Node *		_selected;
	Node *		_disabled;
	bool		_enable;
	bool		_isSelected;
	ButtonState	_state;
	Function<void()>	_func;
};


class ToggleButton :
	public Button
{
public:
	ToggleButton();

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onMouseOver,				/* ��ť��ʱ���������״̬ */
		Node * offMouseOver,			/* ��ť�ر�ʱ���������״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onMouseOver,				/* ��ť��ʱ���������״̬ */
		Node * offMouseOver,			/* ��ť�ر�ʱ���������״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		Node * onDisabled,				/* ��ť��ʱ������״̬ */
		Node * offDisabled,				/* ��ť�ر�ʱ������״̬ */
		const Function<void()>& func = nullptr	/* ��ť������ִ�к��� */
	);

	// ��ȡ����״̬���򿪻�رգ�
	bool getState() const;

	// ���ÿ��ذ�ť��״̬���򿪻�رգ�
	void setState(
		bool bState
	);

	// ���ð�ť��״̬����ʾ�İ�ť
	virtual void setNormal(
		Node * normal
	) override;

	// ���ð�ť��״̬�£�������밴ťʱ��ʾ�İ�ť
	virtual void setMouseOver(
		Node * mouseover
	) override;

	// ���ð�ť��״̬�£���갴�°�ťʱ��ʾ�İ�ť
	virtual void setSelected(
		Node * selected
	) override;

	// ���ð�ť��״̬�£�������ʱ��ʾ�İ�ť
	virtual void setDisabled(
		Node * disabled
	) override;

	// ���ð�ť�ر�״̬����ʾ�İ�ť
	void setNormalOff(
		Node * normal
	);

	// ���ð�ť�ر�״̬�£�������밴ťʱ��ʾ�İ�ť
	void setMouseOverOff(
		Node * mouseover
	);

	// ���ð�ť�ر�״̬�£���갴�°�ťʱ��ʾ�İ�ť
	void setSelectedOff(
		Node * selected
	);

	// ���ð�ť�ر�״̬�£���ť������ʱ��ʾ�İ�ť
	void setDisabledOff(
		Node * disabled
	);

protected:
	// ˢ�°�ť����
	virtual void _updateState();

	// ִ�а�ť��������
	virtual void _runCallback() override;

protected:
	Node *	_normalOff;
	Node *	_mouseoverOff;
	Node *	_selectedOff;
	Node *	_disabledOff;
	bool	_toggle;
};


class Menu :
	public Node
{
public:
	Menu();

	explicit Menu(
		const std::vector<Button*>& buttons	/* ��ť���� */
	);

	// ��ȡ�˵��Ƿ����
	bool isEnable() const;

	// ��ȡ�˵��еİ�ť����
	size_t getButtonCount() const;

	// ���ò˵����û����
	void setEnable(
		bool enable
	);

	// ��Ӱ�ť
	void addButton(
		Button * button
	);

	// �Ƴ���ť
	bool removeButton(
		Button * button
	);

	// ��ȡ���а�ť
	const std::vector<Button*>& getAllButtons() const;

protected:
	bool _enable;
	std::vector<Button*> _buttons;
};

}