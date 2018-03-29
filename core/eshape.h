#pragma once
#include "ebase.h"


namespace e2d
{

class ShapeManager;
class Node;


class Shape :
	public Object
{
	friend ShapeManager;
	friend Node;

public:
	Shape();

	virtual ~Shape();

	// �ж�����״�Ľ�����ϵ
	virtual int getRelationWith(
		Shape * pShape
	) const;

	// ��ȡ���ڵ�
	Node * getParentNode() const;

	// ��ȡ�������
	UINT32 getCategoryBitmask() const;

	// ��ȡ��ͻ����
	UINT32 getCollisionBitmask() const;

	// �����������
	void setCategoryBitmask(
		UINT32 mask
	);

	// ���ó�ͻ����
	void setCollisionBitmask(
		UINT32 mask
	);

	// ���û�رո���״
	virtual void setEnable(
		bool bEnable
	);

	// ������״�Ŀɼ���
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
	// ת����״
	virtual void _transform();

	// �����С
	virtual void _resize() = 0;

	// ��Ⱦ��״
	virtual void _render();

protected:
	bool	m_bEnable;
	bool	m_bIsVisiable;
	bool	m_bAutoResize;
	UINT32	m_nCategoryBitmask;
	UINT32	m_nCollisionBitmask;
	UINT32	m_nColor;
	float	m_fOpacity;
	Node *	m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedShape;
};


class Rect :
	public Shape
{
public:
	// ����һ���վ���
	Rect();

	// �������Ͻ�����Ϳ�ߴ�������
	Rect(
		double x,
		double y,
		double width,
		double height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ľ���
	Rect(
		Node * node
	);

	virtual ~Rect();

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1RectangleGeometry * getD2dGeometry() const override;

protected:
	void _setRect(
		double left,
		double top,
		double right,
		double bottom
	);

	// �����С
	virtual void _resize();

protected:
	ID2D1RectangleGeometry * m_pD2dRectangle;
};


class Circle :
	public Shape
{
public:
	// ����һ���յ�Բ��
	Circle();

	// ����Բ�ĺͰ뾶����Բ��
	Circle(
		Point center,
		double radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ��Բ��
	Circle(
		Node * node
	);

	virtual ~Circle();

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1EllipseGeometry * getD2dGeometry() const override;

protected:
	void _setCircle(
		Point center,
		double radius
	);

	// �����С
	virtual void _resize();

protected:
	ID2D1EllipseGeometry * m_pD2dCircle;
};


class Ellipse :
	public Shape
{
public:
	// ����һ���յ���Բ
	Ellipse();

	// ����Բ�ĺͰ뾶������Բ
	Ellipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ����Բ
	Ellipse(
		Node * node
	);

	virtual ~Ellipse();

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1EllipseGeometry * getD2dGeometry() const override;

protected:
	void _setEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// �����С
	virtual void _resize();

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}