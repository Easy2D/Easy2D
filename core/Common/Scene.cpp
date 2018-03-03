#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emanagers.h"
#include "..\etools.h"
#include "..\eactions.h"

e2d::Scene::Scene()
	: m_bWillSave(true)
	, m_bAutoUpdate(true)
	, m_bSortNeeded(false)
	, m_bShapeVisiable(false)
	, m_pRoot(new Node())
{
	m_pRoot->retain();
	m_pRoot->_onEnter();
	m_pRoot->_setParentScene(this);
}

e2d::Scene::~Scene()
{
	SafeRelease(&m_pRoot);
}

void e2d::Scene::_render()
{
	m_pRoot->_render();

	if (m_bShapeVisiable)
	{
		// �ָ�����ת��
		Renderer::getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		// �������м���ͼ��
		m_pRoot->_drawShape();
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

bool e2d::Scene::remove(Node * child)
{
	return m_pRoot->removeChild(child);
}

e2d::Node * e2d::Scene::getRoot() const
{
	return m_pRoot;
}

void e2d::Scene::setShapeVisiable(bool visiable)
{
	m_bShapeVisiable = visiable;
}
