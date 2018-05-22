#include "..\e2dcollider.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider()
	: _visiable(true)
	, _color(Color::RED, 0.7f)
	, _parentNode(nullptr)
	, _transformed(nullptr)
	, _enable(true)
	, _autoResize(false)
{
}

e2d::Collider::~Collider()
{
	SafeRelease(_transformed);
}

e2d::Node * e2d::Collider::getParentNode() const
{
	return _parentNode;
}

e2d::Color e2d::Collider::getColor() const
{
	return _color;
}

void e2d::Collider::setEnable(bool enable)
{
	_enable = enable;
}

void e2d::Collider::setVisiable(bool bVisiable)
{
	_visiable = bVisiable;
}

void e2d::Collider::setColor(Color color)
{
	_color = color;
}

void e2d::Collider::setAutoResize(bool enable)
{
	_autoResize = enable;
}

void e2d::Collider::_render()
{
	if (_transformed && _enable)
	{
		// ��ȡ��ɫ��ˢ
		ID2D1SolidColorBrush * pBrush = Renderer::getSolidColorBrush();
		// ���û�ˢ��ɫ��͸����
		pBrush->SetColor(_color.toD2DColorF());
		// ���Ƽ�����ײ��
		Renderer::getRenderTarget()->DrawGeometry(_transformed, pBrush);
	}
}

e2d::Collider::Relation e2d::Collider::getRelationWith(Collider * pCollider) const
{
	if (_transformed && pCollider->_transformed)
	{
		if (_enable && pCollider->_enable)
		{
			D2D1_GEOMETRY_RELATION relation;

			_transformed->CompareWithGeometry(
				pCollider->_transformed,
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
	if (_parentNode && _enable)
	{
		if (_autoResize)
		{
			this->_resize();
		}

		// �ͷ�ԭ��ײ��
		SafeRelease(_transformed);

		// ���ݸ��ڵ�ת������ͼ��
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			getD2dGeometry(),
			_parentNode->_finalMatri,
			&_transformed
		);

		ColliderManager::__updateCollider(this);
	}
}
