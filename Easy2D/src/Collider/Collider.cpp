#include <e2dcollider.h>
#include <e2dmanager.h>
#include <e2dnode.h>

easy2d::Collider::Collider()
	: _visiable(true)
	, _color(Color::Red, 0.7)
	, _parentNode(nullptr)
	, _transformed(nullptr)
	, _enable(true)
	, _autoResize(false)
{
}

easy2d::Collider::~Collider()
{
	SafeRelease(_transformed);
}

easy2d::Node * easy2d::Collider::getParentNode() const
{
	return _parentNode;
}

easy2d::Color easy2d::Collider::getColor() const
{
	return _color;
}

void easy2d::Collider::setEnable(bool enable)
{
	_enable = enable;
}

void easy2d::Collider::setVisiable(bool bVisiable)
{
	_visiable = bVisiable;
}

void easy2d::Collider::setColor(Color color)
{
	_color = color;
}

void easy2d::Collider::setAutoResize(bool enable)
{
	_autoResize = enable;
}

void easy2d::Collider::_render()
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

easy2d::Collider::Relation easy2d::Collider::getRelationWith(Collider * pCollider) const
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
	return Relation::Unknown;
}

void easy2d::Collider::_transform()
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
