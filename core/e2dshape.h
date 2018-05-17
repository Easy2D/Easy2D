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
	enum class Style : int
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
class Rect :
	public Shape
{
public:
	Rect();

	Rect(
		Size size		/* ��Ⱥ͸߶� */
	);

	Rect(
		Point topLeft,	/* ���Ͻ����� */
		Size size		/* ��Ⱥ͸߶� */
	);

	static Rect * create();

	static Rect * create(
		Size size		/* ��Ⱥ͸߶� */
	);

	static Rect * create(
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
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	RoundRect(
		Point topLeft,	/* ���Ͻ����� */
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	static RoundRect * create();

	static RoundRect * create(
		Size size,		/* ��Ⱥ͸߶� */
		double radiusX,	/* Բ�ǰ뾶 */
		double radiusY	/* Բ�ǰ뾶 */
	);

	static RoundRect * create(
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
	float _radiusX;
	float _radiusY;
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

	static Circle * create();

	static Circle * create(
		double radius	/* �뾶 */
	);

	static Circle * create(
		Point center,	/* Բ������ */
		double radius	/* �뾶 */
	);

	virtual ~Circle();

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

	static Ellipse * create();

	static Ellipse * create(
		double radiusX,	/* ����뾶 */
		double radiusY	/* ����뾶 */
	);

	static Ellipse * create(
		Point center,	/* Բ������ */
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