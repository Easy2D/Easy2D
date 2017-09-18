#include "easy2d.h"
#include <assert.h>

Scene::Scene()
{
}

Scene::~Scene()
{
	clearAllChildren();
}

void Scene::_exec()
{
	// active ��־�����Ƿ�ȡ�ý���
	bool active = true;
	// ����ִ�У������ƵĽڵ㣨��λ�ڻ������Ϸ������ȱ�����
	for (int i = int(m_vChildren.size()) - 1; i >= 0; i--)
	{
		assert(m_vChildren[i]);
		if (m_vChildren[i]->_exec(active))	// ִ�нڵ����
		{
			active = false;					// ���ӽڵ�ȡ�ý��㣬����־�� false
		}
	}
}

void Scene::_onDraw()
{
	// �������нڵ�
	for (auto child : m_vChildren)
	{
		assert(child);
		child->_onDraw();
	}
}

void Scene::onEnter()
{
}

void Scene::onExit()
{
}

void Scene::add(Node * child, int zOrder)
{
	// ������ӵĽڵ�ǿ�
	assert(child);
	// ���ýڵ�ĸ�����
	child->setParentScene(this);
	// ���� z ��˳��
	child->setZOrder(zOrder);
	// ��������ü�����һ
	child->retain();

	// �� z ��˳�����ڵ�
	size_t size = m_vChildren.size();
	for (unsigned i = 0; i <= size; i++)
	{
		if (i == size)
		{
			m_vChildren.push_back(child);
			break;
		}
		else
		{
			auto temp = m_vChildren.at(i);
			if (temp->getZOrder() > zOrder)
			{
				m_vChildren.insert(m_vChildren.begin() + i, child);
				break;
			}
		}
	}
}

bool Scene::del(Node * child)
{
	// ���Խڵ�ǿ�
	assert(child);

	// Ѱ���Ƿ�����ͬ�ڵ�
	std::vector<Node*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// �ҵ���ͬ�ڵ�
		if (*iter == child)
		{
			// ��������ü�����һ
			(*iter)->release();
			// ȥ���ýڵ�
			m_vChildren.erase(iter);
			return true;
		}
	}
	// δ�ҵ��ýڵ㷵�� false
	return false;
}

void Scene::clearAllChildren()
{
	// ���нڵ�����ü�����һ
	for (auto child : m_vChildren)
	{
		child->release();
	}
	// ��մ���ڵ������
	m_vChildren.clear();
}