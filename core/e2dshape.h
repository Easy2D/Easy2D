#pragma once
#include "e2dnode.h"

namespace e2d
{


// ��״
class Shape :
	public Node
{
public:
	// ��״��ʽ
	enum Style : int
	{
		SOLID,		/* ��� */
		ROUND,		/* ���� */
		FILL,		/* ���� + ��� */
	};

public:
	Shape();

	virtual ~Shape();

	// ��ȡ��ʽ
	Style getStyle() const;

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
	void setStyle(Style style);

	// ��Ⱦ��״
	virtual void onRender() override;

protected:
	// ��Ⱦ����
	virtual void _renderLine() = 0;

	// ��Ⱦ���ɫ
	virtual void _renderFill() = 0;

protected:
	Style	_style;
	float	_strokeWidth;
	Color	_lineColor;
	Color	_fillColor;
};


// ����
class RectShape :
	public Shape
{
public:
	RectShape();

	RectShape(
		Size size		/* ��Ⱥ͸߶� */
	);

	RectShape(
		Point topLeft,	/* ���Ͻ����� */
		Size size		/* ��Ⱥ͸߶� */
	);

	virtual ~RectShape();

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;
};


// Բ�Ǿ���
class RoundRectShape :
	public Shape
{
public:
	RoundRectShape();

	RoundRectShape(
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	RoundRectShape(
		Point topLeft,	/* ���Ͻ����� */
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	virtual ~RoundRectShape();

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
	float _radiusX;
	float _radiusY;
};


// Բ��
class CircleShape :
	public Shape
{
public:
	CircleShape();

	CircleShape(
		double radius	/* �뾶 */
	);

	CircleShape(
		Point center,	/* Բ������ */
		double radius	/* �뾶 */
	);

	virtual ~CircleShape();

	// ��ȡ�뾶
	double getRadius() const;

	// ���ð뾶
	virtual void setRadius(
		double radius
	);

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float _radius;
};


// ��Բ��
class EllipseShape :
	public Shape
{
public:
	EllipseShape();

	EllipseShape(
		double radiusX,	/* ����뾶 */
		double radiusY	/* ����뾶 */
	);

	EllipseShape(
		Point center,	/* Բ������ */
		double radiusX,	/* ����뾶 */
		double radiusY	/* ����뾶 */
	);

	virtual ~EllipseShape();

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

protected:
	// ��Ⱦ����
	virtual void _renderLine() override;

	// ��Ⱦ���ɫ
	virtual void _renderFill() override;

protected:
	float _radiusX;
	float _radiusY;
};

}