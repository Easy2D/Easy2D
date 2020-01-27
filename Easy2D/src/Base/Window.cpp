#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>
#include <easy2d/e2dnode.h>
#include <Windowsx.h>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// ���ھ��
static HWND s_HWnd = nullptr;
static easy2d::Window::Cursor s_currentCursor = easy2d::Window::Cursor::Normal;


bool easy2d::Window::__init(const String& title, int nWidth, int nHeight)
{
	// ע�ᴰ����
	WNDCLASSEX wcex		= { 0 };
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpszClassName	= L"Easy2DApp";
	wcex.hIcon			= nullptr;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= Window::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(LONG_PTR);
	wcex.hInstance		= HINST_THISCOMPONENT;
	wcex.hbrBackground	= nullptr;
	wcex.lpszMenuName	= nullptr;
	wcex.hCursor		= ::LoadCursorW(HINST_THISCOMPONENT, IDC_ARROW);

	RegisterClassEx(&wcex);

	// ���㴰�ڴ�С
	DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX &~ WS_THICKFRAME;
	RECT wr = { 0, 0, static_cast<LONG>(nWidth), static_cast<LONG>(nHeight) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// ��ȡ�µĿ��
	nWidth = static_cast<int>(wr.right - wr.left);
	nHeight = static_cast<int>(wr.bottom - wr.top);

	// ��ȡ��Ļ�ֱ���
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	// ��������
	s_HWnd = ::CreateWindowEx(
		NULL,
		L"Easy2DApp",
		title.c_str(),
		dwStyle,
		(screenWidth - nWidth) / 2, (screenHeight - nHeight) / 2, 
		nWidth, nHeight,
		nullptr,
		nullptr,
		HINST_THISCOMPONENT,
		nullptr
	);

	HRESULT hr = s_HWnd ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// �������뷨
		Window::setTypewritingEnable(false);
		// ���ÿ���̨�رհ�ť
		HWND consoleHWnd = ::GetConsoleWindow();
		if (consoleHWnd)
		{
			HMENU hmenu = ::GetSystemMenu(consoleHWnd, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
	else
	{
		::UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		Window::error(L"Create Window Failed!");
	}

	return SUCCEEDED(hr);
}

void easy2d::Window::__uninit()
{
	// �رտ���̨
	if (::GetConsoleWindow())
	{
		::FreeConsole();
	}
	// �رմ���
	if (s_HWnd)
	{
		::DestroyWindow(s_HWnd);
		s_HWnd = nullptr;
	}
}

void easy2d::Window::__poll()
{
	static MSG msg;

	while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void easy2d::Window::__updateCursor()
{
	LPCWSTR pCursorName = nullptr;
	switch (s_currentCursor)
	{
	case Cursor::Normal:
		pCursorName = IDC_ARROW;
		break;

	case Cursor::Hand:
		pCursorName = IDC_HAND;
		break;

	case Cursor::No:
		pCursorName = IDC_NO;
		break;

	case Cursor::Wait:
		pCursorName = IDC_WAIT;
		break;

	case Cursor::ArrowWait:
		pCursorName = IDC_APPSTARTING;
		break;

	default:
		break;
	}

	if (pCursorName)
	{
		HCURSOR hCursor = ::LoadCursor(nullptr, pCursorName);
		::SetCursor(hCursor);
	}
}

float easy2d::Window::getWidth()
{
	return getSize().width;
}

float easy2d::Window::getHeight()
{
	return getSize().height;
}

easy2d::Size easy2d::Window::getSize()
{
	if (s_HWnd)
	{
		// ��ȡ�ͻ�����С
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return Size(float(rcClient.right - rcClient.left), float(rcClient.bottom - rcClient.top));
	}
	return Size();
}

HWND easy2d::Window::getHWnd()
{
	return s_HWnd;
}

void easy2d::Window::setSize(int width, int height)
{
	// ���㴰�ڴ�С
	DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX &~ WS_THICKFRAME;
	RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// ��ȡ�µĿ��
	width = static_cast<int>(wr.right - wr.left);
	height = static_cast<int>(wr.bottom - wr.top);
	// ��ȡ��Ļ�ֱ���
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// ������Ĵ��ڴ�С�ȷֱ��ʴ�ʱ����������
	if (screenWidth < width || screenHeight < height)
		E2D_WARNING(L"The window is larger than screen!");
	// ȡ��Сֵ
	width = min(width, screenWidth);
	height = min(height, screenHeight);
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void easy2d::Window::setTitle(const String& title)
{
	// ���ô��ڱ���
	::SetWindowText(s_HWnd, title.c_str());
}

void easy2d::Window::setIcon(int iconID)
{
	HINSTANCE hInstance = ::GetModuleHandle(nullptr);
	HICON hIcon = (HICON)::LoadImage(hInstance, MAKEINTRESOURCE(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	// ���ô��ڵ�ͼ��
	::SendMessage(s_HWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessage(s_HWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

void easy2d::Window::setCursor(Cursor cursor)
{
	s_currentCursor = cursor;
}

easy2d::String easy2d::Window::getTitle()
{
	wchar_t wszTitle[MAX_PATH] = { 0 };
	::GetWindowText(s_HWnd, wszTitle, MAX_PATH);
	return wszTitle;
}

void easy2d::Window::setTypewritingEnable(bool enable)
{
	static HIMC hImc = nullptr;

	if (enable)
	{
		if (hImc != nullptr)
		{
			::ImmAssociateContext(Window::getHWnd(), hImc);
			hImc = nullptr;
		}
	}
	else
	{
		if (hImc == nullptr)
		{
			hImc = ::ImmAssociateContext(Window::getHWnd(), nullptr);
		}
	}
}

void easy2d::Window::info(const String & text, const String & title)
{
	::MessageBox(s_HWnd, text.c_str(), title.c_str(), MB_ICONINFORMATION | MB_OK);
	Game::reset();
}

void easy2d::Window::warning(const String& title, const String& text)
{
	::MessageBox(s_HWnd, text.c_str(), title.c_str(), MB_ICONWARNING | MB_OK);
	Game::reset();
}

void easy2d::Window::error(const String & text, const String & title)
{
	::MessageBox(s_HWnd, text.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
	Game::reset();
}


LRESULT easy2d::Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	bool hasHandled = false;

	switch (message)
	{

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		KeyDownEvent evt(KeyCode::Value(wParam), int(lParam & 0xFF));
		SceneManager::dispatch(&evt);
	}
	break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		KeyUpEvent evt(KeyCode::Value(wParam), int(lParam & 0xFF));
		SceneManager::dispatch(&evt);
	}
	break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	{
		MouseCode::Value btn = MouseCode::Left;
		if (message == WM_LBUTTONUP) { btn = MouseCode::Left; }
		else if (message == WM_RBUTTONUP) { btn = MouseCode::Right; }
		else if (message == WM_MBUTTONUP) { btn = MouseCode::Middle; }

		MouseUpEvent evt(
			static_cast<float>(GET_X_LPARAM(lParam)),
			static_cast<float>(GET_Y_LPARAM(lParam)),
			btn
		);
		SceneManager::dispatch(&evt);
	}
	break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		MouseCode::Value btn = MouseCode::Left;
		if (message == WM_LBUTTONDOWN) { btn = MouseCode::Left; }
		else if (message == WM_RBUTTONDOWN) { btn = MouseCode::Right; }
		else if (message == WM_MBUTTONDOWN) { btn = MouseCode::Middle; }

		MouseDownEvent evt(
			static_cast<float>(GET_X_LPARAM(lParam)),
			static_cast<float>(GET_Y_LPARAM(lParam)),
			btn
		);
		SceneManager::dispatch(&evt);
	}
	break;

	case WM_MOUSEMOVE:
	{
		MouseMoveEvent evt(
			static_cast<float>(GET_X_LPARAM(lParam)),
			static_cast<float>(GET_Y_LPARAM(lParam))
		);
		SceneManager::dispatch(&evt);
	}
	break;

	case WM_MOUSEWHEEL:
	{
		MouseWheelEvent evt(
			static_cast<float>(GET_X_LPARAM(lParam)),
			static_cast<float>(GET_Y_LPARAM(lParam)),
			GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA
		);
		SceneManager::dispatch(&evt);
	}
	break;

	// �����ڴ�С�仯��Ϣ
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		// ���������յ�һ�� WM_SIZE ��Ϣ�����������������Ⱦ
		// Ŀ���ʵ��������ܻ����ʧ�ܣ�����������Ժ����п��ܵ�
		// ������Ϊ�����������һ�ε��� EndDraw ʱ����
		auto pRT = Renderer::getRenderTarget();
		if (pRT) pRT->Resize(D2D1::SizeU(width, height));
	}
	break;

	// ����ֱ��ʱ仯��Ϣ
	case WM_DISPLAYCHANGE:
	{
		// �ػ�ͻ���
		InvalidateRect(hWnd, nullptr, FALSE);
	}
	result = 0;
	hasHandled = true;
	break;

	// �ػ洰��
	case WM_PAINT:
	{
		easy2d::Renderer::__render();
		ValidateRect(hWnd, nullptr);
	}
	result = 0;
	hasHandled = true;
	break;

	case WM_SETCURSOR:
	{
		Window::__updateCursor();
	}
	break;

	// ���ڹر���Ϣ
	case WM_CLOSE:
	{
		easy2d::Scene * pCurrentScene = easy2d::SceneManager::getCurrentScene();
		if (!pCurrentScene || pCurrentScene->onCloseWindow())
		{
			easy2d::Game::quit();
		}
	}
	result = 0;
	hasHandled = true;
	break;

	// ����������Ϣ
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	result = 1;
	hasHandled = true;
	break;

	}

	if (!hasHandled)
	{
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	return result;
}