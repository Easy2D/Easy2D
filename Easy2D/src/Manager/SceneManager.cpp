#include <easy2d/e2dmanager.h>
#include <easy2d/e2dbase.h>
#include <easy2d/e2dnode.h>
#include <easy2d/e2dtransition.h>
#include <easy2d/e2dlistener.h>
#include <list>

static bool s_bSaveCurrScene = true;
static easy2d::Scene * s_pCurrScene = nullptr;
static easy2d::Scene * s_pNextScene = nullptr;
static easy2d::Transition * s_pTransition = nullptr;
static std::stack<easy2d::Scene*> s_SceneStack;
static std::list<easy2d::Node*> s_DispatcherList;

static void addDispatcher(easy2d::Node* node)
{
	s_DispatcherList.push_back(node);
	node->retain();
}

static void clearDispatcherList()
{
	for (auto node : s_DispatcherList)
	{
		node->release();
	}
	s_DispatcherList.clear();
}

void easy2d::SceneManager::enter(Scene * scene, Transition * transition /* = nullptr */, bool saveCurrentScene /* = true */)
{
	if (scene == nullptr)
	{
		return;
	}

	// ������һ������ָ��
	s_pNextScene = scene;
	s_pNextScene->retain();
	
	// �����л���������
	if (transition)
	{
		if (s_pTransition)
		{
			s_pTransition->_stop();
			s_pTransition->release();
		}
		s_pTransition = transition;
		transition->retain();
		transition->_init(s_pCurrScene, s_pNextScene);
		transition->_update();
	}

	if (s_pCurrScene)
	{
		s_bSaveCurrScene = saveCurrentScene;
	}
}

void easy2d::SceneManager::back(Transition * transition /* = nullptr */)
{
	// ջΪ��ʱ�����÷��س�������ʧ��
	if (s_SceneStack.size() == 0) E2D_WARNING("Scene stack is empty!");
	if (s_SceneStack.size() == 0) return;

	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	s_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();

	// ������һ����ʱ�������浱ǰ����
	if (s_pCurrScene)
	{
		s_bSaveCurrScene = false;
	}

	// �����л���������
	if (transition)
	{
		s_pTransition = transition;
		transition->retain();
		transition->_init(s_pCurrScene, s_pNextScene);
		transition->_update();
	}
}

void easy2d::SceneManager::clear()
{
	clearDispatcherList();
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		GC::release(temp);
		s_SceneStack.pop();
	}
}

easy2d::Scene * easy2d::SceneManager::getCurrentScene()
{
	return s_pCurrScene;
}

std::stack<easy2d::Scene*> easy2d::SceneManager::getSceneStack()
{
	return s_SceneStack;
}

bool easy2d::SceneManager::isTransitioning()
{
	return s_pTransition != nullptr;
}

void easy2d::SceneManager::dispatch(Event* evt)
{
	for (const auto& node : s_DispatcherList)
	{
		node->__dispatchEvent(evt);
	}
}

void easy2d::SceneManager::__update()
{
	clearDispatcherList();

	if (s_pTransition == nullptr)
	{
		// ���³�������
		if (s_pCurrScene)
		{
			s_pCurrScene->_update();
		}
	}
	else
	{
		// ���³�������
		s_pTransition->_update();

		if (s_pTransition->isDone())
		{
			s_pTransition->release();
			s_pTransition = nullptr;
		}
		else
		{
			return;
		}
	}

	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (s_pNextScene)
	{
		// ִ�е�ǰ������ onExit ����
		s_pCurrScene->onExit();

		// ��Ҫ���浱ǰ��������������ջ��
		if (s_bSaveCurrScene)
		{
			s_SceneStack.push(s_pCurrScene);
		}
		else
		{
			GC::release(s_pCurrScene);
		}

		// ִ����һ������ onEnter ����
		s_pNextScene->onEnter();

		s_pCurrScene = s_pNextScene;		// �л�����
		s_pNextScene = nullptr;				// ��һ�����ÿ�
	}
}

void easy2d::SceneManager::__render(bool showBodyShape)
{
	if (s_pTransition)
	{
		s_pTransition->_render();
	}
	else
	{
		// ���Ƶ�ǰ����
		if (s_pCurrScene)
		{
			s_pCurrScene->_render();
			if (showBodyShape)
			{
				s_pCurrScene->_renderBodyShape();
			}
		}
	}
}

bool easy2d::SceneManager::__init()
{
	// ����Ϸ��ʼ��ʱ������Ϊ�գ�����ó���
	if (s_pNextScene)
	{
		s_pCurrScene = s_pNextScene;
		s_pCurrScene->onEnter();
		s_pNextScene = nullptr;
	}

	// ���³�������
	SceneManager::__update();

	return true;
}

void easy2d::SceneManager::__uninit()
{
	GC::release(s_pCurrScene);
	GC::release(s_pNextScene);
	GC::release(s_pTransition);
	SceneManager::clear();
}

void easy2d::SceneManager::__pushDispatcher(Node* node)
{
	addDispatcher(node);
}
