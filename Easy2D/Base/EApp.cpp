#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emsg.h"
#include "..\etools.h"
#include "..\enodes.h"
#include <stack>
#include <thread>
#include <imm.h>  
#pragma comment (lib ,"imm32.lib")

using namespace std::this_thread;
using namespace std::chrono;


// Ψһʵ��ָ��
e2d::EApp * s_pInstance = nullptr;
// ����ջ
std::stack<e2d::EScene*> s_SceneStack;

e2d::EApp::EApp()
	: m_bRunning(false)
	, nAnimationInterval(17LL)
	, m_ClearColor(EColor::Black)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
{
	ASSERT(s_pInstance == nullptr, "EApp instance already exists!");
	s_pInstance = this;		// ����ʵ������

	CoInitialize(NULL);
}

e2d::EApp::~EApp()
{
	// �ͷ���Դ
	SafeReleaseInterface(&GetRenderTarget());
	SafeReleaseInterface(&GetFactory());

	CoUninitialize();
}

e2d::EApp * e2d::EApp::get()
{
	ASSERT(s_pInstance != nullptr, "Nonexistent EApp instance.");
	return s_pInstance;		// ��ȡ EApp ��Ψһʵ��
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, bool showConsole /* = false */)
{
	return init(title, width, height, WS_OVERLAPPEDWINDOW, showConsole);
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, int windowStyle, bool showConsole /* = false */)
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
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = EApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"Easy2DApp";

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
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		// ������Ļ���еľ���
		RECT rtWindow;
		rtWindow.left = (screenWidth - width) / 2;
		rtWindow.top = (screenHeight - height) / 2;
		rtWindow.right = rtWindow.left + width;
		rtWindow.bottom = rtWindow.top + height;
		// ����ͻ�����С
		AdjustWindowRectEx(&rtWindow, windowStyle, FALSE, 0L);
		// ���洰������
		m_sTitle = title;
		// ��������
		GetHWnd() = CreateWindow(
			L"Easy2DApp",
			m_sTitle.c_str(),
			windowStyle,
			rtWindow.left,
			rtWindow.top,
			rtWindow.right - rtWindow.left,
			rtWindow.bottom - rtWindow.top,
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
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");
	// ��ʾ����
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// ������Ϸ
	m_bRunning = true;

	MSG msg;

	while (m_bRunning)
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
	// �ͷ������ڴ���Դ
	this->free();
}

void e2d::EApp::setFPS(UINT32 fps)
{
	fps = min(max(fps, 30), 120);
	nAnimationInterval = 1000 / fps;
}

bool e2d::EApp::onExit()
{
	return true;
}

void e2d::EApp::_mainLoop()
{
	// ʱ����
	static LONGLONG nInterval = 0LL;
	// ����ʱ��
	static LONGLONG nWaitMS = 0L;
	// ˢ�¼�ʱ
	static steady_clock::time_point tLast = steady_clock::now();

	GetNow() = steady_clock::now();
	// ����ʱ����
	nInterval = GetInterval(tLast);
	// �жϼ��ʱ���Ƿ��㹻
	if (nInterval >= nAnimationInterval)
	{
		// ��¼��ǰʱ��
		tLast = GetNow();
		// ��Ϸ��������
		_onControl();
		// ˢ����Ϸ����
		if (!_onRender())
		{
			MessageBox(GetHWnd(), L"Game Render Failed!", L"Error", MB_OK);
			this->quit();
		}
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
	// ��һ����ָ�벻Ϊ��ʱ���л�����
	if (m_pNextScene)
	{
		// ������һ����
		_enterNextScene();
	}
	// ���Ե�ǰ�����ǿ�
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");

	ETimerManager::TimerProc();		// ��ʱ��ִ�г���
	//ActionManager::__exec();	// ����������ִ�г���
	EObjectManager::__flush();	// ˢ���ڴ��
}

// This method discards device-specific
// resources if the Direct3D device dissapears during execution and
// recreates the resources the next time it's invoked.
bool e2d::EApp::_onRender()
{
	HRESULT hr = S_OK;

	hr = _createDeviceResources();

	if (SUCCEEDED(hr))
	{
		GetRenderTarget()->BeginDraw();
		// ʹ�ñ���ɫ�����Ļ
		GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));
		// ���Ƶ�ǰ����
		m_pCurrentScene->_onRender();
		
		hr = GetRenderTarget()->EndDraw();
		// ˢ�½���
		UpdateWindow(GetHWnd());
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		_discardDeviceResources();
	}

	return SUCCEEDED(hr);
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

UINT32 e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetPixelSize().width;
}

UINT32 e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetPixelSize().height;
}

void e2d::EApp::enterScene(EScene * scene, bool save /* = true */)
{
	// ������һ������ָ��
	get()->m_pNextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = save;
	}
}

void e2d::EApp::backScene()
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
}

void e2d::EApp::clearScene()
{
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeDelete(&temp);
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

void e2d::EApp::closeWindow()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::showWindow()
{
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
}

void e2d::EApp::free()
{
	// �ͷų����ڴ�
	SafeDelete(&m_pCurrentScene);
	SafeDelete(&m_pNextScene);
	// ��ճ���ջ
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeDelete(&temp);
		s_SceneStack.pop();
	}
	// ɾ��ͼƬ����
	ESprite::clearCache();
	// ɾ�����ж�ʱ�����������Ͷ���
	ETimerManager::clearAllTimers();
	EMsgManager::clearAllKeyboardListeners();
	EMsgManager::clearAllMouseListeners();
	//ActionManager::clearAllActions();
	// ɾ�����ж���
	EObjectManager::clearAllObjects();
}

void e2d::EApp::quit()
{
	get()->m_bRunning = false;
}

void e2d::EApp::end()
{
	get()->m_bRunning = false;
}

void e2d::EApp::_enterNextScene()
{
	// ִ�е�ǰ������ onExit ����
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();
		m_pCurrentScene->_onExit();

		if (m_pCurrentScene->m_bWillSave)
		{
			// ��Ҫ���浱ǰ��������������ջ��
			s_SceneStack.push(m_pCurrentScene);
		}
		else
		{
			SafeDelete(&m_pCurrentScene);
		}
	}

	// ִ����һ������ onEnter ����
	m_pNextScene->_onEnter();
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
				EMsgManager::MouseProc(message, wParam, lParam);
			}
			result = 0;
			wasHandled = true;
			break;

			// ��������Ϣ
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				EMsgManager::KeyboardProc(message, wParam, lParam);
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

			// ���ڹر���Ϣ
			case WM_CLOSE:
			{
				if (pEApp->onExit())
				{
					DestroyWindow(hWnd);
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
