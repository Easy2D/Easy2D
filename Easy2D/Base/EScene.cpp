#include "..\ebase.h"
#include "..\enodes.h"
#include "..\emsg.h"


e2d::EScene::~EScene()
{
	clearAllChildren();
}

/*void e2d::EScene::_exec()
{
	// active ��־�����Ƿ�ȡ�ý���
	bool active = true;
	// ����ִ�У������ƵĽڵ㣨��λ�ڻ������Ϸ������ȱ�����
	for (int i = int(m_vChildren.size()) - 1; i >= 0; i--)
	{
		if (m_vChildren[i]->_exec(active))	// ִ�нڵ����
		{
			active = false;					// ���ӽڵ�ȡ�ý��㣬����־�� false
		}
	}
}*/

void e2d::EScene::_onRender()
{
	// �������нڵ�
	for (auto child : m_vChildren)
	{
		child->_onRender();
	}
}

void e2d::EScene::onEnter()
{
}

void e2d::EScene::onExit()
{
}

void e2d::EScene::add(ENode * child, int zOrder /* = 0 */)
{
	// ������ӵĽڵ�ǿ�
	ASSERT(child != nullptr, "Scene::add NULL pointer exception.");
	// ���Կ�ָ��
	if (child == nullptr) return;
	// ���ýڵ�ĸ�����
	child->bindWithScene(this);
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

bool e2d::EScene::del(ENode * child, bool autoRelease /* = true */)
{
	if (child == nullptr) return false;

	// Ѱ���Ƿ�����ͬ�ڵ�
	std::vector<ENode*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// �ҵ���ͬ�ڵ�
		if (*iter == child)
		{
			if (autoRelease) (*iter)->autoRelease();
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

std::vector<e2d::ENode*>& e2d::EScene::getChildren()
{
	return m_vChildren;
}

void e2d::EScene::clearAllChildren()
{
	// ���нڵ�����ü�����һ
	for (auto child : m_vChildren)
	{
		child->autoRelease();
		child->release();
	}
	// ��մ���ڵ������
	m_vChildren.clear();
}

void e2d::EScene::bindListener(EMouseListener * listener)
{
	EMsgManager::bindListenerWith(listener, this);
}

void e2d::EScene::bindListener(EKeyboardListener * listener)
{
	EMsgManager::bindListenerWith(listener, this);
}
