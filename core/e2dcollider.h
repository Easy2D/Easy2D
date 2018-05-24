#pragma once
#include "e2dbase.h"


namespace e2d
{



class Listener;
class ColliderManager;

// ��ײ�¼�
class Collision
{
	friend class Game;
	friend class ColliderManager;

public:
	// ��ӿɻ�����ײ���������
	static void addName(
		const String& name1,
		const String& name2
	);

	// ��ӿɻ�����ײ���������
	static void addName(
		const std::vector<std::pair<String, String> >& names
	);

	// �ж����������Ƿ��ǿ���ײ��
	static bool isCollidable(
		Node * node1,
		Node * node2
	);

	// �ж����������Ƿ��ǿ���ײ��
	static bool isCollidable(
		const String& name1,
		const String& name2
	);

	// ��ȡ��ײ����ʱ��������
	static Node * getActiveNode();

	// ��ȡ��ײ����ʱ�ı�����
	static Node * getPassiveNode();

	// �ж���ײ�Ƿ��ɸýڵ�����
	// ����ǣ�����������ײ�Ľڵ�ָ�룬���򷵻ؿ�
	static Node * isCausedBy(
		Node * node
	);

	// �жϷ�����ײ�Ľڵ������Ƿ���ͬ
	// ����ͬ��������ָ�룬���򷵻ؿ�
	static Node * isCausedBy(
		const String& name
	);

	// ������ر�������ײ�������ܣ�Ĭ�Ϲرգ�
	static void setEnable(
		bool enable
	);

	// �Ƿ����������ײ����
	static bool isEnable();

	// �����ײ����
	static Listener * addListener(
		const Function& func,		/* ��������ײʱ��ִ�к��� */
		const String& name = L"",	/* ���������� */
		bool paused = false			/* �Ƿ���ͣ */
	);

	// �����ײ����
	static void addListener(
		Listener * listener			/* ������ */
	);

	// �Ƴ�������
	static void removeListener(
		Listener * listener			/* ������ */
	);

	// ������ײ����
	static void startListener(
		const String& name
	);

	// ֹͣ��ײ����
	static void stopListener(
		const String& name
	);

	// �Ƴ���ײ����
	static void removeListener(
		const String& name
	);

	// �������м�����
	static void startAllListeners();

	// ֹͣ���м�����
	static void stopAllListeners();

	// �Ƴ����м�����
	static void removeAllListeners();

private:
	// ���¼�����
	static void __update(
		Node * active,
		Node * passive
	);

	// ��ռ�����
	static void __clearListeners();
};


// ��ײ��
class Collider :
	public Object
{
	friend class ColliderManager;
	friend class Node;

public:
	// ��ײ�����
	enum class Type
	{
		NONE,		/* �� */
		RECT,		/* ���� */
		CIRCLE,		/* Բ�� */
		ELLIPSE		/* ��Բ�� */
	};

	// ��ײ�彻����ϵ
	enum class Relation : int
	{
		UNKNOWN = 0,		/* ��ϵ��ȷ�� */
		DISJOIN = 1,		/* û�н��� */
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
class RectCollider :
	public Collider
{
public:
	RectCollider();

	explicit RectCollider(
		double x,
		double y,
		double width,
		double height
	);

	explicit RectCollider(
		Node * node
	);

	virtual ~RectCollider();

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
class CircleCollider :
	public Collider
{
public:
	CircleCollider();

	explicit CircleCollider(
		Point center,
		double radius
	);

	explicit CircleCollider(
		Node * node
	);

	virtual ~CircleCollider();

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
class EllipseCollider :
	public Collider
{
public:
	EllipseCollider();

	explicit EllipseCollider(
		Point center,
		double radiusX,
		double radiusY
	);

	explicit EllipseCollider(
		Node * node
	);

	virtual ~EllipseCollider();

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