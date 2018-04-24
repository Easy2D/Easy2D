#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: m_bWillSave(true)
	, m_bAutoUpdate(true)
	, m_bSortNeeded(false)
	, m_bColliderVisiable(false)
	, m_pRoot(new Node())
{
	m_pRoot->retain();
	m_pRoot->_onEnter();
	m_pRoot->_setParentScene(this);
}

e2d::Scene::~Scene()
{
}

void e2d::Scene::_render()
{
	m_pRoot->_render();

	if (m_bColliderVisiable)
	{
		// �ָ�����ת��
		Renderer::getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		// �������м���ͼ��
		m_pRoot->_drawCollider();
	}
}

void e2d::Scene::_update()
{
	// ִ�� onUpdate ����
	if (m_bAutoUpdate)
	{
		this->onUpdate();
	}
	// ���¸��ڵ�
	m_pRoot->_update();
}

void e2d::Scene::setAutoUpdate(bool bAutoUpdate)
{
	m_bAutoUpdate = bAutoUpdate;
}

void e2d::Scene::add(Node * child, int order /* = 0 */)
{
	m_pRoot->addChild(child, order);
}

#ifdef HIGHER_THAN_VS2012
void e2d::Scene::add(const InitList<Node*>& vNodes, int order)
{
	for (const auto &node : vNodes)
	{
		this->add(node, order);
	}
}
#endif

bool e2d::Scene::remove(Node * child)
{
	return m_pRoot->removeChild(child);
}

std::vector<e2d::Node*> e2d::Scene::get(String name) const
{
	return m_pRoot->getChildren(name);
}

e2d::Node * e2d::Scene::getOne(String name) const
{
	return m_pRoot->getChild(name);
}

std::vector<e2d::Node*> e2d::Scene::getAll() const
{
	return m_pRoot->getAllChildren();
}

e2d::Node * e2d::Scene::getRoot() const
{
	return m_pRoot;
}

void e2d::Scene::showCollider(bool visiable)
{
	m_bColliderVisiable = visiable;
}

void e2d::Scene::destroy()
{
	SafeRelease(&m_pRoot);
}
