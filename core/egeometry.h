#pragma once
#include "ebase.h"


namespace e2d
{

class EPhysicsManager;
class ENode;


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

	// ���ü�����״�Ŀɼ���
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
	// �ж�����״�Ľ�����ϵ
	virtual EPhysicsMsg::INTERSECT_RELATION _intersectWith(
		EGeometry * pGeometry
	);

	// ת����״
	virtual void _transform();

	// ��Ⱦ����ͼ��
	virtual void _render();

	virtual ID2D1Geometry * _getD2dGeometry() const = 0;

protected:
	bool	m_bIsVisiable;
	UINT32	m_nCategoryBitmask;
	UINT32	m_nCollisionBitmask;
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