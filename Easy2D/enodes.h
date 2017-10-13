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
		EPoint p
	);

	ENode(
		int x, 
		int y
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
	virtual e2d::ESize getSize() const;

	// ��ȡ�ڵ����ڵľ���
	virtual e2d::ERect getRect() const;

	// ��ȡ���ڵ�
	virtual e2d::ENode* &getParent();

	// ��ȡ�ڵ����ڳ���
	EScene * &getParentScene();

	// ���ýڵ��Ƿ���ʾ
	virtual void setVisiable(
		bool value
	);

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
		e2d::ESize size
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
		e2d::ERect rect
	);

	// ���ýڵ��ͼ˳��0Ϊ���Ȼ��ƣ���ʾ����ײ㣩
	virtual void setZOrder(
		int z
	);

	// ���ø��ڵ�
	virtual void setParent(
		ENode* parent
	);

	// ���ýڵ����ڳ���
	void setParentScene(
		EScene * scene
	);

protected:
	int			m_nZOrder;
	bool		m_bVisiable;
	ERect		m_Rect;
	EScene *	m_pParentScene;
	ENode *		m_pParent;

protected:

	virtual bool _exec(bool active);

	virtual void _onRender();
};

}