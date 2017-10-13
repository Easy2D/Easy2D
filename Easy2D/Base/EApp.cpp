#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emsg.h"
#include "..\etools.h"
#include <stack>
#include <chrono>
#include <thread>
using namespace std::chrono;
using namespace std::this_thread;


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


e2d::EApp * s_pInstance = nullptr;
std::stack<e2d::EScene*> s_SceneStack;

e2d::EApp::EApp()
	: m_bRunning(false)
	, m_ClearColor(EColor::Black)
	, m_bSaveScene(true)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
	, m_pLoadingScene(nullptr)
{
	s_pInstance = this;		// ����ʵ������
}

e2d::EApp::~EApp()
{
	SafeReleaseInterface(&GetFactory());
	SafeReleaseInterface(&GetRenderTarget());
}

e2d::EApp * e2d::EApp::get()
{
	ASSERT(s_pInstance != nullptr);
	return s_pInstance;		// ��ȡ EApp ��Ψһʵ��
}

bool e2d::EApp::init(e2d::EString title, e2d::ESize size, bool bShowConsole /* = false */)
{
	return init(title, size.cx, size.cy, bShowConsole);
}

bool e2d::EApp::init(e2d::EString title, UINT32 width, UINT32 height, bool bShowConsole /* = false */)
{
	HRESULT hr;
	hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		// �رտ���̨
		if (!bShowConsole)
		{
			HWND consoleHWnd = FindWindow(L"ConsoleWindowClass", NULL);

			if (consoleHWnd)
			{
				ShowWindow(consoleHWnd, SW_HIDE);
			}
		}

		// ��ʼ�� device-indpendent ��Դ
		// ���� Direct2D factory.
		hr = _createDeviceIndependentResources();
	}

	if (SUCCEEDED(hr))
	{
		// ע�ᴰ����
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = EApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"E2DApp";

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		GetFactory()->GetDesktopDpi(&dpiX, &dpiY);

		m_sTitle = title;

		// Create the window.
		GetHWnd() = CreateWindow(
			L"E2DApp",
			m_sTitle.c_str(),
			WS_OVERLAPPEDWINDOW | CS_DBLCLKS,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(width * dpiX / 96.f)),
			static_cast<UINT>(ceil(height * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		hr = GetHWnd() ? S_OK : E_FAIL;
		if (FAILED(hr))
		{
			UnregisterClass(L"E2DApp", HINST_THISCOMPONENT);
			MessageBox(nullptr, L"Create Window Failed!", L"Error", MB_OK);
		}
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

// ������Ϸ
void e2d::EApp::run()
{
	ASSERT(m_pNextScene != nullptr);
	// �����һ������
	_enterNextScene();
	// ��ʾ����
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// ������Ϸ
	m_bRunning = true;

	MSG msg;

	while (m_bRunning)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// ��������Ϣ
			if (msg.message == WM_QUIT)
			{
				m_bRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// ִ����ѭ��
			_mainLoop();
		}
	}
	// ��Ϸ��������ִ��һ��ѭ��
	_mainLoop();
	// �رմ���
	close();
	// �ͷ������ڴ�ռ��
	free();
}

void e2d::EApp::_mainLoop()
{
	static steady_clock::time_point nNow;
	static steady_clock::time_point nLast = steady_clock::now();
	// ֡���
	static LONGLONG nAnimationInterval = 17LL;
	// ʱ����
	static LONGLONG nInterval = 0LL;
	// ����ʱ��
	static LONGLONG nWaitMS = 0L;

	// ˢ�¼�ʱ
	nNow = steady_clock::now();
	// ����ʱ����
	nInterval = duration_cast<milliseconds>(nNow - nLast).count();
	// �жϼ��ʱ���Ƿ��㹻
	if (nInterval >= nAnimationInterval)
	{
		// ��¼��ǰʱ��
		nLast = nNow;
		// ִ����Ϸ����
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
			std::this_thread::sleep_for(milliseconds(nWaitMS));
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
	ASSERT(m_pCurrentScene != nullptr);

	//MouseMsg::__exec();			// �����
	//KeyMsg::__exec();			// ���̰������
	//Timer::__exec();			// ��ʱ��ִ�г���
	//ActionManager::__exec();	// ����������ִ�г���
	EObjectManager::__flush();		// ˢ���ڴ��
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

		GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

		GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));

		m_pCurrentScene->_onRender();	// ���Ƶ�ǰ����

		hr = GetRenderTarget()->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		_discardDeviceResources();
	}

	return SUCCEEDED(hr);
}

void e2d::EApp::setWindowSize(int width, int height)
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
	// ���ٵ�ǰ����
	// DestroyWindow(m_);
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	SetWindowPos(
		GetHWnd(), 
		HWND_TOP,
		(screenWidth - width) / 2,
		(screenHeight - height) / 2,
		width,
		height,
		SWP_SHOWWINDOW
	);
}

void e2d::EApp::setWindowSize(e2d::ESize size)
{
	setWindowSize(size.cx, size.cy);
}

void e2d::EApp::setWindowTitle(e2d::EString title)
{
	// ���ô��ڱ���
	SetWindowText(GetHWnd(), title.c_str());
	// ���浱ǰ���⣬�����޸Ĵ��ڴ�Сʱ�ָ�����
	m_sTitle = title;
}

e2d::EString e2d::EApp::getTitle()
{
	return m_sTitle;
}

e2d::ESize e2d::EApp::getSize()
{
	return e2d::ESize(GetRenderTarget()->GetPixelSize().width, GetRenderTarget()->GetPixelSize().height);
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
	m_pNextScene = scene;
	// �л�����ʱ���Ƿ񱣴浱ǰ����
	m_bSaveScene = save;
}

void e2d::EApp::backScene()
{
	// ��ջ��ȡ������ָ�룬��Ϊ��һ����
	m_pNextScene = s_SceneStack.top();
	// �����浱ǰ����
	m_bSaveScene = false;
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
	return m_pCurrentScene;
}

e2d::EScene * e2d::EApp::getLoadingScene()
{
	return m_pLoadingScene;
}

void e2d::EApp::setLoadingScene(EScene * scene)
{
	m_pLoadingScene = scene;
}

void e2d::EApp::setAppName(e2d::EString appname)
{
	s_pInstance->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	return s_pInstance->m_sAppName;
}

void e2d::EApp::setBkColor(EColor::Enum color)
{
	m_ClearColor = color;
}

void e2d::EApp::close()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::show()
{
	ShowWindow(GetHWnd(), SW_NORMAL);
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
	// ɾ�����ж�ʱ��
	//Timer::clearAllTimers();
	//MouseMsg::clearAllListeners();
	//KeyMsg::clearAllListeners();
	//ActionManager::clearAllActions();
	// ɾ�����ж���
	//EObjectManager::__clearAllObjects();
}

void e2d::EApp::quit()
{
	m_bRunning = false;
}

void e2d::EApp::end()
{
	m_bRunning = false;
}

void e2d::EApp::_enterNextScene()
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
			//Timer::waitAllSceneTimers(m_pCurrentScene);
			//MouseMsg::waitAllSceneListeners(m_pCurrentScene);
			//KeyMsg::waitAllSceneListeners(m_pCurrentScene);
			//ActionManager::waitAllSceneActions(m_pCurrentScene);
		}
		else
		{
			// �����泡��ʱ��ֹͣ��ǰ���������е����ж�ʱ������ɾ����ǰ����
			//Timer::clearAllSceneTimers(m_pCurrentScene);
			//MouseMsg::clearAllSceneListeners(m_pCurrentScene);
			//KeyMsg::clearAllSceneListeners(m_pCurrentScene);
			//ActionManager::stopAllSceneActions(m_pCurrentScene);
			SafeDelete(&m_pCurrentScene);
		}
	}

	m_pCurrentScene = m_pNextScene;		// �л�����
	m_pNextScene = nullptr;				// ��һ�����ÿ�

	if (bBackScene)
	{
		// ������һ����ʱ���ָ������ϵĶ�ʱ��
		//Timer::notifyAllSceneTimers(m_pCurrentScene);
		EMsgManager::notifyAllListenersOnScene(m_pCurrentScene);
		//ActionManager::notifyAllSceneActions(m_pCurrentScene);
	}
	else
	{
		m_pCurrentScene->init();		// ����һ���³���ʱ��ִ������ init ����
	}

	m_pCurrentScene->onEnter();			// ִ����һ������ onEnter ����
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
	HRESULT hr = S_OK;

	if (!GetRenderTarget())
	{
		RECT rc;
		GetClientRect(GetHWnd(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = GetFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(GetHWnd(), size),
			&GetRenderTarget()
		);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create Device Resources Failed!", L"Error", MB_OK);
	}

	return hr;
}

// Discards device-dependent resources. These resources must be
// recreated when the Direct3D device is lost.
void e2d::EApp::_discardDeviceResources()
{
	SafeReleaseInterface(&GetRenderTarget());
}

//  If the application receives a WM_SIZE message, this method
//  re2d::ESizes the render target appropriately.
void e2d::EApp::_onResize(UINT width, UINT height)
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
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		e2d::EApp *pEApp = (e2d::EApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(pEApp)
		);

		result = 1;
	}
	else
	{
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
			/*case WM_ACTIVATE:
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					MSG msg;
					do
					{
						GetMessage(&msg, nullptr, 0, 0);
					} while (msg.wParam != WA_ACTIVE);
				}
			}*/
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
			break;

			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_CHAR:
			{
				EMsgManager::KeyboardProc(message, wParam, lParam);
			}
			result = 0;
			break;

			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pEApp->_onResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hWnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pEApp->_onRender();
				ValidateRect(hWnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
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
