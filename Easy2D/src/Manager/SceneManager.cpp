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

	// 保存下一场景的指针
	s_pNextScene = scene;
	s_pNextScene->retain();
	
	// 设置切换场景动作
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
	// 栈为空时，调用返回场景函数失败
	if (s_SceneStack.size() == 0) E2D_WARNING("Scene stack is empty!");
	if (s_SceneStack.size() == 0) return;

	// 从栈顶取出场景指针，作为下一场景
	s_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();

	// 返回上一场景时，不保存当前场景
	if (s_pCurrScene)
	{
		s_bSaveCurrScene = false;
	}

	// 设置切换场景动作
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
	// 清空场景栈
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
		// 更新场景内容
		if (s_pCurrScene)
		{
			s_pCurrScene->_update();
		}
	}
	else
	{
		// 更新场景动作
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

	// 下一场景指针不为空时，切换场景
	if (s_pNextScene)
	{
		// 执行当前场景的 onExit 函数
		s_pCurrScene->onExit();

		// 若要保存当前场景，把它放入栈中
		if (s_bSaveCurrScene)
		{
			s_SceneStack.push(s_pCurrScene);
		}
		else
		{
			GC::release(s_pCurrScene);
		}

		// 执行下一场景的 onEnter 函数
		s_pNextScene->onEnter();

		s_pCurrScene = s_pNextScene;		// 切换场景
		s_pNextScene = nullptr;				// 下一场景置空
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
		// 绘制当前场景
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
	// 若游戏初始化时场景不为空，进入该场景
	if (s_pNextScene)
	{
		s_pCurrScene = s_pNextScene;
		s_pCurrScene->onEnter();
		s_pNextScene = nullptr;
	}

	// 更新场景内容
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
