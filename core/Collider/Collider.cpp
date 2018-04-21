#include "..\e2dcollider.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider()
	: m_bIsVisiable(true)
	, m_nColor(Color::RED)
	, m_fOpacity(1)
	, m_pParentNode(nullptr)
	, m_pTransformedGeometry(nullptr)
	, m_bEnable(true)
	, m_bAutoResize(true)
{
}

e2d::Collider::~Collider()
{
	SafeReleaseInterface(&m_pTransformedGeometry);
}

e2d::Node * e2d::Collider::getParentNode() const
{
	return m_pParentNode;
}

void e2d::Collider::setEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void e2d::Collider::setVisiable(bool bVisiable)
{
	m_bIsVisiable = bVisiable;
}

void e2d::Collider::setColor(UINT32 color)
{
	m_nColor = color;
}

void e2d::Collider::setOpacity(double opacity)
{
	m_fOpacity = min(max(static_cast<float>(opacity), 0), 1);
}

void e2d::Collider::setAutoResize(bool bEnable)
{
	m_bAutoResize = bEnable;
}

void e2d::Collider::_render()
{
	if (m_pTransformedGeometry && m_bEnable)
	{
		// ��ȡ��ɫ��ˢ
		ID2D1SolidColorBrush * pBrush = Renderer::getSolidColorBrush();
		// ���û�ˢ��ɫ��͸����
		pBrush->SetColor(D2D1::ColorF(m_nColor, m_fOpacity));
		// ���Ƽ�����ײ��
		Renderer::getRenderTarget()->DrawGeometry(m_pTransformedGeometry, pBrush);
	}
}

int e2d::Collider::getRelationWith(Collider * pCollider) const
{
	if (m_pTransformedGeometry && pCollider->m_pTransformedGeometry)
	{
		if (m_bEnable && pCollider->m_bEnable)
		{
			D2D1_GEOMETRY_RELATION relation;

			m_pTransformedGeometry->CompareWithGeometry(
				pCollider->m_pTransformedGeometry,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return relation;
		}
	}
	return Relation::UNKNOWN;
}

void e2d::Collider::_transform()
{
	if (m_pParentNode && m_bEnable)
	{
		if (m_bAutoResize)
		{
			this->_resize();
		}

		// �ͷ�ԭ��ײ��
		SafeReleaseInterface(&m_pTransformedGeometry);

		// ���ݸ��ڵ�ת������ͼ��
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			getD2dGeometry(),
			m_pParentNode->m_MatriFinal,
			&m_pTransformedGeometry
		);

		ColliderManager::__updateCollider(this);
	}
}
