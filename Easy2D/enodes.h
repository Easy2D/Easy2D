#pragma once
#include "ebase.h"

namespace e2d 
{

class EText;
class ESprite;
class EAction;

class ENode :
	public EObject
{
	friend EScene;

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
	virtual void move(
		float x,
		float y
	);

	// �ƶ��ڵ�
	virtual void move(
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

	// ��������ê��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setAnchorX(
		float anchorX
	);

	// ���ú���ê��
	// Ĭ��Ϊ 0, ��Χ [0, 1]
	virtual void setAnchorY(
		float anchorY
	);

	// ����ê��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	virtual void setAnchor(
		float anchorX,
		float anchorY
	);

	// ����ӽڵ�
	virtual void addChild(
		ENode * child,
		int order = 0
	);

	// �Ӹ��ڵ��Ƴ�
	virtual void removeFromParent(
		bool release = false
	);

	// �Ƴ��ӽڵ�
	virtual bool removeChild(
		ENode * child, 
		bool release = true
	);

	// �Ƴ��ӽڵ�
	virtual void removeChild(
		const EString & childName,
		bool release = true
	);

	// �Ƴ����нڵ�
	virtual void clearAllChildren(
		bool release = true
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

protected:
	// ���ʽڵ�
	virtual void _callOn();

	// ��Ⱦ�ڵ�
	virtual void _onRender();

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
	float		m_fAnchorX;
	float		m_fAnchorY;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bDisplayedInScene;
	bool		m_bSortChildrenNeeded;
	bool		m_bTransformChildrenNeeded;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	D2D1::Matrix3x2F	m_Matri;
	EVector<ENode*>		m_vChildren;
};


class ETexture :
	public EObject
{
	friend ESprite;

public:
	// ����һ���յ�����
	ETexture();

	// �ӱ����ļ��ж�ȡ��Դ
	ETexture(
		const EString & fileName
	);

	// ��ȡ������Դ
	ETexture(
		const EString & resourceName,
		const EString & resourceType
	);

	virtual ~ETexture();

	// �ӱ����ļ��ж�ȡ��Դ
	void loadFromFile(
		const EString & fileName
	);

	// ��ȡ������Դ
	void loadFromResource(
		const EString & resourceName,
		const EString & resourceType
	);

	// ��ȡԴͼƬ���
	virtual float getSourceWidth() const;

	// ��ȡԴͼƬ�߶�
	virtual float getSourceHeight() const;

	// ��ȡԴͼƬ��С
	virtual ESize getSourceSize() const;

	// Ԥ������Դ
	static bool preload(
		const EString & fileName
	);

	// Ԥ������Դ
	static bool preload(
		const EString & resourceName,
		const EString & resourceType
	);

	// ��ջ���
	static void clearCache();

protected:
	ID2D1Bitmap * _getBitmap();

protected:
	ID2D1Bitmap * m_pBitmap;
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
		const EString & resourceName,
		const EString & resourceType
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
		const EString & resourceName,
		const EString & resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESpriteFrame();

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
		const EString & resourceName,
		const EString & resourceType
	);

	// ����ԴͼƬ�������鲢�ü�
	ESprite(
		const EString & resourceName,
		const EString & resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESprite();
	
	// ���þ�������
	void setTexture(
		ETexture * texture
	);

	// ���þ��������ü�
	void setTexture(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// �� ESpriteFrame ������Դ
	void loadFromSpriteFrame(
		ESpriteFrame * frame
	);

	// �ü�����
	void clipTexture(
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


class EFont :
	public EObject
{
	friend EText;

public:
	EFont();

	EFont(
		EString fontFamily,
		float fontSize = 22,
		EFontWeight fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EFont();

	// ��ȡ��ǰ�ֺ�
	float getFontSize() const;

	// ��ȡ��ǰ�����ϸֵ
	EFontWeight getFontWeight() const;

	// �Ƿ���б��
	bool isItalic() const;

	// ��������
	void setFamily(
		EString fontFamily
	);

	// �����ֺ�
	void setSize(
		float fontSize
	);

	// ���������ϸֵ
	void setWeight(
		EFontWeight fontWeight
	);

	// ��������б��
	void setItalic(
		bool value
	);

protected:
	// �������ָ�ʽ
	void _initTextFormat();

	// ��ȡ���ָ�ʽ
	IDWriteTextFormat * _getTextFormat();

protected:
	EString		m_sFontFamily;
	float		m_fFontSize;
	EFontWeight	m_FontWeight;
	bool		m_bItalic;
	bool		m_bRecreateNeeded;
	IDWriteTextFormat * m_pTextFormat;
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
		EColor color,
		EFont * font
	);

	EText(
		const EString & text,
		EColor color,
		EString fontFamily,
		float fontSize = 22,
		EFontWeight fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EText();

	// ��ȡ�ı�
	EString getText() const;

	// ��ȡ�ı����
	virtual float getWidth() const override;

	// ��ȡ�ı���ȣ����������ţ�
	virtual float getRealWidth() const override;

	// ��ȡ������ɫ
	EColor getColor() const;

	// ��ȡ����
	EFont * getFont() const;

	// �����ı�
	void setText(
		const EString & text
	);

	// ����������ɫ
	void setColor(
		EColor color
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
	EColor	m_Color;
	bool	m_bWordWrapping;
	float	m_fWordWrappingWidth;
	EFont * m_pFont;
};

}