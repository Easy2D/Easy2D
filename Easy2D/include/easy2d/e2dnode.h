#pragma once
#include <easy2d/e2dbase.h>

namespace easy2d 
{


class Action;
class Transition;
class SceneManager;

class Node :
	public Object
{
	friend class Scene;
	friend class Transition;
	friend class SceneManager;

public:
	// �ڵ�����
	struct Property
	{
		bool visable;		// �ɼ���
		float posX;			// X ����
		float posY;			// Y ����
		float width;		// ���
		float height;		// �߶�
		float opacity;		// ͸����
		float anchorX;		// ê�� X ����
		float anchorY;		// ê�� Y ����
		float scaleX;		// ��������
		float scaleY;		// ��������
		float rotation;		// ��ת�Ƕ�
		float skewAngleX;	// ������б�Ƕ�
		float skewAngleY;	// ������б�Ƕ�
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

	// ��ȡ�ڵ�����
	String getName() const;

	// ��ȡ�ڵ����Ƶ� Hash ֵ
	size_t getHashName() const;

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
	Rect getBounds() const;

	// ��ȡ���а�Χ��
	Rect getBoundingBox() const;

	// ��ȡ��ά�任����
	Matrix32 getTransform() const;

	// ��ȡ��ά�任�����
	Matrix32 getInverseTransform() const;

	// ��ȡ���ڵ�
	Node * getParent() const;

	// ��ȡ�ڵ����ڳ���
	Scene * getParentScene() const;

	// �Ƿ����������
	bool containsPoint(Point const& point);

	// ���ýڵ��Ƿ���ʾ
	void setVisible(
		bool value
	);

	// ��������� onUpdate ����
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// ���ýڵ�����
	void setName(
		const String& name
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

	// �ڵ�����̶�
	void setPosFixed(
		bool fixed
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

	// ���ýڵ��ͼ˳��
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
		Node * child,
		int order = 0	/* ��Ⱦ˳�� */
	);

	// ��Ӷ���ӽڵ�
	void addChild(
		const std::vector<Node*>& nodes,	/* �ڵ����� */
		int order = 0						/* ��Ⱦ˳�� */
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

	// �Ӹ��ڵ��Ƴ�
	void removeFromParent();

	// �Ƴ����нڵ�
	void removeAllChildren();

	// �ַ��¼�
	void dispatch(Event* evt);

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
		const String& name = L"",		/* ���������� */
		bool paused = false				/* �Ƿ���ͣ */
	);

	// �����ײ����
	void addListener(
		Listener* listener		/* ������ */
	);

	// �Ƴ�������
	void removeListener(
		Listener* listener		/* ������ */
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

protected:
	// ���½ڵ�
	void _update();

	// ��Ⱦ�ڵ�
	void _render();

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

	// ���¼�����
	void __updateListeners(Event* evt);

	// ��ռ�����
	void __clearListeners();

protected:
	bool		_visible;
	bool		_autoUpdate;
	bool		_needSort;
	bool		_positionFixed;
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
	float		_anchorX;
	float		_anchorY;
	int			_nOrder;
	String		_name;
	size_t		_hashName;
	Scene *		_parentScene;
	Node *		_parent;
	
	std::vector<Node*>	_children;
	std::vector<Listener*> _listeners;

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
		float		wrappingWidth;		// �Զ����п��
		float		lineSpacing;		// �м��
		bool		hasUnderline;		// �»���
		bool		hasStrikethrough;	// ɾ����
		bool		hasOutline;			// ��ʾ���
		Color		outlineColor;		// �����ɫ
		float		outlineWidth;		// ����߿�
		LineJoin	outlineJoin;		// ������ཻ��ʽ

	public:
		Style();

		Style(
			Color color,
			Align alignment = Align::Left,
			bool wrapping = false,
			float wrappingWidth = 0.0,
			float lineSpacing = 0.0,
			bool hasUnderline = false,
			bool hasStrikethrough = false,
			bool hasOutline = true,
			Color outlineColor = Color(Color::Black, 0.5),
			float outlineWidth = 1.0,
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
	float getFontSize() const;

	// ��ȡ��ǰ�����ϸֵ
	UINT getFontWeight() const;

	// ��ȡ������ɫ
	Color getColor() const;

	// ��ȡ�����ɫ
	Color getOutlineColor() const;

	// ��ȡ����߿�
	float getOutlineWidth() const;

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
		float size
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
		float wrappingWidth
	);

	// �����м�ࣨĬ��Ϊ 0��
	void setLineSpacing(
		float lineSpacing
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
		float outlineWidth
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


// ��ť
class Button :
	public Node
{
public:
	using Callback = Function<void()>;

	Button();

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * selected,				/* ��갴��״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * mouseover,				/* �������״̬ */
		Node * selected,				/* ��갴��״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * mouseover,				/* �������״̬ */
		Node * selected,				/* �������״̬ */
		Node * disabled,				/* ��ť����״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
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
		const Callback& func
	);

protected:
	// ��ť״̬ö��
	enum class ButtonState { Normal, Mouseover, Selected };

	// ���ð�ť״̬
	virtual void _setState(ButtonState state);

	// ˢ�°�ť��ʾ
	virtual void _updateVisiable();

	// ִ�а�ť��������
	virtual void _runCallback();

	void updateStatus(Event* evt);

protected:
	bool		_enable;
	bool		_isSelected;
	bool		_isHover;
	bool		_isPressed;
	Node *		_normal;
	Node *		_mouseover;
	Node *		_selected;
	Node *		_disabled;
	ButtonState	_state;
	Callback	_func;
};


// ���ذ�ť
class ToggleButton :
	public Button
{
public:
	ToggleButton();

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
	);

	explicit ToggleButton(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onMouseOver,				/* ��ť��ʱ���������״̬ */
		Node * offMouseOver,			/* ��ť�ر�ʱ���������״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
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
		const Callback& func = nullptr	/* ��ť������ִ�к��� */
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
	void _updateState();

	// ִ�а�ť��������
	void _runCallback() override;

protected:
	Node*	_normalOn;
	Node*	_mouseoverOn;
	Node*	_selectedOn;
	Node*	_disabledOn;
	Node*	_normalOff;
	Node*	_mouseoverOff;
	Node*	_selectedOff;
	Node*	_disabledOff;
	bool	_toggle;
};


// �˵�
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