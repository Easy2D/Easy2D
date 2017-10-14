#pragma once
#include "ebase.h"
#include "Win\winbase.h"

namespace e2d 
{

class ENode :
	public EObject
{
public:
	ENode();

	ENode(
		EString name
	);

	virtual ~ENode();

	// �ڵ��Ƿ���ʾ
	virtual bool isVisiable() const;

	// ��ȡ�ڵ��ͼ˳��
	virtual int getOrder() const;

	// ��ȡ�ڵ������
	virtual float getX() const;

	// ��ȡ�ڵ�������
	virtual float getY() const;

	// ��ȡ�ڵ���
	virtual float getWidth() const;

	// ��ȡ�ڵ�߶�
	virtual float getHeight() const;

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
	virtual std::vector<ENode*> &getChildren();

	// ��ȡ�ӽڵ�����
	virtual size_t getChildrenCount() const;

	// �������ֻ�ȡ�ӽڵ�
	virtual ENode * getChild(
		EString name
	) const;

	// �������ֻ�ȡ�ӽڵ�
	static ENode * getChild(
		EString name,
		const std::vector<ENode*> &children
	);

	// ���ýڵ��Ƿ���ʾ
	virtual void setVisiable(
		bool value
	);

	// ���ýڵ�����
	virtual void setName(
		EString name
	);

	// ���ýڵ������
	virtual void setX(
		float x
	);

	// ���ýڵ�������
	virtual void setY(
		float y
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
		float width,
		float height
	);

	// ���ýڵ��ͼ˳��
	virtual void setOrder(
		int order
	);

	// ���ú�������
	virtual void setScaleX(
		float scaleX
	);

	// ������������
	virtual void setScaleY(
		float scaleY
	);

	// ��������
	virtual void setScale(
		float scaleX,
		float scaleY
	);

	// ��������
	virtual void setScale(
		float scale
	);

	// ���ú�����б�Ƕ�
	virtual void setSkewX(
		float angleX
	);

	// ����������б�Ƕ�
	virtual void setSkewY(
		float angleY
	);

	// ������б�Ƕ�
	virtual void setSkew(
		float angleX,
		float angleY
	);

	// ������ת�Ƕ�
	virtual void setRotation(
		float rotation
	);

	// ����͸����
	virtual void setOpacity(
		float opacity
	);

	// ���ýڵ����ڳ���
	virtual void setParentScene(
		EScene * scene
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
	virtual void removeChild(
		ENode * child, 
		bool release = false
	);

	// �Ƴ��ӽڵ�
	virtual void removeChild(
		EString childName, 
		bool release = false
	);

	// ���ʽڵ�
	virtual void callOn();

protected:
	// ��Ⱦ�ڵ�
	virtual void _onRender();

	// �ӽڵ�����
	void _sortChildren();

	// �ڵ�״̬ת��
	void _transfrom();

protected:
	EString		m_sName;
	size_t		m_nHashName;
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
	D2D1::Matrix3x2F m_Matri;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bSortNeeded;
	bool		m_bTransformNeeded;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	std::vector<ENode*> m_vChildren;
};


class ERectangle :
	public ENode
{
public:
	ERectangle();

	EColor::Enum getColor() const;

	void setColor(EColor::Enum color);

protected:
	virtual void _onRender() override;

protected:
	EColor::Enum m_Color;
};

}