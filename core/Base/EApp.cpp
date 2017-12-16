#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\etransitions.h"
#include "..\etools.h"
#include <stack>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


// Ψһʵ��ָ��
static e2d::EApp * s_pInstance = nullptr;
// ����ջ
static std::stack<e2d::EScene*> s_SceneStack;
// ��Ϸ��ʼʱ��
static LARGE_INTEGER s_tStart;


e2d::EApp::EApp()
	: m_bEnd(false)
	, m_bPaused(false)
	, m_bManualPaused(false)
	, m_bShowConsole(false)
	, m_nAnimationInterval()
	, m_ClearColor(EColor::BLACK)
	, m_pTransition(nullptr)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
{
	CoInitialize(NULL);

	// ��ȡʱ��Ƶ��
	LARGE_INTEGER tFreq;
	QueryPerformanceFrequency(&tFreq);
	// Ĭ��֡��Ϊ 60
	m_nAnimationInterval.QuadPart = static_cast<LONGLONG>(1.0 / 60 * tFreq.QuadPart);
}

e2d::EApp::~EApp()
{
	SafeReleaseInterface(&GetSolidColorBrush());
	SafeReleaseInterface(&GetRenderTarget());
	SafeReleaseInterface(&GetFactory());
	SafeReleaseInterface(&GetImagingFactory());
	SafeReleaseInterface(&GetDirectWriteFactory());
	CoUninitialize();
}

e2d::EApp * e2d::EApp::getInstance()
{
	if (!s_pInstance)
	{
		s_pInstance = new EApp();
	}
	return s_pInstance;
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, const EWindowStyle &wStyle /* = nullptr */)
{
	EApp * pApp = EApp::getInstance();
	HRESULT hr;

	// ���洰����ʽ
	pApp->m_WindowStyle = wStyle;
	// ���洰������
	pApp->m_sTitle = title;

	// ע�ᴰ����
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = EApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"Easy2DApp";
	// ���ô����Ƿ��йرհ�ť
	if (wStyle.m_bNoClose)
	{
		wcex.style |= CS_NOCLOSE;
	}
	// ���ó���ͼ��
	if (wStyle.m_pIconID)
	{
		wcex.hIcon = (HICON)::LoadImage(
			GetModuleHandle(NULL), 
			wStyle.m_pIconID, 
			IMAGE_ICON, 
			0, 
			0, 
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}

	RegisterClassEx(&wcex);

	// ��Ϊ CreateWindow ����ʹ�õ������ش�С����ȡϵͳ�� DPI ��ʹ��
	// ��Ӧ��������
	FLOAT dpiX, dpiY;

	// ���������ص�ǰ��ϵͳ DPI�����ֵҲ��������������
	GetFactory()->GetDesktopDpi(&dpiX, &dpiY);

	width = static_cast<UINT>(ceil(width * dpiX / 96.f));
	height = static_cast<UINT>(ceil(height * dpiY / 96.f));

	// ��ȡ��Ļ�ֱ���
	UINT screenWidth = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN));
	// ������Ĵ��ڴ�С�ȷֱ��ʴ�ʱ����������
	WARN_IF(screenWidth < width || screenHeight < height, "The window is larger than screen!");
	// ȡ��Сֵ
	width = min(width, screenWidth);
	height = min(height, screenHeight);

	// ����������ʽ
	DWORD dwStyle = WS_OVERLAPPED | WS_SYSMENU;
	if (!wStyle.m_bNoMiniSize)
	{
		dwStyle |= WS_MINIMIZEBOX;
	}
	// ��������
	GetHWnd() = CreateWindow(
		L"Easy2DApp",
		title,
		dwStyle,
		0, 0, width, height,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		NULL
	);

	hr = GetHWnd() ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// �������뷨
		EApp::setKeyboardLayoutEnable(false);
		// ����ͻ�����С
		EApp::setWindowSize(width, height);
	}
	else
	{
		UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

int e2d::EApp::run()
{
	if (GetHWnd() == nullptr)
	{
		MessageBox(nullptr, L"Invalid window handle!", L"Error", MB_OK);
		return -1;
	}

	EApp * pApp = EApp::getInstance();
	// �����һ������
	pApp->_enterNextScene();
	// �رտ���̨
	if (!pApp->m_bShowConsole)
	{
		showConsole(false);
	}
	// ��ʾ����
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// ���ô����ö�
	if (pApp->m_WindowStyle.m_bTopMost)
	{
		SetWindowPos(GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	// ����ʱ��
	LONG nWait = 0L;
	// ʱ����
	LONGLONG nInterval;
	// ��һ֡�������ʱ��
	LARGE_INTEGER tLast;
	// ʱ��Ƶ��
	LARGE_INTEGER tFreq;

	// �޸�ʱ�侫��
	timeBeginPeriod(1);
	// ��ȡʱ��Ƶ��
	QueryPerformanceFrequency(&tFreq);
	// ˢ�µ�ǰʱ��
	QueryPerformanceCounter(&GetNow());
	// ��¼��ʼʱ��
	s_tStart = GetNow();
	tLast = GetNow();
	// ������Ϣ
	MSG msg;

	while (!pApp->m_bEnd)
	{
		// ��������Ϣ
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ˢ�µ�ǰʱ��
		QueryPerformanceCounter(&GetNow());
		// ����ʱ����
		nInterval = GetNow().QuadPart - tLast.QuadPart;
		// �жϼ��ʱ���Ƿ��㹻
		if (nInterval >= pApp->m_nAnimationInterval.QuadPart)
		{
			// ��¼��ǰʱ��
			tLast.QuadPart += pApp->m_nAnimationInterval.QuadPart;
			// ������Ϸ����
			pApp->_update();
			// ˢ����Ϸ����
			pApp->_render();
		}
		else
		{
			// �������ʱ��
			nWait = static_cast<LONG>(ToMilliseconds(pApp->m_nAnimationInterval.QuadPart - nInterval) - 1LL);
			// �����̣߳��ͷ� CPU ռ��
			if (nWait > 1L)
			{
				Sleep(nWait);
			}
		}
	}

	// �رտ���̨
	EApp::showConsole(false);
	// ����ʱ�侫��
	timeEndPeriod(1);

	return 0;
}

void e2d::EApp::pause()
{
	EApp::getInstance()->m_bManualPaused = true;
}

void e2d::EApp::resume()
{
	if (isPaused())
	{
		EApp::getInstance()->m_bPaused = false;
		EApp::getInstance()->m_bManualPaused = false;
		EApp::getInstance()->_updateTime();
	}
}

bool e2d::EApp::isPaused()
{
	return s_pInstance->m_bPaused || s_pInstance->m_bManualPaused;
}

void e2d::EApp::quit()
{
	getInstance()->m_bEnd = true;	// ���������������Ϸ�Ƿ����
}

void e2d::EApp::showConsole(bool show /* = true */)
{
	static FILE * stdoutstream = nullptr;
	static FILE * stdinstream = nullptr;
	static FILE * stderrstream = nullptr;

	EApp::getInstance()->m_bShowConsole = show;
	// �����Ѵ��ڵĿ���̨���
	HWND hwnd = GetConsoleWindow();
	// �رտ���̨
	if (show)
	{
		if (hwnd)
		{
			ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		else
		{
			// ��ʾһ���¿���̨
			if (AllocConsole())
			{
				freopen_s(&stdoutstream, "CONOUT$", "w+t", stdout);
				freopen_s(&stderrstream, "CONOUT$", "w+t", stderr);
				freopen_s(&stdinstream, "CONIN$", "r+t", stdin);
			}
			else
			{
				MessageBox(nullptr, L"Alloc Console Failed!", L"Error", MB_OK);
			}
		}
	}
	else
	{
		if (hwnd)
		{
			if (stdoutstream)
			{
				fclose(stdoutstream);
				fclose(stdinstream);
				fclose(stderrstream);

				stdoutstream = stdinstream = stderrstream = nullptr;
			}
			FreeConsole();
		}
	}
}

bool e2d::EApp::onActivate()
{
	return false;
}

bool e2d::EApp::onInactive()
{
	return false;
}

bool e2d::EApp::onCloseWindow()
{
	return true;
}

void e2d::EApp::_update()
{
	if (isPaused())
	{
		return;
	}

	// �����л�����ʱ��ִ�г����л�����
	if (m_pTransition)
	{
		m_pTransition->_update();
		if (m_pTransition->isEnding())
		{
			m_pTransition->release();
			m_pTransition = nullptr;
			// ������һ����
			_enterNextScene();
			// ˢ�¼�ʱ��
			_updateTime();
		}
		return;
	}

	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (m_pNextScene)
	{
		// ������һ����
		_enterNextScene();
	}

	// ���Ե�ǰ�����ǿ�
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");

	EObjectManager::__flush();		// ˢ���ڴ��
	ETimerManager::TimerProc();		// ��ʱ��������ִ�г���
	EActionManager::ActionProc();	// ����������ִ�г���
}

void e2d::EApp::_render()
{
	HRESULT hr = S_OK;

	// ��ʼ��ͼ
	GetRenderTarget()->BeginDraw();
	// ʹ�ñ���ɫ�����Ļ
	GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));
	// ���Ƶ�ǰ����
	if (m_pCurrentScene)
	{
		m_pCurrentScene->_render();
	}
	// �л�����ʱ��ͬʱ����������
	if (m_pTransition && m_pNextScene)
	{
		m_pNextScene->_render();
	}
	// ��ֹ��ͼ
	hr = GetRenderTarget()->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// ��� Direct3D �豸��ִ�й�������ʧ����������ǰ���豸�����Դ
		// ������һ�ε���ʱ�ؽ���Դ
		hr = S_OK;
		SafeReleaseInterface(&GetRenderTarget());
	}

	if (FAILED(hr))
	{
		// ��Ⱦʱ������δ֪�Ĵ����˳���Ϸ
		MessageBox(GetHWnd(), L"Game rendering failed!", L"Error", MB_OK);
		this->quit();
	}
}

void e2d::EApp::setWindowSize(UINT32 width, UINT32 height)
{
	// ��ȡ��Ļ�ֱ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// ��ȡ���ڴ�С�������˵�����
	tagRECT rcWindow;
	GetWindowRect(GetHWnd(), &rcWindow);
	// ��ȡ�ͻ�����С
	tagRECT rcClient;
	GetClientRect(GetHWnd(), &rcClient);
	// ����߿��С
	width += (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	height += (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	MoveWindow(GetHWnd(), (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::EApp::setWindowTitle(const EString &title)
{
	// ���ô��ڱ���
	SetWindowText(GetHWnd(), title);
	// ���浱ǰ���⣬�����޸Ĵ��ڴ�Сʱ�ָ�����
	getInstance()->m_sTitle = title;
}

e2d::EString e2d::EApp::getTitle()
{
	return getInstance()->m_sTitle;
}

float e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetSize().width;
}

float e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetSize().height;
}

e2d::ESize e2d::EApp::getSize()
{
	return ESize(GetRenderTarget()->GetSize().width, GetRenderTarget()->GetSize().height);
}

void e2d::EApp::enterScene(EScene * scene, ETransition * transition /* = nullptr */, bool saveCurrentScene /* = true */)
{
	ASSERT(scene != nullptr, "Next scene NULL pointer exception!");
	scene->retain();
	// ������һ������ָ��
	getInstance()->m_pNextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	if (getInstance()->m_pCurrentScene)
	{
		getInstance()->m_pCurrentScene->m_bWillSave = saveCurrentScene;
	}
	// �����л���������
	if (transition)
	{
		getInstance()->m_pTransition = transition;
		transition->retain();
		transition->_setTarget(
			getInstance()->m_pCurrentScene, 
			getInstance()->m_pNextScene
		);
	}
}

void e2d::EApp::backScene(ETransition * transition /* = nullptr */)
{
	// ջΪ��ʱ�����÷��س�������ʧ��
	WARN_IF(s_SceneStack.size() == 0, "Scene stack now is empty!");
	if (s_SceneStack.size() == 0) return;

	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	getInstance()->m_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();

	// ������һ����ʱ�������浱ǰ����
	if (getInstance()->m_pCurrentScene)
	{
		getInstance()->m_pCurrentScene->m_bWillSave = false;
	}

	// �����л���������
	if (transition)
	{
		getInstance()->m_pTransition = transition;
		transition->retain();
		transition->_setTarget(
			getInstance()->m_pCurrentScene, 
			getInstance()->m_pNextScene
		);
	}
}

void e2d::EApp::clearScene()
{
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeRelease(&temp);
		s_SceneStack.pop();
	}
}

void e2d::EApp::setFPS(UINT32 fps)
{
	fps = min(max(fps, 30), 120);

	// ��ȡʱ��Ƶ��
	LARGE_INTEGER tFreq;
	QueryPerformanceFrequency(&tFreq);
	EApp::getInstance()->m_nAnimationInterval.QuadPart = static_cast<LONGLONG>(1.0 / fps * tFreq.QuadPart);
}

e2d::EScene * e2d::EApp::getCurrentScene()
{
	return getInstance()->m_pCurrentScene;
}

void e2d::EApp::setAppName(const EString &appname)
{
	getInstance()->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	if (getInstance()->m_sAppName.isEmpty())
		getInstance()->m_sAppName = getInstance()->m_sTitle;
	return getInstance()->m_sAppName;
}

void e2d::EApp::setBkColor(UINT32 color)
{
	getInstance()->m_ClearColor = color;
}

void e2d::EApp::setKeyboardLayoutEnable(bool value)
{
	static HIMC hImc = NULL;

	if (value)
	{
		if (hImc != NULL)
		{
			ImmAssociateContext(GetHWnd(), hImc);
			hImc = NULL;
		}
	}
	else
	{
		if (hImc == NULL)
		{
			hImc = ImmAssociateContext(GetHWnd(), NULL);
		}
	}
}

HWND e2d::EApp::getHWnd()
{
	return GetHWnd();
}

e2d::EWindowStyle e2d::EApp::getWindowStyle()
{
	return getInstance()->m_WindowStyle;
}

LONGLONG e2d::EApp::getTotalDurationFromStart()
{
	return (s_tStart.QuadPart - GetNow().QuadPart) * 1000LL / GetFreq().QuadPart;
}

void e2d::EApp::hideWindow()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::showWindow()
{
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
}

void e2d::EApp::_enterNextScene()
{
	if (m_pNextScene == nullptr)
		return;

	// ִ�е�ǰ������ onCloseWindow ����
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();

		if (m_pCurrentScene->m_bWillSave)
		{
			// ��Ҫ���浱ǰ��������������ջ��
			s_SceneStack.push(m_pCurrentScene);
		}
		else
		{
			SafeRelease(&m_pCurrentScene);
		}
	}

	// ִ����һ������ onEnter ����
	m_pNextScene->onEnter();

	m_pCurrentScene = m_pNextScene;		// �л�����
	m_pNextScene = nullptr;				// ��һ�����ÿ�
}

void e2d::EApp::_updateTime()
{
	// ˢ�µ�ǰʱ��
	QueryPerformanceCounter(&GetNow());
	// ���ö����Ͷ�ʱ��
	EActionManager::_resetAllActions();
	ETimerManager::_resetAllTimers();
}

LRESULT e2d::EApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (s_pInstance == nullptr)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else
	{
		LRESULT result = 0;

		switch (message)
		{
		// ���������Ϣ
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		{
			// ִ�г����л�ʱ���ΰ����������Ϣ
			if (!s_pInstance->m_pTransition && !s_pInstance->m_pNextScene)
			{
				EMsgManager::MouseProc(message, wParam, lParam);
			}
		}
		result = 0;
		break;

		// ��������Ϣ
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			// ִ�г����л�ʱ���ΰ����������Ϣ
			if (!s_pInstance->m_pTransition && !s_pInstance->m_pNextScene)
			{
				EMsgManager::KeyboardProc(message, wParam, lParam);
			}
		}
		result = 0;
		break;

		// �����ڴ�С�仯��Ϣ
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			// ���������յ�һ�� WM_SIZE ��Ϣ�����������������Ⱦ
			// Ŀ���ʵ��������ܻ����ʧ�ܣ�����������Ժ����п��ܵ�
			// ������Ϊ�����������һ�ε��� EndDraw ʱ����
			GetRenderTarget()->Resize(D2D1::SizeU(width, height));
		}
		break;

		// ����ֱ��ʱ仯��Ϣ
		case WM_DISPLAYCHANGE:
		{
			// �ػ�ͻ���
			InvalidateRect(hWnd, NULL, FALSE);
		}
		result = 0;
		break;

		// �ػ洰��
		case WM_PAINT:
		{
			s_pInstance->_render();
			ValidateRect(hWnd, NULL);
		}
		result = 0;
		break;

		// ���ڼ�����Ϣ
		case WM_ACTIVATE:
		{
			EScene * pCurrentScene = s_pInstance->getCurrentScene();

			if (LOWORD(wParam) == WA_INACTIVE)
			{
				if (pCurrentScene &&
					pCurrentScene->onInactive() &&
					s_pInstance->onInactive())
				{
					s_pInstance->m_bPaused = true;
				}
			}
			else if (s_pInstance->m_bPaused)
			{
				if (pCurrentScene &&
					pCurrentScene->onActivate() &&
					s_pInstance->onActivate())
				{
					s_pInstance->m_bPaused = false;
					if (!s_pInstance->m_bManualPaused)
					{
						s_pInstance->_updateTime();
					}
				}
			}
		}
		result = 1;
		break;

		// ���ڹر���Ϣ
		case WM_CLOSE:
		{
			EScene * pCurrentScene = s_pInstance->getCurrentScene();

			if (pCurrentScene)
			{
				if (pCurrentScene->onCloseWindow() && 
					s_pInstance->onCloseWindow())
				{
					DestroyWindow(hWnd);
				}
			}
			else 
			{
				if (s_pInstance->onCloseWindow())
				{
					DestroyWindow(hWnd);
				}
			}
		}
		result = 1;
		break;

		// ���ڱ�����
		case WM_DESTROY:
		{
			// �˳�����
			s_pInstance->quit();
			// �����˳���Ϣ
			PostQuitMessage(0);
		}
		result = 1;
		break;

		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}

		return result;
	}
}
