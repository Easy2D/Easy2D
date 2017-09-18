#include "easy2d.h"
#include "EasyX\easyx.h"
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
	m_currentScene(nullptr), 
	m_nextScene(nullptr), 
	m_bRunning(false), 
	m_nWidth(0),
	m_nHeight(0),
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
	initgraph(m_nWidth, m_nHeight, m_nWindowMode);
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
	}
	else
	{
		setWindowText(m_sTitle);
	}
}

void App::_mainLoop()
{
	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (m_nextScene)
	{
		// ִ�е�ǰ������ onExit ����
		if (m_currentScene)
		{
			m_currentScene->onExit();
		}
		// ������һ����
		_enterNextScene();
		// ִ�е�ǰ������ onEnter ����
		m_currentScene->onEnter();
	}
	// ���Ե�ǰ�����ǿ�
	assert(m_currentScene);
	
	cleardevice();				// ��ջ���
	m_currentScene->_onDraw();	// ���Ƶ�ǰ����
	FlushBatchDraw();			// ˢ�»���

	// ����ִ�г���
	MouseMsg::__exec();			// �����
	KeyMsg::__exec();			// ���̰������
	Timer::__exec();			// ��ʱ��ִ�г���
	FreePool::__flush();		// ˢ���ڴ��
}

void App::createWindow(int width, int height, int mode)
{
	// ���洰����Ϣ
	m_nWidth = width;
	m_nHeight = height;
	m_nWindowMode = mode;
	// ��������
	_initGraph();
}

void App::createWindow(tstring title, int width, int height, int mode)
{
	// ���洰����Ϣ
	m_nWidth = width;
	m_nHeight = height;
	m_nWindowMode = mode;
	m_sTitle = title;
	// ��������
	_initGraph();
}

void App::setWindowSize(int width, int height)
{
	// ��Ϸ��������ʱ�������޸Ĵ��ڴ�С
	assert(m_bRunning);

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

void App::setWindowText(tstring title)
{
	// ���ô��ڱ���
	SetWindowText(GetHWnd(), title.c_str());
	// ���浱ǰ���⣬�����޸Ĵ��ڴ�Сʱ�ָ�����
	m_sTitle = title;
}

void App::close()
{
	closegraph();	// �رջ�ͼ����
}

void App::enterScene(Scene * scene, bool save)
{
	// ������һ������ָ��
	m_nextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	m_bSaveScene = save;
}

void App::backScene()
{
	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	m_nextScene = m_sceneStack.top();
	// �����浱ǰ����
	m_bSaveScene = false;
}

void App::clearScene()
{
	// ��ճ���ջ
	while (m_sceneStack.size())
	{
		auto temp = m_sceneStack.top();
		SAFE_DELETE(temp);
		m_sceneStack.pop();
	}
}

void App::setBkColor(COLORREF color)
{
	setbkcolor(color);
}

void App::_enterNextScene()
{
	// ����һ��������ջ����˵�����ڷ�����һ����
	if (m_sceneStack.size() && m_nextScene == m_sceneStack.top())
	{
		m_sceneStack.pop();					// ɾ��ջ������
	}
	
	if (m_bSaveScene)
	{
		m_sceneStack.push(m_currentScene);	// ��Ҫ���浱ǰ������������ָ��ŵ�ջ��
	}
	else
	{
		SAFE_DELETE(m_currentScene);		// ����ɾ����ǰ����
	}
	
	m_currentScene = m_nextScene;			// �л�����
	m_nextScene = nullptr;					// ��һ�����ÿ�
}

void App::quit()
{
	m_bRunning = false;
}

void App::end()
{
	m_bRunning = false;
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
	return m_currentScene;
}

LPCTSTR easy2d::App::getVersion()
{
	return _T("1.0.2");
}

void App::setFPS(DWORD fps)
{
	// ���û���֡�ʣ��Ժ���Ϊ��λ
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	m_nAnimationInterval.QuadPart = (LONGLONG)(1.0 / fps * nFreq.QuadPart);
}

int App::getWidth() const
{
	return m_nWidth;
}

int App::getHeight() const
{
	return m_nHeight;
}

void App::free()
{
	// �ͷų����ڴ�
	SAFE_DELETE(m_currentScene);
	SAFE_DELETE(m_nextScene);
	// ��ճ���ջ
	while (m_sceneStack.size())
	{
		auto temp = m_sceneStack.top();
		SAFE_DELETE(temp);
		m_sceneStack.pop();
	}
	// ɾ�����ж�ʱ��
	Timer::clearAllTimers();
}

void App::destory()
{
	// �ͷ������ڴ�
	free();
	// ʵ��ָ���ÿ�
	s_pInstance = nullptr;
}
