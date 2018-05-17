#pragma once
#include "e2dbase.h"


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
	// ��ײ�����
	enum class Type : int
	{
		RECT,		/* ���� */
		CIRCLE,		/* Բ�� */
		ELLIPSE		/* ��Բ�� */
	};

	// ��ײ�彻����ϵ
	enum class Relation : int
	{
		UNKNOWN = 0,		/* ��ϵ��ȷ�� */
		DISJOINT = 1,		/* û�н��� */
		IS_CONTAINED = 2,	/* ��ȫ������ */
		CONTAINS = 3,		/* ��ȫ���� */
		OVERLAP = 4			/* �����ص� */
	};

public:
	Collider();

	virtual ~Collider();

	// �ж�����ײ��Ľ�����ϵ
	virtual Relation getRelationWith(
		Collider * pCollider
	) const;

	// ��ȡ���ڵ�
	Node * getParentNode() const;

	// ��ȡ������ɫ
	Color getColor() const;

	// ���û�رո���ײ��
	virtual void setEnable(
		bool enable
	);

	// ������ײ��Ŀɼ���
	void setVisiable(
		bool bVisiable
	);

	// ���û�����ɫ
	void setColor(
		Color color
	);

	// ���ô�С����
	void setAutoResize(
		bool enable
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
	bool	_enable;
	bool	_visiable;
	bool	_autoResize;
	Color	_color;
	Node *	_parentNode;
	ID2D1TransformedGeometry * _transformed;
};


// ������ײ��
class ColliderRect :
	public Collider
{
public:
	ColliderRect();

	ColliderRect(
		double x,
		double y,
		double width,
		double height
	);

	ColliderRect(
		Node * node
	);

	// ����һ��Ĭ�Ͼ�����ײ��
	static ColliderRect * create();

	// �������Ͻ�����Ϳ�ߴ���������ײ��
	static ColliderRect * create(
		double x,
		double y,
		double width,
		double height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ľ�����ײ��
	static ColliderRect * create(
		Node * node
	);

	virtual ~ColliderRect();

	// �޸ľ�����ײ���С
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
	ID2D1RectangleGeometry * _d2dRectangle;
};


// Բ����ײ��
class ColliderCircle :
	public Collider
{
public:
	ColliderCircle();

	ColliderCircle(
		Point center,
		double radius
	);

	ColliderCircle(
		Node * node
	);

	// ����һ��Ĭ��Բ����ײ��
	static ColliderCircle * create();

	// ����Բ�ĺͰ뾶����Բ����ײ��
	static ColliderCircle * create(
		Point center,
		double radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ��Բ����ײ��
	static ColliderCircle * create(
		Node * node
	);

	virtual ~ColliderCircle();

	// �޸�Բ����ײ���С
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
	ID2D1EllipseGeometry * _d2dCircle;
};


// ��Բ����ײ��
class ColliderEllipse :
	public Collider
{
public:
	ColliderEllipse();

	ColliderEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	ColliderEllipse(
		Node * node
	);

	// ����һ��Ĭ����Բ��ײ��
	static ColliderEllipse * create();

	// ����Բ�ĺͰ뾶������Բ��ײ��
	static ColliderEllipse * create(
		Point center,
		double radiusX,
		double radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ����Բ��ײ��
	static ColliderEllipse * create(
		Node * node
	);

	virtual ~ColliderEllipse();

	// �޸���Բ��ײ���С
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
	ID2D1EllipseGeometry * _d2dEllipse;
};

}