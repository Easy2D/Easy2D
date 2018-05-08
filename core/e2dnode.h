#pragma once
#include "e2dbase.h"

namespace e2d 
{


class Action;
class TransitionBase;
class Collider;
class ColliderManager;

class Node :
	public Object
{
	friend Scene;
	friend Collider;
	friend TransitionBase;
	friend ColliderManager;

public:
	Node();

	virtual ~Node();

	// ���½ڵ�
	virtual void onUpdate() {}

	// �̶��ظ��£���Ϸ��ͣʱ��Ȼ���У�
	virtual void onFixedUpdate() {}

	// ��Ⱦ�ڵ�
	virtual void onRender() {}

	// ��ײ����
	virtual void onCollide(
		Node* node	/* ������ײ�Ľڵ� */
	) {}

	// ��ȡ�ڵ���ʾ״̬
	virtual bool isVisiable() const;

	// �жϵ��Ƿ��ڽڵ���
	virtual bool isPointIn(
		Point point
	) const;

	// �ж����ڵ��Ƿ��ཻ
	virtual bool isIntersectWith(
		const Node * node
	) const;

	// ��ȡ�ڵ�����
	virtual String getName() const;

	// ��ȡ�ڵ����Ƶ� Hash ֵ
	virtual unsigned int getHashName() const;

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
	virtual NodeProperty getProperty() const;

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
	virtual std::vector<Node*> getAllChildren() const;

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
		NodeProperty prop
	);

	// ������ײ��
	virtual void setCollider(
		ColliderType nColliderType
	);

	// ������ײ��
	virtual void setCollider(
		Collider * pCollider
	);

	// ��ӿ���ײ�ڵ������
	virtual void addColliableName(
		const String& collliderName
	);

#ifdef HIGHER_THAN_VS2012
	// ��Ӷ������ײ�ڵ������
	virtual void addColliableName(
		const std::initializer_list<String>& vCollliderName	/* �����б� */
	);
#endif

	// �Ƴ�����ײ�ڵ������
	virtual void removeColliableName(
		const String& collliderName
	);

	// ����ӽڵ�
	virtual void addChild(
		Node * child,
		int order = 0	/* ��Ⱦ˳�� */
	);

#ifdef HIGHER_THAN_VS2012
	// ��Ӷ���ӽڵ�
	virtual void addChild(
		const std::initializer_list<Node*>& vNodes,	/* �ڵ��б� */
		int order = 0								/* ��Ⱦ˳�� */
	);
#endif

	// ִ�ж���
	virtual void runAction(
		Action * action
	);

	// ��������
	virtual void resumeAction(
		const String& strActionName
	);

	// ��ͣ����
	virtual void pauseAction(
		const String& strActionName
	);

	// ֹͣ����
	virtual void stopAction(
		const String& strActionName
	);

	// ��ȡ������ͬ�Ķ���
	virtual Action * getAction(
		const String& strActionName
	);

	// ��ȡ����������ͬ�Ķ���
	virtual std::vector<Action*> getActions(
		const String& strActionName
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

	// ���ýڵ��Ƿ����Ĭ����ײ�壨Ĭ�ϴ򿪣�
	static void setDefaultColliderEnable(
		bool enable
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

	// ��������ж�ά����任
	void _updateSelfTransform();

	// ���½ڵ��ά����
	void _updateTransform();

	// ���½ڵ�͸����
	void _updateOpacity();

protected:
	String		_name;
	unsigned	_nHashName;
	float		_fPosX;
	float		_fPosY;
	float		_fWidth;
	float		_fHeight;
	float		_fScaleX;
	float		_fScaleY;
	float		_fRotation;
	float		_fSkewAngleX;
	float		_fSkewAngleY;
	float		_fDisplayOpacity;
	float		_fRealOpacity;
	float		_fPivotX;
	float		_fPivotY;
	int			_nOrder;
	bool		_bVisiable;
	bool		_bAutoUpdate;
	bool		_bSortChildrenNeeded;
	bool		_bTransformNeeded;
	bool		_bPositionFixed;
	Collider *	_pCollider;
	Scene *		_pParentScene;
	Node *		_pParent;
	D2D1::Matrix3x2F		_MatriInitial;
	D2D1::Matrix3x2F		_MatriFinal;
	std::set<unsigned int>	_vColliders;
	std::vector<Node*>		_vChildren;
};


class Sprite :
	public Node
{
public:
	// ����һ���վ���
	Sprite();

	// �� Image ���󴴽�����
	Sprite(
		Image * image
	);

	// ����ͼƬ�ļ�
	Sprite(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	// ����ͼƬ��Դ
	Sprite(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	// ����ͼƬ�ļ�
	Sprite(
		const String& filePath,	/* ͼƬ�ļ�·�� */
		double x,
		double y,
		double width,
		double height
	);

	// ����ͼƬ��Դ
	Sprite(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType,	/* ͼƬ��Դ���� */
		double x,
		double y,
		double width,
		double height
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
		double x,
		double y,
		double width,
		double height
	);

	// ��ȡ Image ����
	virtual Image * getImage() const;

	// ��Ⱦ����
	virtual void onRender() override;

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	Image * _pImage;
};


class Text :
	public Node
{
public:
	Text();

	Text(
		const String& text		/* �������� */
	);

	Text(
		TextStyle textStyle		/* ������ʽ */
	);

	Text(
		const String& text,		/* �������� */
		TextStyle textStyle		/* ������ʽ */
	);

	Text(
		const String& text,						/* ��������*/
		const String& fontFamily,				/* ���� */
		double fontSize = 22,					/* �ֺ� */
		UINT32 color = Color::WHITE,			/* ��ɫ */
		UINT32 fontWeight = FontWeight::NORMAL,	/* ��ϸֵ */
		bool italic = false,					/* б�� */
		TextAlign alignment = TextAlign::LEFT,	/* ���뷽ʽ */
		bool wrapping = false,					/* ���Զ����� */
		double wrappingWidth = 0.0,				/* �Զ����п�� */
		double lineSpacing = 0.0,				/* �м�� */
		bool hasUnderline = false,				/* �»��� */
		bool hasStrikethrough = false,			/* ɾ���� */
		bool hasOutline = true,					/* ��ʾ��� */
		UINT32 outlineColor = Color::BLACK,		/* �����ɫ */
		UINT32 outlineWidth = 1.0				/* ����߿� */
	);

	virtual ~Text();

	// ��ȡ�ı�
	String getText() const;

	// ��ȡ�ı���ʽ
	TextStyle getTextStyle() const;

	// ��ȡ����
	String getFontFamily() const;

	// ��ȡ��ǰ�ֺ�
	double getFontSize() const;

	// ��ȡ��ǰ�����ϸֵ
	UINT32 getFontWeight() const;

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
	void setTextStyle(
		TextStyle pTextStyle
	);

	// ��������
	void setFontFamily(
		const String& fontFamily
	);

	// �����ֺţ�Ĭ��ֵΪ 22��
	void setFontSize(
		double fontSize
	);

	// ���������ϸֵ��Ĭ��ֵΪ FontWeight::NORMAL��
	void setFontWeight(
		UINT32 fontWeight
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
		double fWrappingWidth
	);

	// �����м�ࣨĬ��Ϊ 0��
	void setLineSpacing(
		double fLineSpacing
	);

	// ���ö��뷽ʽ��Ĭ��Ϊ TextAlign::LEFT��
	void setAlignment(
		TextAlign align
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
	String	_sText;
	TextStyle _TextStyle;
	IDWriteTextFormat * _pDWriteTextFormat;
	IDWriteTextLayout * _pDWriteTextLayout;
};


class Button :
	public Node
{
public:
	// ����һ���հ�ť
	Button();

	// ������ť
	Button(
		Node * normal,					/* ��ͨ״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
	);

	// ������ť
	Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * selected,				/* ��갴��״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
	);

	// ������ť
	Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * mouseover,				/* �������״̬ */
		Node * selected,				/* ��갴��״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
	);

	// ������ť
	Button(
		Node * normal,					/* ��ͨ״̬ */
		Node * mouseover,				/* �������״̬ */
		Node * selected,				/* �������״̬ */
		Node * disabled,				/* ��ť����״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
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
		const Function& func
	);

	// ���°�ť״̬
	virtual void onFixedUpdate() override;

protected:
	// ��ť״̬ö��
	enum class ButtonState { NORMAL, MOUSEOVER, SELECTED };

	// ���ð�ť״̬
	virtual void _setState(ButtonState state);

	// ˢ�°�ť��ʾ
	virtual void _updateVisiable();

	// ִ�а�ť��������
	virtual void _runCallback();

protected:
	Node *		_pNormal;
	Node *		_pMouseover;
	Node *		_pSelected;
	Node *		_pDisabled;
	bool		_bEnable;
	bool		_bIsSelected;
	ButtonState	_eBtnState;
	Function	_func;
};


class ButtonToggle :
	public Button
{
public:
	// ����һ���յĿ��ذ�ť
	ButtonToggle();

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
	);

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
	);

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onMouseOver,				/* ��ť��ʱ���������״̬ */
		Node * offMouseOver,			/* ��ť�ر�ʱ���������״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
	);

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,				/* ��ť��ʱ����ͨ״̬ */
		Node * offNormal,				/* ��ť�ر�ʱ����ͨ״̬ */
		Node * onMouseOver,				/* ��ť��ʱ���������״̬ */
		Node * offMouseOver,			/* ��ť�ر�ʱ���������״̬ */
		Node * onSelected,				/* ��ť��ʱ����갴��״̬ */
		Node * offSelected,				/* ��ť�ر�ʱ����갴��״̬ */
		Node * onDisabled,				/* ��ť��ʱ������״̬ */
		Node * offDisabled,				/* ��ť�ر�ʱ������״̬ */
		const Function& func = nullptr	/* ��ť������ִ�к��� */
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
	Node *	_pNormalOn;
	Node *	_pNormalOff;
	Node *	_pMouseoverOn;
	Node *	_pMouseoverOff;
	Node *	_pSelectedOn;
	Node *	_pSelectedOff;
	Node *	_pDisabledOn;
	Node *	_pDisabledOff;
	bool	_bState;
};


class Menu :
	public Node
{
public:
	// �����ղ˵�
	Menu();

#ifdef HIGHER_THAN_VS2012
	// �����˵�
	Menu(
		const std::initializer_list<Button*>& vButtons	/* ��ť�б� */
	);
#else
	// �����˵�
	Menu(
		int number,			/* �˵��а�ť������ */
		Button * button1,	/* ��һ����ť */
		...
	);
#endif

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

protected:
	bool _bEnable;
	std::vector<Button*> _vButtons;
};

}