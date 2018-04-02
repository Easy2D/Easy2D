#pragma once
#include "ebase.h"


namespace e2d
{


class ColliderManager;

// ��ײ��
class Collider :
	public Object
{
	friend ColliderManager;
	friend Node;

public:
	Collider();

	virtual ~Collider();

	// �ж�����ײ��Ľ�����ϵ
	virtual int getRelationWith(
		Collider * pCollider
	) const;

	// ��ȡ���ڵ�
	Node * getParentNode() const;

	// ���û�رո���ײ��
	virtual void setEnable(
		bool bEnable
	);

	// ������ײ��Ŀɼ���
	void setVisiable(
		bool bVisiable
	);

	// ���û�����ɫ
	void setColor(
		UINT32 color
	);

	// ���û���͸����
	void setOpacity(
		double opacity
	);

	// ���ô�С����
	void setAutoResize(
		bool bEnable
	);

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1Geometry * getD2dGeometry() const = 0;

protected:
	// ת����ײ��
	virtual void _transform();

	// �����С
	virtual void _resize() = 0;

	// ��Ⱦ��ײ��
	virtual void _render();

protected:
	bool	m_bEnable;
	bool	m_bIsVisiable;
	bool	m_bAutoResize;
	UINT32	m_nColor;
	float	m_fOpacity;
	Node *	m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedGeometry;
};


// ����
class ColliderRect :
	public Collider
{
public:
	// ����һ��Ĭ�Ͼ���
	ColliderRect();

	// �������Ͻ�����Ϳ�ߴ�������
	ColliderRect(
		double x,
		double y,
		double width,
		double height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ľ���
	ColliderRect(
		Node * node
	);

	virtual ~ColliderRect();

	// �޸ľ��δ�С
	void setRect(
		double left,
		double top,
		double right,
		double bottom
	);

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1RectangleGeometry * getD2dGeometry() const override;

protected:
	// �����С
	virtual void _resize();

protected:
	ID2D1RectangleGeometry * m_pD2dRectangle;
};


// Բ��
class ColliderCircle :
	public Collider
{
public:
	// ����һ��Ĭ��Բ��
	ColliderCircle();

	// ����Բ�ĺͰ뾶����Բ��
	ColliderCircle(
		Point center,
		double radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ��Բ��
	ColliderCircle(
		Node * node
	);

	virtual ~ColliderCircle();

	// �޸�Բ�δ�С
	void setCircle(
		Point center,
		double radius
	);

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1EllipseGeometry * getD2dGeometry() const override;

protected:
	// �����С
	virtual void _resize();

protected:
	ID2D1EllipseGeometry * m_pD2dCircle;
};


// ��Բ��
class ColliderEllipse :
	public Collider
{
public:
	// ����һ��Ĭ����Բ
	ColliderEllipse();

	// ����Բ�ĺͰ뾶������Բ
	ColliderEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ����Բ
	ColliderEllipse(
		Node * node
	);

	virtual ~ColliderEllipse();

	// �޸���Բ��С
	void setEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1EllipseGeometry * getD2dGeometry() const override;

protected:
	// �����С
	virtual void _resize();

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}