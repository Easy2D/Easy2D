#include <e2dbase.h>
#include <e2dnode.h>
#include <e2dmanager.h>

easy2d::Scene::Scene()
	: _autoUpdate(true)
	, _colliderVisiable(false)
	, _root(nullptr)
{
	_root = gcnew Node;
	if (_root)
	{
		_root->retain();
		_root->_setParentScene(this);
	}
}

easy2d::Scene::~Scene()
{
}

void easy2d::Scene::_render()
{
	_root->_render();

	if (_colliderVisiable)
	{
		// 恢复矩阵转换
		Renderer::getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		// 绘制所有几何图形
		_root->_drawCollider();
	}
}

void easy2d::Scene::_update()
{
	// 执行 onUpdate 函数
	if (_autoUpdate)
	{
		this->onUpdate();
	}
	// 更新根节点
	_root->_update();
}

void easy2d::Scene::setAutoUpdate(bool bAutoUpdate)
{
	_autoUpdate = bAutoUpdate;
}

void easy2d::Scene::add(Node * child, int order /* = 0 */)
{
	_root->addChild(child, order);
}

void easy2d::Scene::add(const std::vector<Node*>& nodes, int order)
{
	for (auto node : nodes)
	{
		this->add(node, order);
	}
}

bool easy2d::Scene::remove(Node * child)
{
	return _root->removeChild(child);
}

std::vector<easy2d::Node*> easy2d::Scene::get(const String& name) const
{
	return _root->getChildren(name);
}

easy2d::Node * easy2d::Scene::getOne(const String& name) const
{
	return _root->getChild(name);
}

const std::vector<easy2d::Node*>& easy2d::Scene::getAll() const
{
	return _root->getAllChildren();
}

easy2d::Node * easy2d::Scene::getRoot() const
{
	return _root;
}

void easy2d::Scene::showCollider(bool visiable)
{
	_colliderVisiable = visiable;
}

void easy2d::Scene::onDestroy()
{
	GC::release(_root);
}
