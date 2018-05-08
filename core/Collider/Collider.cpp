#include "..\e2dcollider.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider()
	: _bIsVisiable(true)
	, _nColor(Color::RED, 0.7f)
	, _pParentNode(nullptr)
	, _pTransformedGeometry(nullptr)
	, _bEnable(true)
	, _bAutoResize(true)
{
}

e2d::Collider::~Collider()
{
	SafeReleaseInterface(&_pTransformedGeometry);
}

e2d::Node * e2d::Collider::getParentNode() const
{
	return _pParentNode;
}

e2d::Color e2d::Collider::getColor() const
{
	return _nColor;
}

void e2d::Collider::setEnable(bool enable)
{
	_bEnable = enable;
}

void e2d::Collider::setVisiable(bool bVisiable)
{
	_bIsVisiable = bVisiable;
}

void e2d::Collider::setColor(Color color)
{
	_nColor = color;
}

void e2d::Collider::setAutoResize(bool enable)
{
	_bAutoResize = enable;
}

void e2d::Collider::_render()
{
	if (_pTransformedGeometry && _bEnable)
	{
		// ��ȡ��ɫ��ˢ
		ID2D1SolidColorBrush * pBrush = Renderer::getSolidColorBrush();
		// ���û�ˢ��ɫ��͸����
		pBrush->SetColor(_nColor.toColorF());
		// ���Ƽ�����ײ��
		Renderer::getRenderTarget()->DrawGeometry(_pTransformedGeometry, pBrush);
	}
}

e2d::Relation e2d::Collider::getRelationWith(Collider * pCollider) const
{
	if (_pTransformedGeometry && pCollider->_pTransformedGeometry)
	{
		if (_bEnable && pCollider->_bEnable)
		{
			D2D1_GEOMETRY_RELATION relation;

			_pTransformedGeometry->CompareWithGeometry(
				pCollider->_pTransformedGeometry,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return Relation(relation);
		}
	}
	return Relation::UNKNOWN;
}

void e2d::Collider::_transform()
{
	if (_pParentNode && _bEnable)
	{
		if (_bAutoResize)
		{
			this->_resize();
		}

		// �ͷ�ԭ��ײ��
		SafeReleaseInterface(&_pTransformedGeometry);

		// ���ݸ��ڵ�ת������ͼ��
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			getD2dGeometry(),
			_pParentNode->_MatriFinal,
			&_pTransformedGeometry
		);

		ColliderManager::__updateCollider(this);
	}
}
