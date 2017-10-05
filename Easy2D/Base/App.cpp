#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <time.h>
#include <assert.h>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// App ��Ψһʵ��
static App * s_pInstance = nullptr;
// ����ԭ�����������
static int originX = 0;
static int originY = 0;

App::App() : 
	m_CurrentScene(nullptr), 
	m_NextScene(nullptr), 
	m_bRunning(false), 
	m_nWindowMode(0)
{
	assert(!s_pInstance);	// ����ͬʱ�������� App ʵ��
	s_pInstance = this;		// ����ʵ������
	setFPS(60);				// Ĭ�� FPS Ϊ 60
}

App::~App()
{
	destory();				// ���� App
}

App * App::get()
{
	assert(s_pInstance);	// ����ʵ������
	return s_pInstance;		// ��ȡ App ��Ψһʵ��
}

void App::setOrigin(int originX, int originY)
{
	::originX = originX;
	::originY = originY;
	setorigin(originX, originY);
}

int App::getOriginX()
{
	return ::originX;
}

int App::getOriginY()
{
	return ::originY;
}

int App::run()
{
	// ����������ͼ
	BeginBatchDraw();
	// �޸�ʱ�侫��
	timeBeginPeriod(1);
	// ��ȡ CPU ÿ��δ�������
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	// ����ʱ�����
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&nLast);
	// ʱ����
	LONGLONG interval = 0LL;
	// ����ʱ��
	LONG waitMS = 0L;
	// �����صĴ�����ʾ
	ShowWindow(GetHWnd(), SW_NORMAL);
	// ������Ϸ
	m_bRunning = true;

	// ������ѭ��
	while (m_bRunning)
	{
		// ��ȡ��ǰʱ��
		QueryPerformanceCounter(&nNow);
		// ����ʱ����
		interval = nNow.QuadPart - nLast.QuadPart;
		// �жϼ��ʱ���Ƿ��㹻
		if (interval >= m_nAnimationInterval.QuadPart)
		{
			// ��¼��ǰʱ��
			nLast.QuadPart = nNow.QuadPart;
			// ִ����Ϸ�߼�
			_mainLoop();
		}
		else
		{
			// �������ʱ��
			waitMS = LONG((m_nAnimationInterval.QuadPart - interval) * 1000LL / freq.QuadPart) - 1L;
			// �����̣߳��ͷ� CPU ռ��
			if (waitMS > 1L)
				Sleep(waitMS);
		}
	}
	// ֹͣ������ͼ
	EndBatchDraw();
	// �رմ���
	close();
	// �ͷ������ڴ�ռ��
	destory();
	// ����ʱ�侫��
	timeEndPeriod(1);

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
		m_sAppName = title;
	}
	else
	{
		setWindowTitle(m_sTitle);
	}
}

void App::_mainLoop()
{
	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (m_NextScene)
	{
		// ִ�е�ǰ������ onExit ����
		if (m_CurrentScene)
		{
			m_CurrentScene->onExit();
		}
		// ������һ����
		_enterNextScene();
		// ִ�е�ǰ������ onEnter ����
		m_CurrentScene->onEnter();
	}
	// ���Ե�ǰ�����ǿ�
	assert(m_CurrentScene);
	
	cleardevice();				// ��ջ���
	m_CurrentScene->_onDraw();	// ���Ƶ�ǰ����
	FlushBatchDraw();			// ˢ�»���

	// ����ִ�г���
	MouseMsg::__exec();			// �����
	KeyMsg::__exec();			// ���̰������
	Timer::__exec();			// ��ʱ��ִ�г���
	ActionManager::__exec();	// ����������ִ�г���
	FreePool::__flush();		// ˢ���ڴ��
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
	s_pInstance->m_NextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	s_pInstance->m_bSaveScene = save;
}

void App::backScene()
{
	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	s_pInstance->m_NextScene = s_pInstance->m_SceneStack.top();
	// �����浱ǰ����
	s_pInstance->m_bSaveScene = false;
}

void App::clearScene()
{
	// ��ճ���ջ
	while (s_pInstance->m_SceneStack.size())
	{
		auto temp = s_pInstance->m_SceneStack.top();
		SafeDelete(temp);
		s_pInstance->m_SceneStack.pop();
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
	// ����һ��������ջ����˵�����ڷ�����һ����
	if (m_SceneStack.size() && m_NextScene == m_SceneStack.top())
	{
		m_SceneStack.pop();					// ɾ��ջ������
	}
	
	if (m_bSaveScene)
	{
		m_SceneStack.push(m_CurrentScene);	// ��Ҫ���浱ǰ������������ָ��ŵ�ջ��
	}
	else
	{
		SafeDelete(m_CurrentScene);		// ����ɾ����ǰ����
	}
	
	m_CurrentScene = m_NextScene;			// �л�����
	m_NextScene = nullptr;					// ��һ�����ÿ�
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
	return s_pInstance->m_CurrentScene;
}

void App::setFPS(DWORD fps)
{
	// ���û���֡�ʣ��Ժ���Ϊ��λ
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	s_pInstance->m_nAnimationInterval.QuadPart = (LONGLONG)(1.0 / fps * nFreq.QuadPart);
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
	SafeDelete(m_CurrentScene);
	SafeDelete(m_NextScene);
	// ��ճ���ջ
	while (m_SceneStack.size())
	{
		auto temp = m_SceneStack.top();
		SafeDelete(temp);
		m_SceneStack.pop();
	}
	// ɾ�����ж�ʱ��
	Timer::clearAllTimers();
	MouseMsg::clearAllListeners();
	KeyMsg::clearAllListeners();
	ActionManager::clearAllActions();
	// ɾ�����ж���
	FreePool::__clearAllObjects();
}

void App::destory()
{
	// �ͷ������ڴ�
	s_pInstance->free();
	// ʵ��ָ���ÿ�
	delete s_pInstance;
	s_pInstance = nullptr;
}
