#pragma once
#include "ebase.h"


namespace e2d
{

class ShapeManager;
class Node;


class Shape :
	public Obj
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
		float opacity
	);

protected:
	// ת����״
	virtual void _transform();

	// ��Ⱦ��״
	virtual void _render();

	// ��ȡ ID2D1Geometry ����
	virtual ID2D1Geometry * _getD2dGeometry() const = 0;

protected:
	bool	m_bEnable;
	bool	m_bIsVisiable;
	UINT32	m_nCategoryBitmask;
	UINT32	m_nCollisionBitmask;
	UINT32	m_nColor;
	float	m_fOpacity;
	Node * m_pParentNode;
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
		float x,
		float y,
		float width,
		float height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ľ���
	Rect(
		Node * node
	);

	virtual ~Rect();

protected:
	void _setRect(
		float left,
		float top,
		float right,
		float bottom
	);

	virtual ID2D1RectangleGeometry * _getD2dGeometry() const override;

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
		float radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ��Բ��
	Circle(
		Node * node
	);

	virtual ~Circle();

protected:
	void _setCircle(
		Point center,
		float radius
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

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
		float radiusX,
		float radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ����Բ
	Ellipse(
		Node * node
	);

	virtual ~Ellipse();

protected:
	void _setEllipse(
		Point center,
		float radiusX,
		float radiusY
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}