#pragma once
#include "ebase.h"

namespace e2d 
{

class ENode :
	public EObject
{
	friend EScene;

public:
	ENode();

	ENode(
		EString name
	);

	virtual ~ENode();

	// �ڵ��Ƿ���ʾ
	virtual bool isVisiable() const;

	// ��ȡ�ڵ��ͼ˳��
	virtual int getZOrder() const;

	// ��ȡ�ڵ������
	virtual int getX() const;

	// ��ȡ�ڵ�������
	virtual int getY() const;

	// ��ȡ�ڵ�����
	virtual EPoint getPos() const;

	// ��ȡ�ڵ���
	virtual UINT32 getWidth() const;

	// ��ȡ�ڵ�߶�
	virtual UINT32 getHeight() const;

	// ��ȡ�ڵ��С
	virtual ESize getSize() const;

	// ��ȡ�ڵ����ڵľ���
	virtual ERect getRect() const;

	// ��ȡ���ڵ�
	virtual ENode * &getParent();

	// ��ȡ�ڵ����ڳ���
	virtual EScene * &getParentScene();

	// ��ȡ�����ӽڵ�
	virtual std::vector<ENode*> &getChildren();

	// ��ȡ�ӽڵ�����
	virtual int getChildrenCount() const;

	// �������ֻ�ȡ�ӽڵ�
	virtual ENode * getChild(EString name);

	// ���ýڵ��Ƿ���ʾ
	virtual void setVisiable(
		bool value
	);

	// ���ýڵ�����
	virtual void setName(EString name);

	// ���ýڵ������
	virtual void setX(
		int x
	);

	// ���ýڵ�������
	virtual void setY(
		int y
	);

	// ���ýڵ�����
	virtual void setPos(
		int x, 
		int y
	);

	// ���ýڵ�����
	virtual void setPos(
		EPoint p
	);

	// �ƶ��ڵ�
	virtual void move(
		int x, 
		int y
	);

	// �ƶ��ڵ�
	virtual void move(
		EVector v
	);

	// ���ýڵ���
	virtual void setWidth(
		UINT32 width
	);

	// ���ýڵ�߶�
	virtual void setHeight(
		UINT32 height
	);

	// ���ýڵ��С
	virtual void setSize(
		UINT32 width, 
		UINT32 height
	);

	// ���ýڵ��С
	virtual void setSize(
		ESize size
	);

	// ���ýڵ����ڵľ���
	virtual void setRect(
		int x1, 
		int y1, 
		int x2, 
		int y2
	);

	// ���ýڵ����ڵľ���
	virtual void setRect(
		EPoint leftTop, 
		EPoint rightBottom
	);

	// ���ýڵ����ڵľ���
	virtual void setRect(
		ERect rect
	);

	// ���ýڵ��ͼ˳��0Ϊ���Ȼ��ƣ���ʾ����ײ㣩
	virtual void setZOrder(
		int z
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
	virtual void addChild(ENode * child);

	// �Ӹ��ڵ��Ƴ�
	virtual void removeFromParent(bool release = false);

	// �Ƴ��ӽڵ�
	virtual void removeChild(ENode * child, bool release = false);

	// �Ƴ��ӽڵ�
	virtual void removeChild(EString childName, bool release = false);

protected:
	// ��Ⱦ�ڵ�
	virtual void _onRender();

protected:
	EString		m_sName;
	size_t		m_nHashName;
	int			m_nZOrder;
	bool		m_bVisiable;
	ERect		m_Rect;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	std::vector<ENode*> m_vChildren;
};

}