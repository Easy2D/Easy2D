#include "..\easy2d.h"
#include "..\Win\winbase.h"
#include "..\EasyX\easyx.h"
#include <time.h>
#include <assert.h>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#include <stack>
#include <chrono>
#include <thread>

using namespace std::chrono;

// App ��Ψһʵ��
static App * s_pInstance = nullptr;
// ����ջ
static std::stack<Scene*> s_SceneStack;

App::App() : 
	m_pCurrentScene(nullptr), 
	m_pNextScene(nullptr), 
	m_pLoadingScene(nullptr),
	m_bRunning(false), 
	m_nWindowMode(0),
	m_bSaveScene(true)
{
	assert(!s_pInstance);	// ����ͬʱ�������� App ʵ��
	s_pInstance = this;		// ����ʵ������
}

App::~App()
{
}

App * App::get()
{
	assert(s_pInstance);	// ����ʵ������
	return s_pInstance;		// ��ȡ App ��Ψһʵ��
}

int App::run()
{
	// ����������ͼ
	BeginBatchDraw();
	// ��¼��ǰʱ��
	steady_clock::time_point nLast = steady_clock::now();
	// ֡���
	LONGLONG nAnimationInterval = 17LL;
	// ʱ����
	LONGLONG nInterval = 0LL;
	// ����ʱ��
	LONGLONG nWaitMS = 0L;

	// �����صĴ�����ʾ
	ShowWindow(GetHWnd(), SW_NORMAL);
	// ������Ϸ
	m_bRunning = true;

	// �������߳�
	//std::thread t(std::bind(&App::_mainLoop, this));
	//t.join();

	// ������ѭ��
	while (m_bRunning)
	{
		// ˢ�¼�ʱ
		::FlushSteadyClock();
		// ����ʱ����
		nInterval = duration_cast<milliseconds>(GetNow() - nLast).count();
		// �жϼ��ʱ���Ƿ��㹻
		if (nInterval >= nAnimationInterval)
		{
			// ��¼��ǰʱ��
			nLast = GetNow();
			// ˢ����Ϸ����
			_draw();
		}
		else
		{
			// �������ʱ��
			/*nWaitMS = nAnimationInterval * 2 - nInterval;
			// �����̣߳��ͷ� CPU ռ��
			if (nWaitMS > 1LL)
			{
				std::this_thread::sleep_for(milliseconds(nWaitMS));
			}*/
		}
	}
	// ֹͣ������ͼ
	EndBatchDraw();
	// �رմ���
	close();
	// �ͷ������ڴ�ռ��
	free();

	return 0;
}

void App::_initGraph()
{
	// ������ͼ����
	initgraph(m_Size.cx, m_Size.cy, m_nWindowMode);
	// ���ص�ǰ���ڣ���ֹ�ڼ��ؽ׶���ʾ�ڴ��ڣ�
	ShowWindow(GetHWnd(), SW_HIDE);
	// ��ȡ��Ļ�ֱ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// ��ȡ���ڴ�С
	CRect rcWindow;
	GetWindowRect(GetHWnd(), &rcWindow);
	// ���ô�������Ļ����
	SetWindowPos(GetHWnd(), HWND_TOP,
		(screenWidth - rcWindow.Size().cx) / 2,
		(screenHeight - rcWindow.Size().cy) / 2,
		rcWindow.Size().cx,
		rcWindow.Size().cy,
		SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	// �������뷨
	ImmAssociateContext(GetHWnd(), NULL);
	// ���û�ͼ����
	reset();
	// ���ô��ڱ���
	if (m_sTitle.empty())
	{
		// ���浱ǰ����
		TCHAR title[31];
		GetWindowText(GetHWnd(), title, 30);
		m_sTitle = title;
		if (m_sAppName.empty()) m_sAppName = title;
	}
	else
	{
		setWindowTitle(m_sTitle);
	}
}

void App::_draw()
{
	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (m_pNextScene)
	{
		// ������һ����
		_enterNextScene();
	}
	// ���Ե�ǰ�����ǿ�
	assert(m_pCurrentScene);

	cleardevice();				// ��ջ���
	m_pCurrentScene->_onDraw();	// ���Ƶ�ǰ����
	FlushBatchDraw();			// ˢ�»���

	MouseMsg::__exec();			// �����
	KeyMsg::__exec();			// ���̰������
	Timer::__exec();			// ��ʱ��ִ�г���
	ActionManager::__exec();	// ����������ִ�г���
	FreePool::__flush();		// ˢ���ڴ��
}

void App::_mainLoop()
{
	while (true)
	{
		if (m_bRunning)
		{
			MouseMsg::__exec();			// �����
			KeyMsg::__exec();			// ���̰������
			Timer::__exec();			// ��ʱ��ִ�г���
			ActionManager::__exec();	// ����������ִ�г���
			FreePool::__flush();		// ˢ���ڴ��
		}
		std::this_thread::sleep_for(milliseconds(10));
	}
	
}

void App::createWindow(int width, int height, int mode)
{
	// ���洰����Ϣ
	m_Size.cx = width;
	m_Size.cy = height;
	m_nWindowMode = mode;
	// ��������
	_initGraph();
}

void App::createWindow(CSize size, int mode)
{
	createWindow(size.cx, size.cy, mode);
}

void App::createWindow(TString title, int width, int height, int mode)
{
	// ���洰����Ϣ
	m_Size.cx = width;
	m_Size.cy = height;
	m_nWindowMode = mode;
	m_sTitle = title;
	if (m_sAppName.empty()) m_sAppName = title;
	// ��������
	_initGraph();
}

void App::createWindow(TString title, CSize size, int mode)
{
	createWindow(title, size.cx, size.cy, mode);
}

void App::setWindowSize(int width, int height)
{
	// ��Ϸ��������ʱ�������޸Ĵ��ڴ�С
	assert(s_pInstance->m_bRunning);

	// ��ȡ��Ļ�ֱ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// ��ȡ���ڴ�С�������˵�����
	CRect rcWindow;
	GetWindowRect(GetHWnd(), &rcWindow);
	// ��ȡ�ͻ�����С
	CRect rcClient;
	GetClientRect(GetHWnd(), &rcClient);
	// ����߿��С
	width += (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	height += (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	// ���ٵ�ǰ����
	// DestroyWindow(GetHWnd());/* �޷��������̵߳���ʧЧ */
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	SetWindowPos(GetHWnd(), HWND_TOP,
		(screenWidth - width) / 2,
		(screenHeight - height) / 2,
		width,
		height,
		SWP_SHOWWINDOW);
	// ���ô�������
	reset();
}

void App::setWindowSize(CSize size)
{
	setWindowSize(size.cx, size.cy);
}

void App::setWindowTitle(TString title)
{
	// ���ô��ڱ���
	SetWindowText(GetHWnd(), title.c_str());
	// ���浱ǰ���⣬�����޸Ĵ��ڴ�Сʱ�ָ�����
	s_pInstance->m_sTitle = title;
}

TString App::getWindowTitle()
{
	return s_pInstance->m_sTitle;
}

void App::close()
{
	closegraph();	// �رջ�ͼ����
}

void App::enterScene(Scene * scene, bool save)
{
	// ������һ������ָ��
	s_pInstance->m_pNextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	s_pInstance->m_bSaveScene = save;
}

void App::backScene()
{
	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	s_pInstance->m_pNextScene = s_SceneStack.top();
	// �����浱ǰ����
	s_pInstance->m_bSaveScene = false;
}

void App::clearScene()
{
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeDelete(temp);
		s_SceneStack.pop();
	}
}

void App::setAppName(TString appname)
{
	s_pInstance->m_sAppName = appname;
}

TString App::getAppName()
{
	return s_pInstance->m_sAppName;
}

void App::setBkColor(COLORREF color)
{
	setbkcolor(color);
}

void App::_enterNextScene()
{
	bool bBackScene = false;

	// ����һ��������ջ����˵�����ڷ�����һ����
	if (s_SceneStack.size() && m_pNextScene == s_SceneStack.top())
	{
		bBackScene = true;
		// ɾ��ջ������
		s_SceneStack.pop();
	}

	// ִ�е�ǰ������ onExit ����
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();
		if (m_bSaveScene)
		{
			// ��Ҫ���浱ǰ��������������ջ��
			s_SceneStack.push(m_pCurrentScene);
			// ��ͣ��ǰ���������е����ж�ʱ��
			Timer::waitAllSceneTimers(m_pCurrentScene);
			MouseMsg::waitAllSceneListeners(m_pCurrentScene);
			KeyMsg::waitAllSceneListeners(m_pCurrentScene);
			ActionManager::waitAllSceneActions(m_pCurrentScene);
		}
		else
		{
			// �����泡��ʱ��ֹͣ��ǰ���������е����ж�ʱ������ɾ����ǰ����
			Timer::clearAllSceneTimers(m_pCurrentScene);
			MouseMsg::clearAllSceneListeners(m_pCurrentScene);
			KeyMsg::clearAllSceneListeners(m_pCurrentScene);
			ActionManager::stopAllSceneActions(m_pCurrentScene);
			SafeDelete(m_pCurrentScene);
		}
	}

	m_pCurrentScene = m_pNextScene;		// �л�����
	m_pNextScene = nullptr;				// ��һ�����ÿ�

	if (bBackScene)
	{
		// ������һ����ʱ���ָ������ϵĶ�ʱ��
		Timer::notifyAllSceneTimers(m_pCurrentScene);
		MouseMsg::notifyAllSceneListeners(m_pCurrentScene);
		KeyMsg::notifyAllSceneListeners(m_pCurrentScene);
		ActionManager::notifyAllSceneActions(m_pCurrentScene);
	}
	else
	{
		m_pCurrentScene->init();		// ����һ���³���ʱ��ִ������ init ����
	}

	m_pCurrentScene->onEnter();			// ִ����һ������ onEnter ����
}

void App::quit()
{
	s_pInstance->m_bRunning = false;
}

void App::end()
{
	s_pInstance->m_bRunning = false;
}

void App::reset()
{
	// ���û�ͼ����
	graphdefaults();
	setbkmode(TRANSPARENT);
	setbkcolor(Color::black);
}

Scene * App::getCurrentScene()
{
	// ��ȡ��ǰ������ָ��
	return s_pInstance->m_pCurrentScene;
}

Scene * App::getLoadingScene()
{
	return s_pInstance->m_pLoadingScene;
}

int App::getWidth()
{
	return s_pInstance->m_Size.cx;
}

int App::getHeight()
{
	return s_pInstance->m_Size.cy;
}

void App::free()
{
	// �ͷų����ڴ�
	SafeDelete(m_pCurrentScene);
	SafeDelete(m_pNextScene);
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeDelete(temp);
		s_SceneStack.pop();
	}
	// ɾ�����ж�ʱ��
	Timer::clearAllTimers();
	MouseMsg::clearAllListeners();
	KeyMsg::clearAllListeners();
	ActionManager::clearAllActions();
	// ɾ�����ж���
	FreePool::__clearAllObjects();
}
