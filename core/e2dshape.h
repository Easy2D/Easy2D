#pragma once
#include "e2dnode.h"

namespace e2d
{


// ��״
class Shape :
	public Node
{
public:
	Shape();

	virtual ~Shape();

	// ��ȡ��ʽ
	ShapeStyle getStyle() const;

	// ��ȡ�����ɫ
	Color getFillColor() const;

	// ��ȡ������ɫ
	Color getLineColor() const;

	// ��ȡ�������
	double getStrokeWidth() const;

	// ���������ɫ
	void setFillColor(
		Color fillColor
	);

	// ����������ɫ
	void setLineColor(
		Color lineColor
	);

	// �����������
	void setStrokeWidth(
		double strokeWidth
	);

	// ������ʽ
	void setStyle(ShapeStyle style);

	// ��Ⱦ��״
	virtual void onRender() override;

protected:
	// ��Ⱦ����
	virtual void _renderLine() = 0;

	// ��Ⱦ���ɫ
	virtual void _renderFill() = 0;

protected:
	ShapeStyle	m_nStyle;
	float	m_fStrokeWidth;
	Color	m_nLineColor;
	Color	m_nFillColor;
};


// ����
class Rect :
	public Shape
{
public:
	Rect();

	Rect(
		double width,	/* ��� */
		double height	/* �߶� */
	);

	Rect(
		Size size		/* ��Ⱥ͸߶� */
	);

	Rect(
		double top,		/* ���ϽǺ����� */
		double left,	/* ���Ͻ������� */
		double width,	/* ��� */
		double height	/* �߶� */
	);

	Rect(
		Point topLeft,	/* ���Ͻ����� */
		Size size		/* ��Ⱥ͸߶� */
	);

	virtual ~Rect();

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;
};


// Բ�Ǿ���
class RoundRect :
	public Shape
{
public:
	RoundRect();

	RoundRect(
		double width,	/* ��� */
		double height,	/* �߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	RoundRect(
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	RoundRect(
		double top,		/* ���ϽǺ����� */
		double left,	/* ���Ͻ������� */
		double width,	/* ��� */
		double height,	/* �߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	RoundRect(
		Point topLeft,	/* ���Ͻ����� */
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	virtual ~RoundRect();

	// ��ȡԲ�ǰ뾶
	double getRadiusX() const;

	// ��ȡԲ�ǰ뾶
	double getRadiusY() const;

	// ����Բ�ǰ뾶
	virtual void setRadiusX(
		double radiusX
	);

	// ����Բ�ǰ뾶
	virtual void setRadiusY(
		double radiusY
	);

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float m_fRadiusX;
	float m_fRadiusY;
};


// Բ��
class Circle :
	public Shape
{
public:
	Circle();

	Circle(
		double radius	/* �뾶 */
	);

	Circle(
		Point center,	/* Բ������ */
		double radius	/* �뾶 */
	);

	Circle(
		double centerX,	/* Բ�ĺ����� */
		double centerY,	/* Բ�������� */
		double radius	/* �뾶 */
	);

	virtual ~Circle();

	// ��ȡ�뾶
	double getRadius() const;

	// ���ð뾶
	virtual void setRadius(
		double radius
	);

public:
	// ���õĺ���
	void setWidth() {}

	// ���õĺ���
	void setHeight() {}

	// ���õĺ���
	void setSize() {}

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float m_fRadius;
};


// ��Բ��
class Ellipse :
	public Shape
{
public:
	Ellipse();

	Ellipse(
		double radiusX,	/* ����뾶 */
		double radiusY	/* ����뾶 */
	);

	Ellipse(
		Point center,	/* Բ������ */
		double radiusX,	/* ����뾶 */
		double radiusY	/* ����뾶 */
	);

	Ellipse(
		double centerX,	/* Բ�ĺ����� */
		double centerY,	/* Բ�������� */
		double radiusX,	/* ����뾶 */
		double radiusY	/* ����뾶 */
	);

	virtual ~Ellipse();

	// ��ȡ����뾶
	double getRadiusX() const;

	// ��ȡ����뾶
	double getRadiusY() const;

	// ���ú���뾶
	virtual void setRadiusX(
		double radiusX
	);

	// ��������뾶
	virtual void setRadiusY(
		double radiusY
	);

public:
	// ���õĺ���
	void setWidth() {}

	// ���õĺ���
	void setHeight() {}

	// ���õĺ���
	void setSize() {}

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float m_fRadiusX;
	float m_fRadiusY;
};

}