#pragma once
#include "ebase.h"
#include "Win\winbase.h"

namespace e2d 
{

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

	// ���ýڵ���
	virtual void setWidth(
		float width
	);

	// ���ýڵ�߶�
	virtual void setHeight(
		float height
	);

	// ���ýڵ��С
	virtual void setSize(
		const ESize & size
	);

	// ���ýڵ��С
	virtual void setSize(
		float width,
		float height
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

	// ���ø��ڵ�
	virtual void setParent(
		ENode* parent
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
		bool release = false
	);

	// �Ƴ��ӽڵ�
	virtual void removeChild(
		const EString & childName,
		bool release = false
	);

	// �Ƴ����нڵ�
	virtual void clearAllChildren(
		bool release = false
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

	// �ڵ����ʱ��ִ�г���
	virtual void _onClear();

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
	EVector<ENode*>	m_vChildren;
};


class ERectangle :
	public ENode
{
public:
	ERectangle();

	EColor getColor() const;

	void setColor(EColor color);

protected:
	virtual void _onRender() override;

protected:
	EColor m_Color;
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

	// ����ԴͼƬ��������
	ESprite(
		const EString & resourceName,
		const EString & resourceType
	);
	
	// ���ļ�����ͼƬ
	void setImage(
		const EString & fileName
	);

	// ����Դ����ͼƬ
	void setImage(
		const EString & resourceName,
		const EString & resourceType
	);

	// Ԥ������Դ
	static bool preloadImage(
		const EString & fileName
	);

	// Ԥ������Դ
	static bool preloadImage(
		const EString & resourceName,
		const EString & resourceType
	);

	// ���ͼƬ����
	static void clearCache();

protected:
	// ��Ⱦ����
	virtual void _onRender() override;

protected:
	EString m_sFileName;
	EString m_sResourceName;
	EString m_sResourceType;
	ID2D1Bitmap * pBitmap;
};

}