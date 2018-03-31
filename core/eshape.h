#pragma once
#include "ebase.h"


namespace e2d
{


// ����
class ShapeRectangle :
	public Shape
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
	public Shape
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
	public Shape
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