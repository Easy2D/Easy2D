#pragma once
#include "ebase.h"


namespace e2d
{


class CollisionManager;

// ��״
class ShapeBase :
	public Object
{
	friend CollisionManager;
	friend Node;

public:
	ShapeBase();

	virtual ~ShapeBase();

	// �ж�����״�Ľ�����ϵ
	virtual Relation getRelationWith(
		ShapeBase * pShape
	) const;

	// ��ȡ���ڵ�
	Node * getParentNode() const;

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
	UINT32	m_nColor;
	float	m_fOpacity;
	Node *	m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedShape;
};


// ����
class ShapeRectangle :
	public ShapeBase
{
public:
	// ����һ��Ĭ�Ͼ���
	ShapeRectangle();

	// �������Ͻ�����Ϳ�ߴ�������
	ShapeRectangle(
		double x,
		double y,
		double width,
		double height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ľ���
	ShapeRectangle(
		Node * node
	);

	virtual ~ShapeRectangle();

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
class ShapeCircle :
	public ShapeBase
{
public:
	// ����һ��Ĭ��Բ��
	ShapeCircle();

	// ����Բ�ĺͰ뾶����Բ��
	ShapeCircle(
		Point center,
		double radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ��Բ��
	ShapeCircle(
		Node * node
	);

	virtual ~ShapeCircle();

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
class ShapeEllipse :
	public ShapeBase
{
public:
	// ����һ��Ĭ����Բ
	ShapeEllipse();

	// ����Բ�ĺͰ뾶������Բ
	ShapeEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ����Բ
	ShapeEllipse(
		Node * node
	);

	virtual ~ShapeEllipse();

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