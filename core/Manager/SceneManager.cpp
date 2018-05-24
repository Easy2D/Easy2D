#include "..\e2dmanager.h"
#include "..\e2dbase.h"
#include "..\e2dtransition.h"

static bool s_bSaveCurrScene = true;
static e2d::Scene * s_pCurrScene = nullptr;
static e2d::Scene * s_pNextScene = nullptr;
static e2d::Transition * s_pTransition = nullptr;
static std::stack<e2d::Scene*> s_SceneStack;

void e2d::SceneManager::enter(Scene * scene, Transition * transition /* = nullptr */, bool saveCurrentScene /* = true */)
{
	if (scene == nullptr)
	{
		throw Exception(L"������ָ���쳣");
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

void e2d::SceneManager::back(Transition * transition /* = nullptr */)
{
	// ջΪ��ʱ�����÷��س�������ʧ��
	WARN_IF(s_SceneStack.size() == 0, "Scene stack is empty!");
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

void e2d::SceneManager::clear()
{
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		GC::release(temp);
		s_SceneStack.pop();
	}
}

e2d::Scene * e2d::SceneManager::getCurrentScene()
{
	return s_pCurrScene;
}

std::stack<e2d::Scene*> e2d::SceneManager::getSceneStack()
{
	return s_SceneStack;
}

bool e2d::SceneManager::isTransitioning()
{
	return s_pTransition != nullptr;
}

void e2d::SceneManager::__update()
{
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

void e2d::SceneManager::__render()
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
		}
	}
}

bool e2d::SceneManager::__init()
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

void e2d::SceneManager::__uninit()
{
	GC::release(s_pCurrScene);
	GC::release(s_pNextScene);
	GC::release(s_pTransition);
	SceneManager::clear();
}
