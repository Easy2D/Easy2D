#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: _bWillSave(true)
	, _bAutoUpdate(true)
	, _bSortNeeded(false)
	, _bColliderVisiable(false)
	, _pRoot(new Node())
{
	_pRoot->retain();
	_pRoot->_setParentScene(this);
}

e2d::Scene::~Scene()
{
}

void e2d::Scene::_render()
{
	_pRoot->_render();

	if (_bColliderVisiable)
	{
		// �ָ�����ת��
		Renderer::getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		// �������м���ͼ��
		_pRoot->_drawCollider();
	}
}

void e2d::Scene::_update()
{
	// ִ�� onUpdate ����
	if (_bAutoUpdate)
	{
		this->onUpdate();
	}
	// ���¸��ڵ�
	_pRoot->_update();
}

void e2d::Scene::setAutoUpdate(bool bAutoUpdate)
{
	_bAutoUpdate = bAutoUpdate;
}

void e2d::Scene::add(Node * child, int order /* = 0 */)
{
	_pRoot->addChild(child, order);
}

#ifdef HIGHER_THAN_VS2012
void e2d::Scene::add(const std::initializer_list<Node*>& vNodes, int order)
{
	for (const auto &node : vNodes)
	{
		this->add(node, order);
	}
}
#endif

bool e2d::Scene::remove(Node * child)
{
	return _pRoot->removeChild(child);
}

std::vector<e2d::Node*> e2d::Scene::get(const String& name) const
{
	return _pRoot->getChildren(name);
}

e2d::Node * e2d::Scene::getOne(const String& name) const
{
	return _pRoot->getChild(name);
}

std::vector<e2d::Node*> e2d::Scene::getAll() const
{
	return _pRoot->getAllChildren();
}

e2d::Node * e2d::Scene::getRoot() const
{
	return _pRoot;
}

void e2d::Scene::showCollider(bool visiable)
{
	_bColliderVisiable = visiable;
}

void e2d::Scene::destroy()
{
	SafeRelease(&_pRoot);
}
