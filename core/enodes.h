#pragma once
#include "ebase.h"

namespace e2d 
{


class Action;
class Shape;
class Transition;

class Node :
	public Object
{
	friend Scene;
	friend Shape;
	friend Transition;

public:
	Node();

	virtual ~Node();

	// ���볡��ʱִ��
	virtual void onEnter() {}

	// �뿪����ʱִ��
	virtual void onExit() {}

	// ���½ڵ�
	virtual void onUpdate() {}

	// �̶��ظ��£���Ϸ��ͣʱ��Ȼ���У�
	virtual void onFixedUpdate() {}

	// ��Ⱦ�ڵ�
	virtual void onRender() {}

	// ��ײ����
	virtual void onCollide(
		Node* pCollisionNode,	/* ������ײ�Ľڵ� */
		int nRelation			/* ��ײ��ϵ��ȡֵΪ ERelation::VALUE �е�һ�� */
	) {}

	// �ڵ㱻����ʱ�Ĵ���
	virtual void onDestroy() {}

	// ��ȡ�ڵ���ʾ״̬
	virtual bool isVisiable() const;

	// �жϵ��Ƿ��ڽڵ���
	virtual bool isPointIn(
		Point point
	);

	// ��ȡ�ڵ�����
	virtual String getName() const;

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

	// ��ȡ�ڵ���״
	virtual Shape * getShape() const;

	// ��ȡ���ڵ�
	virtual Node * getParent() const;

	// ��ȡ�ڵ����ڳ���
	virtual Scene * getParentScene() const;

	// ��ȡ������ͬ���ӽڵ�
	virtual Node * getChild(
		const String & name
	);

	// ��ȡ����������ͬ���ӽڵ�
	virtual std::vector<Node*> getChildren(
		const String & name
	);

	// ��ȡ�����ӽڵ�
	virtual std::vector<Node*> &getChildren();

	// ��ȡ�ӽڵ�����
	virtual int getChildrenCount() const;

	// �Ƴ��ӽڵ�
	virtual bool removeChild(
		Node * child
	);

	// �Ƴ�����������ͬ���ӽڵ�
	virtual void removeChildren(
		const String & childName
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
		const String & name
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
	void setWidth(
		double width
	);

	// �޸Ľڵ�߶�
	void setHeight(
		double height
	);

	// �޸Ľڵ��С
	void setSize(
		double width,
		double height
	);

	// �޸Ľڵ��С
	void setSize(
		Size size
	);

	// ���ýڵ���״
	virtual void setShape(
		Shape * pShape
	);

	// ����ӽڵ�
	virtual void addChild(
		Node * child,
		int order = 0
	);

	// ִ�ж���
	virtual void runAction(
		Action * action
	);

	// ��������
	virtual void resumeAction(
		Action * action
	);

	// ��ͣ����
	virtual void pauseAction(
		Action * action
	);

	// ֹͣ����
	virtual void stopAction(
		Action * action
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

protected:
	// ���½ڵ�
	void _update();

	// ��Ⱦ�ڵ�
	void _render();

	// ��Ⱦͼ��
	void _drawShape();

	// �ڵ㱻��ӵ�����ʱ��ִ�г���
	void _onEnter();

	// �ڵ�ӳ�������ʧʱ��ִ�г���
	void _onExit();

	// ���ýڵ����ڳ���
	void _setParentScene(
		Scene * scene
	);

	// ��������ж�ά����任
	void _updateTransform();

	// ���������ӽڵ����
	void _updateChildrenTransform();

	// ���������ӽڵ�͸����
	void _updateChildrenOpacity();

	// ���½ڵ��ά����
	static void _updateTransform(Node * node);

	// ���½ڵ�͸����
	static void _updateOpacity(Node * node);

protected:
	String		m_sName;
	unsigned	m_nHashName;
	float		m_fPosX;
	float		m_fPosY;
	float		m_fWidth;
	float		m_fHeight;
	float		m_fScaleX;
	float		m_fScaleY;
	float		m_fRotation;
	float		m_fSkewAngleX;
	float		m_fSkewAngleY;
	float		m_fDisplayOpacity;
	float		m_fRealOpacity;
	float		m_fPivotX;
	float		m_fPivotY;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bAutoUpdate;
	bool		m_bDisplayedInScene;
	bool		m_bSortChildrenNeeded;
	bool		m_bTransformNeeded;
	Shape *	m_pShape;
	Scene *		m_pParentScene;
	Node *		m_pParent;
	D2D1::Matrix3x2F	m_MatriInitial;
	D2D1::Matrix3x2F	m_MatriFinal;
	std::vector<Node*>	m_vChildren;
};


class Sprite :
	public Node
{
public:
	// ����һ���վ���
	Sprite();

	// �� EImage ���󴴽�����
	Sprite(
		Image * image
	);

	// ���ļ�ͼƬ��������
	Sprite(
		const String & imageFileName
	);

	// ���ļ�ͼƬ�������鲢�ü�
	Sprite(
		const String & imageFileName,
		double x,
		double y,
		double width,
		double height
	);

	virtual ~Sprite();
	
	// ���ؾ���ͼƬ
	virtual void open(
		Image * texture
	);

	// �ӱ����ļ�����ͼƬ
	virtual void open(
		const String & imageFileName
	);

	// �ü�ͼƬ
	virtual void clip(
		double x,
		double y,
		double width,
		double height
	);

	// ��ȡ EImage ����
	virtual Image * getImage() const;

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	Image * m_pImage;
};


class Text :
	public Node
{
public:
	Text();

	Text(
		const String & text	/* �������� */
	);

	Text(
		Font * font			/* ������ʽ */
	);

	Text(
		const String & text,/* �������� */
		Font * font			/* ������ʽ */
	);

	Text(
		const String & text,					/* ��������*/
		String fontFamily,						/* ���� */
		double fontSize = 22,					/* �ֺ� */
		UINT32 color = Color::WHITE,			/* ��ɫ */
		UINT32 fontWeight = FontWeight::REGULAR,/* ��ϸֵ */
		bool italic = false						/* б�� */
	);

	virtual ~Text();

	// ��ȡ�ı�
	String getText() const;

	// ��ȡ�ı����
	virtual double getWidth() const override;

	// ��ȡ�ı���ȣ����������ţ�
	virtual double getRealWidth() const override;

	// ��ȡ����
	Font * getFont() const;

	// �����ı�
	void setText(
		const String & text
	);

	// ��������
	void setFont(
		Font * pFont
	);

	// ���������Զ�����
	void setWordWrappingEnable(
		bool bEnable
	);

	// �������ֻ��п�ȣ�WordWrapping ��ʱ��Ч��
	void setWordWrappingWidth(
		double fWordWrapWidth
	);

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	// �������ֲ���
	void _initTextLayout();

protected:
	String	m_sText;
	bool	m_bWordWrapping;
	float	m_fWordWrappingWidth;
	Font * m_pFont;
};


class Button :
	public Node
{
public:
	// ����һ���հ�ť
	Button();

	// ������ť
	Button(
		Node * normal,		/* ��ͨ״̬ */
		const ButtonCallback & callback = nullptr
	);

	// ������ť
	Button(
		Node * normal,		/* ��ͨ״̬ */
		Node * selected,	/* ��갴��״̬ */
		const ButtonCallback & callback = nullptr
	);

	// ������ť
	Button(
		Node * normal,		/* ��ͨ״̬ */
		Node * mouseover,	/* �������״̬ */
		Node * selected,	/* ��갴��״̬ */
		const ButtonCallback & callback = nullptr
	);

	// ������ť
	Button(
		Node * normal,		/* ��ͨ״̬ */
		Node * mouseover,	/* �������״̬ */
		Node * selected,	/* �������״̬ */
		Node * disabled,	/* ��ť����״̬ */
		const ButtonCallback & callback = nullptr
	);

	// ��ȡ��ť״̬�����û��ǽ���
	bool isEnable() const;

	// ���ð�ť���û����
	void setEnable(
		bool bEnable
	);

	// ����һ���������ʾ�İ�ť
	virtual void setNormal(
		Node * normal
	);

	// ����������밴ťʱ��ʾ�İ�ť
	virtual void setMouseOver(
		Node * mouseover
	);

	// �������ѡ�а�ťʱ��ʾ�İ�ť
	virtual void setSelected(
		Node * selected
	);

	// ���ð�ť������ʱ��ʾ�İ�ť
	virtual void setDisabled(
		Node * disabled
	);

	// ���ûص�����
	void setCallback(
		const ButtonCallback & callback
	);

	// ���°�ť״̬
	virtual void onFixedUpdate() override;

protected:
	enum BTN_STATE { NORMAL, MOUSEOVER, SELECTED };

	// ���ð�ť״̬
	virtual void _setState(BTN_STATE state);

	// ˢ�°�ť��ʾ
	virtual void _updateVisiable();

	// ִ�а�ť�ص�����
	virtual void _runCallback();

protected:
	Node *			m_pNormal;
	Node *			m_pMouseover;
	Node *			m_pSelected;
	Node *			m_pDisabled;
	bool			m_bEnable;
	bool			m_bIsSelected;
	BTN_STATE		m_eBtnState;
	ButtonCallback	m_Callback;
};


class ButtonToggle :
	public Button
{
public:
	// ����һ���յĿ��ذ�ť
	ButtonToggle();

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,
		Node * offNormal,
		const ButtonCallback & callback = nullptr
	);

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,
		Node * offNormal,
		Node * onSelected,
		Node * offSelected,
		const ButtonCallback & callback = nullptr
	);

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,
		Node * offNormal,
		Node * onMouseOver,
		Node * offMouseOver,
		Node * onSelected,
		Node * offSelected,
		const ButtonCallback & callback = nullptr
	);

	// �������ذ�ť
	ButtonToggle(
		Node * onNormal,
		Node * offNormal,
		Node * onMouseOver,
		Node * offMouseOver,
		Node * onSelected,
		Node * offSelected,
		Node * onDisabled,
		Node * offDisabled,
		const ButtonCallback & callback = nullptr
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

	// ���ð�ť��״̬�£����ѡ�а�ťʱ��ʾ�İ�ť
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

	// ���ð�ť�ر�״̬�£����ѡ�а�ťʱ��ʾ�İ�ť
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

	// ִ�а�ť�ص�����
	virtual void _runCallback() override;

protected:
	Node * m_pNormalOn;
	Node * m_pNormalOff;
	Node * m_pMouseoverOn;
	Node * m_pMouseoverOff;
	Node * m_pSelectedOn;
	Node * m_pSelectedOff;
	Node * m_pDisabledOn;
	Node * m_pDisabledOff;
	bool	m_bState;
};


class Menu :
	public Node
{
public:
	// �����ղ˵�
	Menu();

	// �����˵�
	Menu(
		int number,			/* �˵��а�ť������ */
		Button * button1,	/* ��һ����ť */
		...
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

protected:
	bool m_bEnable;
	std::vector<Button*> m_vButtons;
};

}