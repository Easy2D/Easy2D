#pragma once
#include "ebase.h"

namespace e2d 
{


class Action;
class Shape;
class Transition;

class Node :
	public Obj
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
	virtual float getPosX() const;

	// ��ȡ�ڵ�������
	virtual float getPosY() const;

	// ��ȡ�ڵ�����
	virtual Point getPos() const;

	// ��ȡ�ڵ���
	virtual float getWidth() const;

	// ��ȡ�ڵ�߶�
	virtual float getHeight() const;

	// ��ȡ�ڵ��ȣ����������ţ�
	virtual float getRealWidth() const;

	// ��ȡ�ڵ�߶ȣ����������ţ�
	virtual float getRealHeight() const;

	// ��ȡ�ڵ��С�����������ţ�
	virtual Size getRealSize() const;

	// ��ȡ�ڵ�����ĵ�
	virtual float getPivotX() const;

	// ��ȡ�ڵ�����ĵ�
	virtual float getPivotY() const;

	// ��ȡ�ڵ��С
	virtual Size getSize() const;

	// ��ȡ�ڵ�������ű���
	virtual float getScaleX() const;

	// ��ȡ�ڵ��������ű���
	virtual float getScaleY() const;

	// ��ȡ�ڵ������б�Ƕ�
	virtual float getSkewX() const;

	// ��ȡ�ڵ�������б�Ƕ�
	virtual float getSkewY() const;

	// ��ȡ�ڵ���ת�Ƕ�
	virtual float getRotation() const;

	// ��ȡ�ڵ�͸����
	virtual float getOpacity() const;

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
		float x
	);

	// ���ýڵ�������
	virtual void setPosY(
		float y
	);

	// ���ýڵ�����
	virtual void setPos(
		const Point & point
	);

	// ���ýڵ�����
	virtual void setPos(
		float x,
		float y
	);

	// �ƶ��ڵ�
	virtual void movePosX(
		float x
	);

	// �ƶ��ڵ�
	virtual void movePosY(
		float y
	);

	// �ƶ��ڵ�
	virtual void movePos(
		float x,
		float y
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
		float scaleX
	);

	// �����������ű���
	// Ĭ��Ϊ 1.0f
	virtual void setScaleY(
		float scaleY
	);

	// �������ű���
	// Ĭ��Ϊ (1.0f, 1.0f)
	virtual void setScale(
		float scaleX,
		float scaleY
	);

	// �������ű���
	// Ĭ��Ϊ 1.0f
	virtual void setScale(
		float scale
	);

	// ���ú�����б�Ƕ�
	// Ĭ��Ϊ 0
	virtual void setSkewX(
		float angleX
	);

	// ����������б�Ƕ�
	// Ĭ��Ϊ 0
	virtual void setSkewY(
		float angleY
	);

	// ������б�Ƕ�
	// Ĭ��Ϊ (0, 0)
	virtual void setSkew(
		float angleX,
		float angleY
	);

	// ������ת�Ƕ�
	// Ĭ��Ϊ 0
	virtual void setRotation(
		float rotation
	);

	// ����͸����
	// Ĭ��Ϊ 1.0f, ��Χ [0, 1]
	virtual void setOpacity(
		float opacity
	);

	// �������ĵ�ĺ���λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotX(
		float pivotX
	);

	// �������ĵ������λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotY(
		float pivotY
	);

	// �������ĵ�λ��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
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
		float defaultPiovtX,
		float defaultPiovtY
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

	// �޸Ľڵ��С
	void _setSize(
		float width,
		float height
	);

	// ���½ڵ��ά����
	static void _updateTransform(Node * node);

	// ���½ڵ�͸����
	static void _updateOpacity(Node * node);

protected:
	String		m_sName;
	size_t		m_nHashName;
	Point		m_Pos;
	Size		m_Size;
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
		LPCTSTR imageFileName
	);

	// ���ļ�ͼƬ�������鲢�ü�
	Sprite(
		LPCTSTR imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~Sprite();
	
	// ���ؾ���ͼƬ
	virtual void loadFrom(
		Image * texture
	);

	// �ӱ����ļ�����ͼƬ
	virtual void loadFrom(
		LPCTSTR imageFileName
	);

	// �ü�ͼƬ
	virtual void clip(
		float x,
		float y,
		float width,
		float height
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
		const String & text
	);

	Text(
		Font * font
	);

	Text(
		const String & text,
		Font * font
	);

	Text(
		const String & text,
		String fontFamily,
		float fontSize = 22,
		UINT32 color = Color::WHITE,
		UINT32 fontWeight = FontWeight::REGULAR,
		bool italic = false
	);

	virtual ~Text();

	// ��ȡ�ı�
	String getText() const;

	// ��ȡ�ı����
	virtual float getWidth() const override;

	// ��ȡ�ı���ȣ����������ţ�
	virtual float getRealWidth() const override;

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
		float fWordWrapWidth
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