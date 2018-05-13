#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// ���ھ��
static HWND s_HWnd = nullptr;


bool e2d::Window::__init()
{
	// ע�ᴰ����
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpszClassName = L"Easy2DApp";
	wcex.hIcon = NULL;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wcex);

	// ��Ϊ CreateWindow ����ʹ�õ������ش�С����ȡϵͳ�� DPI ��ʹ��
	// ��Ӧ��������
	float dpiX = Renderer::getDpiScaleX();
	float dpiY = Renderer::getDpiScaleY();

	int nWidth = static_cast<int>(ceil(640 * dpiX / 96.f));
	int nHeight = static_cast<int>(ceil(480 * dpiY / 96.f));

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
		L"Easy2D Game",
		dwStyle,
		(screenWidth - nWidth) / 2, (screenHeight - nHeight) / 2, 
		nWidth, nHeight,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		NULL
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

void e2d::Window::__uninit()
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

void e2d::Window::__poll()
{
	static MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

double e2d::Window::getWidth()
{
	if (s_HWnd)
	{
		// ��ȡ�ͻ�����С
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return rcClient.right - rcClient.left;
	}
	return 0;
}

double e2d::Window::getHeight()
{
	if (s_HWnd)
	{
		// ��ȡ�ͻ�����С
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return rcClient.bottom - rcClient.top;
	}
	return 0;
}

e2d::Size e2d::Window::getSize()
{
	if (s_HWnd)
	{
		// ��ȡ�ͻ�����С
		tagRECT rcClient;
		::GetClientRect(s_HWnd, &rcClient);
		return Size(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	}
	return Size();
}

HWND e2d::Window::getHWnd()
{
	return s_HWnd;
}

void e2d::Window::setSize(int width, int height)
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
	WARN_IF(screenWidth < width || screenHeight < height, "The window is larger than screen!");
	// ȡ��Сֵ
	width = min(width, screenWidth);
	height = min(height, screenHeight);
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::Window::setTitle(const String& title)
{
	// ���ô��ڱ���
	::SetWindowText(s_HWnd, title);
}

void e2d::Window::setIcon(int iconID)
{
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	HICON hIcon = (HICON)::LoadImage(hInstance, MAKEINTRESOURCE(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	// ���ô��ڵ�ͼ��
	::SendMessage(s_HWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessage(s_HWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

void e2d::Window::setCursor(Cursor cursor)
{
	LPCWSTR pCursorName = NULL;
	switch (cursor)
	{
	case Cursor::NORMAL:
		pCursorName = IDC_ARROW;
		break;

	case Cursor::HAND:
		pCursorName = IDC_HAND;
		break;

	case Cursor::NO:
		pCursorName = IDC_NO;
		break;

	case Cursor::WAIT:
		pCursorName = IDC_WAIT;
		break;

	case Cursor::ARROW_WAIT:
		pCursorName = IDC_APPSTARTING;
		break;

	default:
		break;
	}

	HCURSOR hCursor = ::LoadCursor(NULL, pCursorName);
	::SetCursor(hCursor);
}

e2d::String e2d::Window::getTitle()
{
	wchar_t wszTitle[MAX_PATH] = { 0 };
	::GetWindowText(s_HWnd, wszTitle, MAX_PATH);
	return wszTitle;
}

void e2d::Window::showConsole(bool show)
{
	// �����Ѵ��ڵĿ���̨���
	HWND hwnd = ::GetConsoleWindow();
	// �رտ���̨
	if (show)
	{
		if (hwnd)
		{
			::ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		else
		{
			// ��ʾһ���¿���̨
			if (::AllocConsole())
			{
				hwnd = ::GetConsoleWindow();
				// �ض����������
				FILE * stdoutStream, * stdinStream, * stderrStream;
				freopen_s(&stdoutStream, "conout$", "w+t", stdout);
				freopen_s(&stdinStream, "conin$", "r+t", stdin);
				freopen_s(&stderrStream, "conout$", "w+t", stderr);
				// ���ÿ���̨�رհ�ť
				HMENU hmenu = ::GetSystemMenu(hwnd, FALSE);
				::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			}
			else
			{
				Window::error(L"Alloc Console Failed!");
			}
		}
	}
	else 
	{
		if (hwnd)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}
	}
}

void e2d::Window::setTypewritingEnable(bool enable)
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

void e2d::Window::info(const String & text, const String & title)
{
	::MessageBox(s_HWnd, text, title, MB_ICONINFORMATION | MB_OK);
	Game::reset();
}

void e2d::Window::warning(const String& title, const String& text)
{
	::MessageBox(s_HWnd, text, title, MB_ICONWARNING | MB_OK);
	Game::reset();
}

void e2d::Window::error(const String & text, const String & title)
{
	::MessageBox(s_HWnd, text, title, MB_ICONERROR | MB_OK);
	Game::reset();
}


LRESULT e2d::Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	bool hasHandled = false;

	switch (message)
	{

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
		InvalidateRect(hWnd, NULL, FALSE);
	}
	result = 0;
	hasHandled = true;
	break;

	// �ػ洰��
	case WM_PAINT:
	{
		e2d::Renderer::__render();
		ValidateRect(hWnd, NULL);
	}
	result = 0;
	hasHandled = true;
	break;

	// ���ڹر���Ϣ
	case WM_CLOSE:
	{
		e2d::Scene * pCurrentScene = e2d::SceneManager::getCurrentScene();
		if (!pCurrentScene || pCurrentScene->onCloseWindow())
		{
			e2d::Game::quit();
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