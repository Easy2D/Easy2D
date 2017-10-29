#pragma once
#include "ebase.h"


namespace e2d
{

class EPhysicsManager;
class ENode;
class EGeometry;

class EPhysicsMsg
{
	friend EPhysicsManager;

public:
	enum INTERSECT_RELATION
	{
		UNKNOWN = 0,		/* ��ϵ��ȷ�� */
		DISJOINT = 1,		/* û�н��� */
		IS_CONTAINED = 2,	/* ��ȫ������ */
		CONTAINS = 3,		/* ��ȫ���� */
		OVERLAP = 4			/* �����ص� */
	};

	// ��ȡ��ǰ������ײ��Ϣ����
	static INTERSECT_RELATION getMsg();

	// ��ȡ������
	static EGeometry * getActiveGeometry();

	// ��ȡ������
	static EGeometry * getPassiveGeometry();

public:
	static INTERSECT_RELATION s_nRelation;
	static EGeometry * s_pActiveGeometry;
	static EGeometry * s_pPassiveGeometry;
};

class EGeometry :
	public EObject
{
	friend EPhysicsManager;
	friend ENode;

public:
	EGeometry();

	virtual ~EGeometry();

	// ��ȡ���ڵ�
	ENode * getParentNode() const;

	// ���û�����ɫ
	void setColor(
		UINT32 color
	);

	// ���û���͸����
	void setOpacity(
		float opacity
	);

protected:
	// �ж�����״�Ľ�����ϵ
	virtual EPhysicsMsg::INTERSECT_RELATION _intersectWith(
		EGeometry * pGeometry
	);

	// ת����״
	virtual void _transform();

	// ��Ⱦ����ͼ��
	virtual void _onRender();

	virtual ID2D1Geometry * _getD2dGeometry() const = 0;

protected:
	bool	m_bTransformed;
	UINT32	m_nColor;
	float	m_fOpacity;
	ENode * m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedGeometry;
};


class ERectangle :
	public EGeometry
{
public:
	// ����һ���ռ��ξ���
	ERectangle();

	// �������Ͻ�����Ϳ�ߴ������ξ���
	ERectangle(
		float x,
		float y,
		float width,
		float height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ļ��ξ���
	ERectangle(
		ENode * node
	);

	virtual ~ERectangle();

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


class ECircle :
	public EGeometry
{
public:
	// ����һ���յļ���Բ��
	ECircle();

	// ����Բ�ĺͰ뾶��������Բ��
	ECircle(
		EPoint center,
		float radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ļ���Բ��
	ECircle(
		ENode * node
	);

	virtual ~ECircle();

protected:
	void _setCircle(
		EPoint center,
		float radius
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dCircle;
};


class EEllipse :
	public EGeometry
{
public:
	// ����һ���յļ�����Բ
	EEllipse();

	// ����Բ�ĺͰ뾶����������Բ
	EEllipse(
		EPoint center,
		float radiusX,
		float radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ļ�����Բ
	EEllipse(
		ENode * node
	);

	virtual ~EEllipse();

protected:
	void _setEllipse(
		EPoint center,
		float radiusX,
		float radiusY
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}