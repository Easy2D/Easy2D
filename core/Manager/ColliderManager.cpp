#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dcollider.h"
#include "..\e2dtool.h"

// ������
class Listener
{
public:
	Listener(
		e2d::Function func,
		e2d::String name,
		bool paused
	)
		: name(name)
		, callback(func)
		, running(!paused)
		, stopped(false)
	{
	}

	// ���¼�����״̬
	virtual void update()
	{
		if (callback)
		{
			callback();
		}
	}

public:
	bool running;
	bool stopped;
	e2d::String name;
	e2d::Function callback;
};


// ��ײ�弯��
static std::vector<e2d::Collider*> s_vColliders;
// ����������
static std::vector<Listener*> s_vListeners;
// ��ײ����״̬
static bool s_bCollisionEnable = false;
// ������ײ�Ľڵ�
static e2d::Node * s_pActiveNode = nullptr;
static e2d::Node * s_pPassiveNode = nullptr;


void e2d::ColliderManager::setEnable(bool bEnable)
{
	s_bCollisionEnable = bEnable;
}

void e2d::ColliderManager::__update()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto pListener = s_vListeners[i];
		// �����ֹͣ�ļ�����
		if (pListener->stopped)
		{
			delete pListener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ���¼�����
			pListener->update();
			++i;
		}
	}
}

void e2d::ColliderManager::__updateCollider(e2d::Collider * pActiveCollider)
{
	// �ж���ײ�����Ƿ��
	if (!s_bCollisionEnable)
		return;

	Node* pActiveNode = pActiveCollider->m_pParentNode;
	if (pActiveNode)
	{
		// ��ȡ�ڵ����ڳ���
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// �ж���������ײ��Ľ������
		for (size_t i = 0; i < s_vColliders.size(); i++)
		{
			auto pPassiveCollider = s_vColliders[i];
			// �ж�������ײ���Ƿ���ͬһ������
			if (pActiveCollider == pPassiveCollider)
				continue;

			// ��ȡ����ײ�ڵ�
			Node* pPassiveNode = pPassiveCollider->m_pParentNode;
			// �ж����ڵ��Ƿ���ͬһ������
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				// �ж��������Ƿ����໥��ͻ������
				auto IsCollideWith = [](Node * active, Node * passive) -> bool
				{
					unsigned int hash = passive->getHashName();
					FOR_LOOP(collider, active->m_vColliders)
						if (collider == hash)
							return true;
					return false;
				};

				if (IsCollideWith(pActiveNode, pPassiveNode))
				{
					// �ж�����ײ�彻�����
					int relation = pActiveCollider->getRelationWith(pPassiveCollider);
					// ���� UNKNOWN �� DISJOINT ���
					if (relation != Relation::UNKNOWN && relation != Relation::DISJOINT)
					{
						s_pActiveNode = pActiveNode;
						s_pPassiveNode = pPassiveNode;
						pActiveNode->onCollide(pPassiveNode);
						pPassiveNode->onCollide(pActiveNode);
						pCurrentScene->onCollide(pActiveNode, pPassiveNode);
						ColliderManager::__update();
					}
				}
			}
		}
	}
	s_pActiveNode = nullptr;
	s_pPassiveNode = nullptr;
}

void e2d::ColliderManager::add(Function func, String name, bool paused)
{
	auto listener = new Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::ColliderManager::pause(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->running = false;
		}
	}
}

void e2d::ColliderManager::resume(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->running = true;
		}
	}
}

void e2d::ColliderManager::stop(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->stopped = true;
		}
	}
}

void e2d::ColliderManager::pauseAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->running = false;
	}
}

void e2d::ColliderManager::resumeAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->running = true;
	}
}

void e2d::ColliderManager::stopAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->stopped = true;
	}
}

e2d::Node * e2d::ColliderManager::getActiveNode()
{
	return s_pActiveNode;
}

e2d::Node * e2d::ColliderManager::getPassiveNode()
{
	return s_pPassiveNode;
}

e2d::Node* e2d::ColliderManager::isCausedBy(Node * pNode)
{
	if (s_pActiveNode == pNode)
		return s_pPassiveNode;
	if (s_pPassiveNode == pNode)
		return s_pActiveNode;
	return nullptr;
}

e2d::Node* e2d::ColliderManager::isCausedBy(String name)
{
	if (s_pActiveNode->getName() == name)
		return s_pActiveNode;
	if (s_pPassiveNode->getName() == name)
		return s_pPassiveNode;
	return nullptr;
}

void e2d::ColliderManager::__addCollider(Collider * pCollider)
{
	if (pCollider)
	{
		if (pCollider->m_pParentNode)
		{
			WARN_IF(true, "ColliderManager::__add Failed! The shape is already added.");
			return;
		}
		pCollider->retain();
		s_vColliders.push_back(pCollider);
	}
}

void e2d::ColliderManager::__removeCollider(Collider * pCollider)
{
	if (pCollider)
	{
		for (size_t i = 0; i < s_vColliders.size(); i++)
		{
			if (s_vColliders[i] == pCollider)
			{
				SafeRelease(&pCollider);
				s_vColliders.erase(s_vColliders.begin() + i);
				return;
			}
		}
	}
}

void e2d::ColliderManager::__uninit()
{
	FOR_LOOP(listener, s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}
