#pragma once
#include "ebase.h"

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

	// ���ýڵ��ͼ˳��
	virtual void setOrder(
		int order
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

	// �ڵ�״̬ת��
	virtual void _onTransfrom();

	// �ӽڵ�����
	void _sortChildren();

	// �ڵ�״̬ת��
	void _transfrom();

protected:
	EString		m_sName;
	size_t		m_nHashName;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bSortNeeded;
	bool		m_bTransformNeeded;
	ERect		m_Rect;
	EPoint		m_Pos;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	std::vector<ENode*> m_vChildren;
};


class ERectangle :
	public ENode
{
protected:
	virtual void _onRender() override;

	virtual void _onTransfrom() override;
};

}