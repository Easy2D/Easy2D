#include "..\e2dmanager.h"
#include "..\e2dnode.h"
#include "..\e2dcollider.h"
#include "..\e2dtool.h"

// ������
class Listener
{
public:
	Listener(
		const e2d::Function& func,
		const e2d::String& name,
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


void e2d::ColliderManager::setEnable(bool enable)
{
	s_bCollisionEnable = enable;
}

void e2d::ColliderManager::__update()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); ++i)
	{
		auto listener = s_vListeners[i];
		// �����ֹͣ�ļ�����
		if (listener->stopped)
		{
			delete listener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ���¼�����
			listener->update();
			++i;
		}
	}
}

void e2d::ColliderManager::__updateCollider(e2d::Collider * pActiveCollider)
{
	// �ж���ײ�����Ƿ��
	if (!s_bCollisionEnable)
		return;

	Node* pActiveNode = pActiveCollider->_parentNode;
	if (pActiveNode)
	{
		// �ж��������Ƿ����໥��ͻ������
		auto IsCollideWith = [](Node * active, Node * passive) -> bool
		{
			unsigned int hash = passive->getHashName();
			for (auto collider : active->_colliders)
				if (collider == hash)
					return true;
			return false;
		};

		// ��ȡ�ڵ����ڳ���
		Scene* pCurrentScene = pActiveNode->getParentScene();

		// �ж���������ײ��Ľ������
		for (size_t i = 0; i < s_vColliders.size(); ++i)
		{
			auto pPassiveCollider = s_vColliders[i];
			// �ж�������ײ���Ƿ���ͬһ������
			if (pActiveCollider == pPassiveCollider)
				continue;

			// ��ȡ����ײ�ڵ�
			Node* pPassiveNode = pPassiveCollider->_parentNode;
			// �ж����ڵ��Ƿ���ͬһ������
			if (pPassiveNode &&
				pPassiveNode->getParentScene() == pCurrentScene)
			{
				if (IsCollideWith(pActiveNode, pPassiveNode))
				{
					// �ж�����ײ�彻�����
					Collider::Relation relation = pActiveCollider->getRelationWith(pPassiveCollider);
					// ���� UNKNOWN �� DISJOIN ���
					if (relation != Collider::Relation::UNKNOWN && relation != Collider::Relation::DISJOIN)
					{
						Collision::__activeNode = pActiveNode;
						Collision::__passiveNode = pPassiveNode;
						ColliderManager::__update();
					}
					Collision::__activeNode = nullptr;
					Collision::__passiveNode = nullptr;
				}
			}
		}
	}
}

void e2d::ColliderManager::add(const Function& func, const String& name, bool paused)
{
	auto listener = new (std::nothrow) Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::ColliderManager::pause(const String& name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->running = false;
		}
	}
}

void e2d::ColliderManager::resume(const String& name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->running = true;
		}
	}
}

void e2d::ColliderManager::stop(const String& name)
{
	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->stopped = true;
		}
	}
}

void e2d::ColliderManager::pauseAll()
{
	for (auto listener : s_vListeners)
	{
		listener->running = false;
	}
}

void e2d::ColliderManager::resumeAll()
{
	for (auto listener : s_vListeners)
	{
		listener->running = true;
	}
}

void e2d::ColliderManager::stopAll()
{
	for (auto listener : s_vListeners)
	{
		listener->stopped = true;
	}
}

void e2d::ColliderManager::__addCollider(Collider * pCollider)
{
	if (pCollider)
	{
		if (pCollider->_parentNode)
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
		for (size_t i = 0; i < s_vColliders.size(); ++i)
		{
			if (s_vColliders[i] == pCollider)
			{
				SafeRelease(pCollider);
				s_vColliders.erase(s_vColliders.begin() + i);
				return;
			}
		}
	}
}

void e2d::ColliderManager::__uninit()
{
	for (auto listener : s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}
