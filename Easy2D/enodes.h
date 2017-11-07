#pragma once
#include "ebase.h"

namespace e2d 
{

class EText;
class ESprite;
class EAction;
class EButton;
class EButtonToggle;
class EGeometry;

class ENode :
	public EObject
{
	friend EScene;
	friend EButton;
	friend EButtonToggle;
	friend EGeometry;

public:
	ENode();

	ENode(
		const EString & name
	);

	virtual ~ENode();

	// ��д��������������ڽڵ���볡��ʱ�Զ�ִ��
	virtual void onEnter();

	// ��д��������������ڽڵ��뿪����ʱ�Զ�ִ��
	virtual void onExit();

	// ��ȡ�ڵ���ʾ״̬
	virtual bool isVisiable() const;

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

	// ��ȡ�ڵ��ê��
	virtual float getPivotX() const;

	// ��ȡ�ڵ��ê��
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

	// ��ȡ���ڵ�
	virtual ENode * getParent() const;

	// ��ȡ�ڵ����ڳ���
	virtual EScene * getParentScene() const;

	// ��ȡ�����ӽڵ�
	virtual EVector<ENode*> &getChildren();

	// ��ȡ�ӽڵ�����
	virtual size_t getChildrenCount() const;

	// �������ֻ�ȡ�ӽڵ�
	virtual ENode * getChild(
		const EString & name
	);

	// ���ýڵ��Ƿ���ʾ
	virtual void setVisiable(
		bool value
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
		const EVec & v
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

	// ����֧��ĺ���λ��
	// Ĭ��Ϊ 0.5f, ��Χ [0, 1]
	virtual void setPivotX(
		float pivotX
	);

	// ����֧�������λ��
	// Ĭ��Ϊ 0.5f, ��Χ [0, 1]
	virtual void setPivotY(
		float pivotY
	);

	// ����֧��λ��
	// Ĭ��Ϊ (0.5f, 0.5f), ��Χ [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
	);

	// ���ýڵ���״
	virtual void setGeometry(
		EGeometry * geometry
	);

	// ����ӽڵ�
	virtual void addChild(
		ENode * child,
		int order = 0
	);

	// �Ӹ��ڵ��Ƴ�
	virtual void removeFromParent();

	// �Ƴ��ӽڵ�
	virtual bool removeChild(
		ENode * child
	);

	// �Ƴ��ӽڵ�
	virtual void removeChild(
		const EString & childName
	);

	// �Ƴ����нڵ�
	virtual void clearAllChildren();

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
	virtual void startAllActions();

	// ��ͣ���ж���
	virtual void pauseAllActions();

	// ֹͣ���ж���
	virtual void stopAllActions();

	// �жϵ��Ƿ��ڽڵ���
	virtual bool isPointIn(
		EPoint point
	);

protected:
	// ���ʽڵ�
	virtual void _callOn();

	// ��Ⱦ�ڵ�
	virtual void _onRender();

	// ��Ⱦ����ͼ��
	virtual void _drawGeometry();

	// �ڵ㱻��ӵ�����ʱ��ִ�г���
	virtual void _onEnter();

	// �ڵ�ӳ�������ʧʱ��ִ�г���
	virtual void _onExit();

	// �ӽڵ�����
	void _sortChildren();

	// ���ýڵ����ڳ���
	virtual void _setParentScene(
		EScene * scene
	);

	// ֻ����������ж�ά����任
	void _updateTransformToReal();

	// ���������ӽڵ����
	void _updateChildrenTransform();

	// ���������ӽڵ�͸����
	void _updateChildrenOpacity();

	// ���½ڵ����
	static void _updateTransform(ENode * node);

	// ���½ڵ�͸����
	static void _updateOpacity(ENode * node);

	// ���ýڵ���
	virtual void _setWidth(
		float width
	);

	// ���ýڵ�߶�
	virtual void _setHeight(
		float height
	);

	// ���ýڵ��С
	virtual void _setSize(
		const ESize & size
	);

	// ���ýڵ��С
	virtual void _setSize(
		float width,
		float height
	);

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
	bool		m_bDisplayedInScene;
	bool		m_bSortChildrenNeeded;
	bool		m_bTransformChildrenNeeded;
	EGeometry * m_pGeometry;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	D2D1::Matrix3x2F	m_Matri;
	EVector<ENode*>		m_vChildren;
};


class ESpriteFrame :
	public EObject
{
	friend ESprite;

public:
	// �����յľ���֡
	ESpriteFrame();

	// �����յľ���֡
	ESpriteFrame(
		ETexture * texture
	);

	// �����յľ���֡
	ESpriteFrame(
		const EString & imageFileName
	);

	// �����յľ���֡
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// �����յľ���֡
	ESpriteFrame(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// �����յľ���֡
	ESpriteFrame(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// �����յľ���֡
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESpriteFrame();

	// ��ȡ���
	float getWidth() const;

	// ��ȡ�߶�
	float getHeight() const;

	// ��ȡ����
	ETexture * getTexture() const;

protected:
	// ��ȡ����
	void _setTexture(
		ETexture * texture
	);

	// �ü�����
	void _clipTexture(
		float x,
		float y,
		float width,
		float height
	);

protected:
	float	m_fSourceClipX;
	float	m_fSourceClipY;
	float	m_fSourceClipWidth;
	float	m_fSourceClipHeight;
	ETexture * m_pTexture;
};


class ESprite :
	public ENode
{
public:
	// ����һ���վ���
	ESprite();

	// ��������Դ��������
	ESprite(
		ETexture * texture
	);

	// �Ӿ���֡��������
	ESprite(
		ESpriteFrame * spriteFrame
	);

	// ���ļ�ͼƬ��������
	ESprite(
		const EString & imageFileName
	);

	// ���ļ�ͼƬ�������鲢�ü�
	ESprite(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// ����ԴͼƬ��������
	ESprite(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// ����ԴͼƬ�������鲢�ü�
	ESprite(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESprite();
	
	// ���ؾ�������
	void loadFrom(
		ETexture * texture
	);

	// �ӱ����ļ���������
	void loadFrom(
		const EString & imageFileName
	);

	// ����Դ��������
	void loadFrom(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// ���������ü�
	void loadFrom(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// �Ӿ���֡������Դ
	void loadFrom(
		ESpriteFrame * frame
	);

	// �ü�����
	void clip(
		float x,
		float y,
		float width,
		float height
	);

protected:
	// ��Ⱦ����
	virtual void _onRender() override;

protected:
	float	m_fSourceClipX;
	float	m_fSourceClipY;
	ETexture * m_pTexture;
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

protected:
	// ��Ⱦ����
	virtual void _onRender() override;

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
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		ENode * selected,	/* ��갴��״̬ */
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		ENode * mouseover,	/* �������״̬ */
		ENode * selected,	/* ��갴��״̬ */
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// ������ť
	EButton(
		ENode * normal,		/* ��ͨ״̬ */
		ENode * mouseover,	/* �������״̬ */
		ENode * selected,	/* �������״̬ */
		ENode * disabled,	/* ��ť����״̬ */
		const BUTTON_CLICK_CALLBACK & callback = nullptr
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
		const BUTTON_CLICK_CALLBACK & callback
	);

protected:
	enum STATUS { NORMAL, MOUSEOVER, SELECTED };

	// ���ð�ť״̬
	virtual void _setStatus(STATUS status);

	// ˢ�°�ť��ʾ
	virtual void _updateVisiable();

	// ˢ�°�ť״̬
	virtual void _updateStatus();

	// ִ�а�ť�ص�����
	virtual void _runCallback();

protected:
	STATUS	m_eStatus;
	ENode * m_pNormal;
	ENode * m_pMouseover;
	ENode * m_pSelected;
	ENode * m_pDisabled;
	bool	m_bEnable;
	bool	m_bIsSelected;
	EListenerMouse * m_pListener;
	BUTTON_CLICK_CALLBACK m_Callback;
};


class EButtonToggle :
	public EButton
{
public:
	// ����һ���յĿ��ذ�ť
	EButtonToggle();

	// �������ذ�ť
	EButtonToggle(
		ENode * toggleOnNormal,
		ENode * toggleOffNormal,
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// �������ذ�ť
	EButtonToggle(
		ENode * toggleOnNormal,
		ENode * toggleOffNormal,
		ENode * toggleOnSelected,
		ENode * toggleOffSelected,
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// �������ذ�ť
	EButtonToggle(
		ENode * toggleOnNormal,
		ENode * toggleOffNormal,
		ENode * toggleOnMouseOver,
		ENode * toggleOffMouseOver,
		ENode * toggleOnSelected,
		ENode * toggleOffSelected,
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// �������ذ�ť
	EButtonToggle(
		ENode * toggleOnNormal,
		ENode * toggleOffNormal,
		ENode * toggleOnMouseOver,
		ENode * toggleOffMouseOver,
		ENode * toggleOnSelected,
		ENode * toggleOffSelected,
		ENode * toggleOnDisabled,
		ENode * toggleOffDisabled,
		const BUTTON_CLICK_CALLBACK & callback = nullptr
	);

	// �л�����״̬
	void toggle();

	// ��ȡ����״̬
	bool isToggleOn() const;

	// �򿪻�رտ���
	void setToggle(
		bool toggle
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

protected:
	// ˢ�°�ť����
	virtual void _updateToggle();

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
	bool	m_bToggle;
};

}