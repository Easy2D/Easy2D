#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emsg.h"
#include "..\etools.h"
#include "..\enodes.h"
#include "..\etransitions.h"
#include <stack>
#include <thread>
#include <imm.h>  
#pragma comment (lib ,"imm32.lib")

using namespace std::this_thread;
using namespace std::chrono;


// Ψһʵ��ָ��
static e2d::EApp * s_pInstance = nullptr;
// ����ջ
static std::stack<e2d::EScene*> s_SceneStack;
// ��Ϸ��ʼʱ��
static steady_clock::time_point s_tStart;

e2d::EApp::EApp()
	: m_bEnd(false)
	, m_bPaused(false)
	, m_bManualPaused(false)
	, m_bTransitional(false)
	, m_bTopMost(false)
	, nAnimationInterval(17LL)
	, m_ClearColor(EColor::BLACK)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
{
	ASSERT(s_pInstance == nullptr, "EApp instance already exists!");
	s_pInstance = this;		// ����ʵ������

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

e2d::EApp::~EApp()
{
	// �ͷ���Դ
	SafeReleaseInterface(&GetSolidColorBrush());
	SafeReleaseInterface(&GetRenderTarget());
	SafeReleaseInterface(&GetFactory());
	SafeReleaseInterface(&GetImagingFactory());
	SafeReleaseInterface(&GetDirectWriteFactory());
	
	CoUninitialize();
}

e2d::EApp * e2d::EApp::get()
{
	ASSERT(s_pInstance != nullptr, "Nonexistent EApp instance.");
	return s_pInstance;		// ��ȡ EApp ��Ψһʵ��
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, bool showConsole /* = false */)
{
	return init(title, width, height, EWindowStyle(), showConsole);
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, EWindowStyle wStyle, bool showConsole /* = false */)
{
	HRESULT hr;

	// ��ʾ��رտ���̨
	EApp::showConsole(showConsole);

	// �����豸�޹���Դ
	hr = _createDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// ע�ᴰ����
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		UINT style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		if (wStyle.NO_CLOSE)
		{
			style |= CS_NOCLOSE;
		}
		wcex.style = style;
		wcex.lpfnWndProc = EApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"Easy2DApp";
		if (wStyle.ICON_ID)
		{
			wcex.hIcon = (HICON)::LoadImage(GetModuleHandle(NULL), wStyle.ICON_ID, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		}

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		GetFactory()->GetDesktopDpi(&dpiX, &dpiY);

		width = static_cast<UINT>(ceil(width * dpiX / 96.f));
		height = static_cast<UINT>(ceil(height * dpiY / 96.f));

		// ��ȡ��Ļ�ֱ���
		UINT screenWidth = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN));
		UINT screenHeight = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN));
		// ������Ĵ��ڴ�С�ȷֱ��ʴ�ʱ����������
		WARN_IF(screenWidth < width || screenHeight < height, "The window is larger than screen!");
		width = min(width, screenWidth);
		height = min(height, screenHeight);
		
		// ����������ʽ
		DWORD dwStyle = WS_OVERLAPPED | WS_SYSMENU;
		if (!wStyle.NO_MINI_SIZE)
		{
			dwStyle |= WS_MINIMIZEBOX;
		}
		// ���洰���Ƿ��ö���ʾ
		m_bTopMost = wStyle.TOP_MOST;
		// ���洰������
		m_sTitle = title;
		// ��������
		GetHWnd() = CreateWindow(
			L"Easy2DApp",
			m_sTitle.c_str(),
			dwStyle,
			0,
			0,
			width,
			height,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		hr = GetHWnd() ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			// �������뷨
			this->setKeyboardLayoutEnable(false);
			// ����ͻ�����С
			this->setWindowSize(width, height);
		}
		else
		{
			UnregisterClass(L"E2DApp", HINST_THISCOMPONENT);
		}
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

void e2d::EApp::pause()
{
	EApp::get()->m_bManualPaused = true;
}

void e2d::EApp::resume()
{
	if (isPaused())
	{
		EApp::get()->m_bPaused = false;
		EApp::get()->m_bManualPaused = false;
		// ˢ�µ�ǰʱ��
		GetNow() = steady_clock::now();
		// ���ö����Ͷ�ʱ��
		EActionManager::_resetAllActions();
		ETimerManager::_resetAllTimers();
	}
}

bool e2d::EApp::isPaused()
{
	return s_pInstance->m_bPaused || s_pInstance->m_bManualPaused;
}

void e2d::EApp::showConsole(bool show)
{
	// �����Ѵ��ڵĿ���̨���
	HWND hwnd = GetConsoleWindow();
	static FILE * stdoutstream = nullptr;
	static FILE * stdinstream = nullptr;
	static FILE * stderrstream = nullptr;
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

// ������Ϸ
void e2d::EApp::run()
{
	ASSERT(GetHWnd() != nullptr, "Cannot find Game Window.");
	// �����һ������
	_enterNextScene();
	// ��ʾ����
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// ���ô����ö�
	if (m_bTopMost)
	{
		SetWindowPos(GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	// ��¼��ʼʱ��
	s_tStart = steady_clock::now();

	MSG msg;

	while (!m_bEnd)
	{
		// ��������Ϣ
		while (PeekMessage(&msg, GetHWnd(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ִ����ѭ��
		_mainLoop();
	}

	// �رտ���̨
	EApp::showConsole(false);
}

void e2d::EApp::setFPS(UINT32 fps)
{
	fps = min(max(fps, 30), 120);
	s_pInstance->nAnimationInterval = 1000 / fps;
}

bool e2d::EApp::onActivate()
{
	return true;
}

bool e2d::EApp::onInactive()
{
	return true;
}

bool e2d::EApp::onCloseWindow()
{
	return true;
}

void e2d::EApp::_mainLoop()
{
	// ʱ����
	static LONGLONG nInterval = 0LL;
	// ����ʱ��
	static LONGLONG nWaitMS = 0L;
	// ��һ֡�������ʱ��
	static steady_clock::time_point tLast = steady_clock::now();

	// ˢ�µ�ǰʱ��
	GetNow() = steady_clock::now();
	// ����ʱ����
	nInterval = GetInterval(tLast);
	// �жϼ��ʱ���Ƿ��㹻
	if (nInterval >= nAnimationInterval)
	{
		// ��¼��ǰʱ��
		tLast += microseconds(nAnimationInterval);
		// ��Ϸ��������
		_onControl();
		// ˢ����Ϸ����
		_onRender();
	}
	else
	{
		// �������ʱ��
		nWaitMS = nAnimationInterval - nInterval - 1;
		// �����̣߳��ͷ� CPU ռ��
		if (nWaitMS > 1LL)
		{
			sleep_for(milliseconds(nWaitMS));
		}
	}
}

void e2d::EApp::_onControl()
{
	// �����л�����ʱ��ִ�г����л�����
	if (m_bTransitional)
	{
		EActionManager::ActionProc();
		// �������л�δ��������ִ�к���Ĳ���
		if (m_bTransitional)
		{
			return;
		}
	}

	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (m_pNextScene)
	{
		// ������һ����
		_enterNextScene();
	}

	// ���Ե�ǰ�����ǿ�
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");

	ETimerManager::TimerProc();		// ��ʱ��������ִ�г���
	EActionManager::ActionProc();	// ����������ִ�г���
	EObjectManager::__flush();		// ˢ���ڴ��
}

// This method discards device-specific
// resources if the Direct3D device dissapears during execution and
// recreates the resources the next time it's invoked.
void e2d::EApp::_onRender()
{
	HRESULT hr = S_OK;

	hr = _createDeviceResources();

	if (SUCCEEDED(hr))
	{
		// ��ʼ��ͼ
		GetRenderTarget()->BeginDraw();
		// ʹ�ñ���ɫ�����Ļ
		GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor.value));
		// ���Ƶ�ǰ����
		if (m_pCurrentScene)
		{
			m_pCurrentScene->_onRender();
		}
		// �л�����ʱ��ͬʱ����������
		if (m_bTransitional && m_pNextScene)
		{
			m_pNextScene->_onRender();
		}
		// ��ֹ��ͼ
		hr = GetRenderTarget()->EndDraw();
		// ˢ�½���
		UpdateWindow(GetHWnd());
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		_discardDeviceResources();
	}

	if (FAILED(hr))
	{
		MessageBox(GetHWnd(), L"Game Render Failed!", L"Error", MB_OK);
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
	SetWindowText(GetHWnd(), title.c_str());
	// ���浱ǰ���⣬�����޸Ĵ��ڴ�Сʱ�ָ�����
	get()->m_sTitle = title;
}

e2d::EString e2d::EApp::getTitle()
{
	return get()->m_sTitle;
}

float e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetSize().width;
}

float e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetSize().height;
}

void e2d::EApp::enterScene(EScene * scene, bool saveCurrentScene /* = true */)
{
	enterScene(scene, nullptr, saveCurrentScene);
}

void e2d::EApp::enterScene(EScene * scene, ETransition * transition, bool saveCurrentScene /* = true */)
{
	scene->retain();
	// ������һ������ָ��
	get()->m_pNextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = saveCurrentScene;
	}
	// �����л���������
	if (transition)
	{
		get()->m_bTransitional = true;
		transition->_setTarget(
			get()->m_pCurrentScene, 
			get()->m_pNextScene, 
			get()->m_bTransitional
		);
	}
	else
	{
		get()->m_bTransitional = false;
	}
}

void e2d::EApp::backScene()
{
	backScene(nullptr);
}

void e2d::EApp::backScene(ETransition * transition)
{
	ASSERT(s_SceneStack.size(), "Scene stack now is empty!");
	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	get()->m_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();
	// �����浱ǰ����
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = false;
	}
	// �����л���������
	if (transition)
	{
		get()->m_bTransitional = true;
		transition->_setTarget(
			get()->m_pCurrentScene, 
			get()->m_pNextScene, 
			get()->m_bTransitional
		);
	}
	else
	{
		get()->m_bTransitional = false;
	}
}

void e2d::EApp::clearScene()
{
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeReleaseAndClear(&temp);
		s_SceneStack.pop();
	}
}

e2d::EScene * e2d::EApp::getCurrentScene()
{
	return get()->m_pCurrentScene;
}

void e2d::EApp::setAppName(const EString &appname)
{
	s_pInstance->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	if (s_pInstance->m_sAppName.empty())
		s_pInstance->m_sAppName = s_pInstance->m_sTitle;
	return s_pInstance->m_sAppName;
}

void e2d::EApp::setBkColor(EColor color)
{
	get()->m_ClearColor = color;
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

LONGLONG e2d::EApp::getTotalDurationFromStart()
{
	return GetInterval(s_tStart);
}

void e2d::EApp::hideWindow()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::showWindow()
{
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
}

void e2d::EApp::quit()
{
	get()->m_bEnd = true;
}

void e2d::EApp::end()
{
	get()->m_bEnd = true;
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
			SafeReleaseAndClear(&m_pCurrentScene);
		}
	}

	// ִ����һ������ onEnter ����
	m_pNextScene->onEnter();

	m_pCurrentScene = m_pNextScene;		// �л�����
	m_pNextScene = nullptr;				// ��һ�����ÿ�
}

// Creates resources that are not bound to a particular device.
// Their lifetime effectively extends for the duration of the
// application.
HRESULT e2d::EApp::_createDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &GetFactory());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create Device Independent Resources Failed!", L"Error", MB_OK);
	}

	return hr;
}

// Creates resources that are bound to a particular
// Direct3D device. These resources need to be recreated
// if the Direct3D device dissapears, such as when the isVisiable
// changes, the window is remoted, etc.
HRESULT e2d::EApp::_createDeviceResources()
{
	// ����������Զ������豸�����Դ
	GetRenderTarget();

	return S_OK;
}

// Discards device-dependent resources. These resources must be
// recreated when the Direct3D device is lost.
void e2d::EApp::_discardDeviceResources()
{
	SafeReleaseInterface(&GetRenderTarget());
}

//  If the application receives a WM_SIZE message, this method
//  re2d::ESizes the render target appropriately.
void e2d::EApp::_onResize(UINT32 width, UINT32 height)
{
	if (GetRenderTarget())
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		GetRenderTarget()->Resize(D2D1::SizeU(width, height));
	}
}

// Handles window messages.
LRESULT e2d::EApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		// ��ȡ���� WM_CREATE ��Ϣ�� EApp ʵ������ָ��
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		e2d::EApp *pEApp = (e2d::EApp *)pcs->lpCreateParams;
		// ���� EApp ָ�뵽 GWLP_USERDATA �ֶ�
		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(pEApp)
		);

		result = 1;
	}
	else
	{
		// �� GWLP_USERDATA �ֶ�ȡ�� EApp ָ��
		e2d::EApp *pEApp = reinterpret_cast<e2d::EApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hWnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pEApp)
		{
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
				if (!pEApp->m_bTransitional && !pEApp->m_pNextScene)
				{
					EMsgManager::MouseProc(message, wParam, lParam);
				}
			}
			result = 0;
			wasHandled = true;
			break;

			// ��������Ϣ
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				// ִ�г����л�ʱ���ΰ����������Ϣ
				if (!pEApp->m_bTransitional && !pEApp->m_pNextScene)
				{
					EMsgManager::KeyboardProc(message, wParam, lParam);
				}
			}
			result = 0;
			wasHandled = true;
			break;

			// �����ڴ�С�仯��Ϣ
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pEApp->_onResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			// ����ֱ��ʱ仯��Ϣ
			case WM_DISPLAYCHANGE:
			{
				// �ػ�ͻ���
				InvalidateRect(hWnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			// �ػ洰��
			case WM_PAINT:
			{
				pEApp->_onRender();
				ValidateRect(hWnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			// ���ڼ�����Ϣ
			case WM_ACTIVATE:
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					if (pEApp->getCurrentScene() && 
						pEApp->getCurrentScene()->onInactive() &&
						pEApp->onInactive())
					{
						pEApp->m_bPaused = true;
					}
				}
				else
				{
					if (pEApp->getCurrentScene() && 
						pEApp->getCurrentScene()->onActivate() &&
						pEApp->onActivate())
					{
						pEApp->m_bPaused = false;
					}
				}
			}
			result = 1;
			wasHandled = true;
			break;

			// ���ڹر���Ϣ
			case WM_CLOSE:
			{
				if (!pEApp->getCurrentScene())
				{
					if (pEApp->onCloseWindow())
					{
						DestroyWindow(hWnd);
					}
				}
				else 
				{
					if (pEApp->getCurrentScene()->onCloseWindow() &&
						pEApp->onCloseWindow())
					{
						DestroyWindow(hWnd);
					}
				}
			}
			result = 1;
			wasHandled = true;
			break;

			// ���ڱ�����
			case WM_DESTROY:
			{
				// �˳�����
				pEApp->quit();
				// �����˳���Ϣ
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}
