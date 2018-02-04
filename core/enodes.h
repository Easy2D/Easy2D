#pragma once
#include "ebase.h"

namespace e2d 
{


class EAction;
class EShape;
class ETransition;

class ENode :
	public EObject
{
	friend EScene;
	friend EShape;
	friend ETransition;

public:
	ENode();

	virtual ~ENode();

	// ���볡��ʱִ��
	virtual void onEnter() {}

	// �뿪����ʱִ��
	virtual void onExit() {}

	// ���½ڵ�
	virtual void onUpdate() {}

	// ��Ⱦ�ڵ�
	virtual void onRender() {}

	// ��ײ����
	virtual void onCollide(
		ENode* pCollisionNode,	/* ������ײ�Ľڵ� */
		int nRelation			/* ��ײ��ϵ��ȡֵΪ ERelation::VALUE �е�һ�� */
	) {}

	// ��Ϸ��ͣʱ�Ĵ���
	virtual void onPause() {}

	// �ڵ㱻����ʱ�Ĵ���
	virtual void onDestroy() {}

	// ��ȡ�ڵ���ʾ״̬
	virtual bool isVisiable() const;

	// �жϵ��Ƿ��ڽڵ���
	virtual bool isPointIn(
		EPoint point
	);

	// ��ȡ�ڵ�����
	virtual EString getName() const;

	// ��ȡ�ڵ��ͼ˳��
	virtual int getOrder() const;

	// ��ȡ�ڵ������
	virtual float getPosX() const;

	// ��ȡ�ڵ�������
	virtual float getPosY() const;

	// ��ȡ�ڵ�����
	virtual EPoint getPos() const;

	// ��ȡ�ڵ���
	virtual float getWidth() const;

	// ��ȡ�ڵ�߶�
	virtual float getHeight() const;

	// ��ȡ�ڵ��ȣ����������ţ�
	virtual float getRealWidth() const;

	// ��ȡ�ڵ�߶ȣ����������ţ�
	virtual float getRealHeight() const;

	// ��ȡ�ڵ��С�����������ţ�
	virtual ESize getRealSize() const;

	// ��ȡ�ڵ�����ĵ�
	virtual float getPivotX() const;

	// ��ȡ�ڵ�����ĵ�
	virtual float getPivotY() const;

	// ��ȡ�ڵ��С
	virtual ESize getSize() const;

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
	virtual EShape * getShape() const;

	// ��ȡ���ڵ�
	virtual ENode * getParent() const;

	// ��ȡ�ڵ����ڳ���
	virtual EScene * getParentScene() const;

	// ��ȡ������ͬ���ӽڵ�
	virtual ENode * getChild(
		const EString & name
	);

	// ��ȡ����������ͬ���ӽڵ�
	virtual std::vector<ENode*> getChildren(
		const EString & name
	);

	// ��ȡ�����ӽڵ�
	virtual std::vector<ENode*> &getChildren();

	// ��ȡ�ӽڵ�����
	virtual int getChildrenCount() const;

	// �Ƴ��ӽڵ�
	virtual bool removeChild(
		ENode * child
	);

	// �Ƴ�����������ͬ���ӽڵ�
	virtual void removeChildren(
		const EString & childName
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
		const EString & name
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
		const EPoint & point
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
		const EVector2 & v
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
		EShape * pShape
	);

	// ����ӽڵ�
	virtual void addChild(
		ENode * child,
		int order = 0
	);

	// ִ�ж���
	virtual void runAction(
		EAction * action
	);

	// ��������
	virtual void resumeAction(
		EAction * action
	);

	// ��ͣ����
	virtual void pauseAction(
		EAction * action
	);

	// ֹͣ����
	virtual void stopAction(
		EAction * action
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
	void _update(
		bool bPaused
	);

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
		EScene * scene
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
	static void _updateTransform(ENode * node);

	// ���½ڵ�͸����
	static void _updateOpacity(ENode * node);

protected:
	EString		m_sName;
	size_t		m_nHashName;
	EPoint		m_Pos;
	ESize		m_Size;
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
	EShape *	m_pShape;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	D2D1::Matrix3x2F	m_MatriInitial;
	D2D1::Matrix3x2F	m_MatriFinal;
	std::vector<ENode*>	m_vChildren;
};


class ESprite :
	public ENode
{
public:
	// ����һ���վ���
	ESprite();

	// �� EImage ���󴴽�����
	ESprite(
		EImage * image
	);

	// ���ļ�ͼƬ��������
	ESprite(
		LPCTSTR imageFileName
	);

	// ���ļ�ͼƬ�������鲢�ü�
	ESprite(
		LPCTSTR imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESprite();
	
	// ���ؾ���ͼƬ
	virtual void loadFrom(
		EImage * texture
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
	virtual EImage * getImage() const;

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	EImage * m_pImage;
};


class EText :
	public ENode
{
public:
	EText();

	EText(
		const EString & text
	);

	EText(
		EFont * font
	);

	EText(
		const EString & text,
		EFont * font
	);

	EText(
		const EString & text,
		EString fontFamily,
		float fontSize = 22,
		UINT32 color = EColor::WHITE,
		UINT32 fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EText();

	// ��ȡ�ı�
	EString getText() const;

	// ��ȡ�ı����
	virtual float getWidth() const override;

	// ��ȡ�ı���ȣ����������ţ�
	virtual float getRealWidth() const override;

	// ��ȡ����
	EFont * getFont() const;

	// �����ı�
	void setText(
		const EString & text
	);

	// ��������
	void setFont(
		EFont * font
	);

	// ���������Զ�����
	void setWordWrapping(
		bool value
	);

	// �������ֻ��п�ȣ�WordWrapping ��ʱ��Ч��
	void setWordWrappingWidth(
		float wordWrapWidth
	);

	// ��Ⱦ����
	virtual void onRender() override;

protected:
	// �������ֲ���
	void _initTextLayout();

protected:
	EString	m_sText;
	bool	m_bWordWrapping;
	float	m_fWordWrappingWidth;
	EFont * m_pFont;
};


class EButton :
	public ENode
{
public:
	// ����һ���հ�ť
	EButton();

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		const BtnClkCallback & callback = nullptr
	);

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		ENode * selected,	/* ��갴��״̬ */
		const BtnClkCallback & callback = nullptr
	);

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		ENode * mouseover,	/* �������״̬ */
		ENode * selected,	/* ��갴��״̬ */
		const BtnClkCallback & callback = nullptr
	);

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		ENode * mouseover,	/* �������״̬ */
		ENode * selected,	/* �������״̬ */
		ENode * disabled,	/* ��ť����״̬ */
		const BtnClkCallback & callback = nullptr
	);

	// ��ȡ��ť״̬�����û��ǽ���
	bool isEnable() const;

	// ���ð�ť���û����
	void setEnable(
		bool bEnable
	);

	// ����һ���������ʾ�İ�ť
	virtual void setNormal(
		ENode * normal
	);

	// ����������밴ťʱ��ʾ�İ�ť
	virtual void setMouseOver(
		ENode * mouseover
	);

	// �������ѡ�а�ťʱ��ʾ�İ�ť
	virtual void setSelected(
		ENode * selected
	);

	// ���ð�ť������ʱ��ʾ�İ�ť
	virtual void setDisabled(
		ENode * disabled
	);

	// ���ûص�����
	void setCallback(
		const BtnClkCallback & callback
	);

	// �������ĵ�ĺ���λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotX(
		float pivotX
	) override;

	// �������ĵ������λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotY(
		float pivotY
	) override;

	// �������ĵ�λ��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
	) override;

	// ���°�ť״̬
	virtual void onUpdate() override;

	// ������Ϸ��ͣʱ�İ�ť״̬
	virtual void onPause() override;

protected:
	enum BTN_STATE { NORMAL, MOUSEOVER, SELECTED };

	// ���ð�ť״̬
	virtual void _setState(BTN_STATE state);

	// ˢ�°�ť��ʾ
	virtual void _updateVisiable();

	// ִ�а�ť�ص�����
	virtual void _runCallback();

protected:
	ENode *			m_pNormal;
	ENode *			m_pMouseover;
	ENode *			m_pSelected;
	ENode *			m_pDisabled;
	bool			m_bEnable;
	bool			m_bIsSelected;
	BTN_STATE		m_eBtnState;
	BtnClkCallback	m_Callback;
};


class EButtonToggle :
	public EButton
{
public:
	// ����һ���յĿ��ذ�ť
	EButtonToggle();

	// �������ذ�ť
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		const BtnClkCallback & callback = nullptr
	);

	// �������ذ�ť
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		ENode * onSelected,
		ENode * offSelected,
		const BtnClkCallback & callback = nullptr
	);

	// �������ذ�ť
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		ENode * onMouseOver,
		ENode * offMouseOver,
		ENode * onSelected,
		ENode * offSelected,
		const BtnClkCallback & callback = nullptr
	);

	// �������ذ�ť
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		ENode * onMouseOver,
		ENode * offMouseOver,
		ENode * onSelected,
		ENode * offSelected,
		ENode * onDisabled,
		ENode * offDisabled,
		const BtnClkCallback & callback = nullptr
	);

	// �л�����״̬��ִ�лص�������
	void toggle();

	// ��ȡ����״̬
	bool getState() const;

	// ���ÿ��ذ�ť��״̬���򿪻�رգ�
	void setState(
		bool bState
	);

	// ���ð�ť��״̬����ʾ�İ�ť
	virtual void setNormal(
		ENode * normal
	) override;

	// ���ð�ť��״̬�£�������밴ťʱ��ʾ�İ�ť
	virtual void setMouseOver(
		ENode * mouseover
	) override;

	// ���ð�ť��״̬�£����ѡ�а�ťʱ��ʾ�İ�ť
	virtual void setSelected(
		ENode * selected
	) override;

	// ���ð�ť��״̬�£�������ʱ��ʾ�İ�ť
	virtual void setDisabled(
		ENode * disabled
	) override;

	// ���ð�ť�ر�״̬����ʾ�İ�ť
	void setNormalOff(
		ENode * normal
	);

	// ���ð�ť�ر�״̬�£�������밴ťʱ��ʾ�İ�ť
	void setMouseOverOff(
		ENode * mouseover
	);

	// ���ð�ť�ر�״̬�£����ѡ�а�ťʱ��ʾ�İ�ť
	void setSelectedOff(
		ENode * selected
	);

	// ���ð�ť�ر�״̬�£���ť������ʱ��ʾ�İ�ť
	void setDisabledOff(
		ENode * disabled
	);

	// �������ĵ�ĺ���λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotX(
		float pivotX
	) override;

	// �������ĵ������λ��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setPivotY(
		float pivotY
	) override;

	// �������ĵ�λ��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
	) override;

protected:
	// ˢ�°�ť����
	virtual void _updateState();

	// ִ�а�ť�ص�����
	virtual void _runCallback() override;

protected:
	ENode * m_pNormalOn;
	ENode * m_pNormalOff;
	ENode * m_pMouseoverOn;
	ENode * m_pMouseoverOff;
	ENode * m_pSelectedOn;
	ENode * m_pSelectedOff;
	ENode * m_pDisabledOn;
	ENode * m_pDisabledOff;
	bool	m_bState;
};


class EMenu :
	public ENode
{
public:
	// �����ղ˵�
	EMenu();

	// �����˵�
	EMenu(
		int number,			/* �˵��а�ť������ */
		EButton * button1,	/* ��һ����ť */
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
		EButton * button
	);

	// �Ƴ���ť
	bool removeButton(
		EButton * button
	);

protected:
	bool m_bEnable;
	std::vector<EButton*> m_vButtons;
};

}