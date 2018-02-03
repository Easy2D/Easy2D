#pragma once
#include "ebase.h"


namespace e2d
{

class EShapeManager;
class ENode;


class EShape :
	public EObject
{
	friend EShapeManager;
	friend ENode;

public:
	EShape();

	virtual ~EShape();

	// �ж�����״�Ľ�����ϵ
	virtual int getRelationWith(
		EShape * pShape
	) const;

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
	ENode * m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedShape;
};


class ERectangle :
	public EShape
{
public:
	// ����һ���վ���
	ERectangle();

	// �������Ͻ�����Ϳ�ߴ�������
	ERectangle(
		float x,
		float y,
		float width,
		float height
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ�ľ���
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
	public EShape
{
public:
	// ����һ���յ�Բ��
	ECircle();

	// ����Բ�ĺͰ뾶����Բ��
	ECircle(
		EPoint center,
		float radius
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ��Բ��
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
	public EShape
{
public:
	// ����һ���յ���Բ
	EEllipse();

	// ����Բ�ĺͰ뾶������Բ
	EEllipse(
		EPoint center,
		float radiusX,
		float radiusY
	);

	// ����һ���ͽڵ�λ�ô�С��ͬ����Բ
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