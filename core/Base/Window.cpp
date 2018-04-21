#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// ���ھ��
static HWND s_HWnd = nullptr;
// �Ƿ�򿪿���̨
static bool s_bShowConsole = false;


bool e2d::Window::__init()
{
	// ע�ᴰ����
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"Easy2DApp";
	wcex.hIcon = NULL;

	RegisterClassEx(&wcex);

	// ��Ϊ CreateWindow ����ʹ�õ������ش�С����ȡϵͳ�� DPI ��ʹ��
	// ��Ӧ��������
	float dpiX = Renderer::getDpiScaleX();
	float dpiY = Renderer::getDpiScaleY();

	UINT nWidth = static_cast<UINT>(ceil(640 * dpiX / 96.f));
	UINT nHeight = static_cast<UINT>(ceil(480 * dpiY / 96.f));

	// ��ȡ��Ļ�ֱ���
	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));
	// ������Ĵ��ڴ�С�ȷֱ��ʴ�ʱ����������
	WARN_IF(screenWidth < nWidth || screenHeight < nHeight, "The window is larger than screen!");
	// ȡ��Сֵ
	nWidth = min(nWidth, screenWidth);
	nHeight = min(nHeight, screenHeight);

	// ���㴰�ڴ�С
	DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX;
	RECT wr = { 0, 0, static_cast<LONG>(nWidth), static_cast<LONG>(nHeight) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// ��ȡ�µĿ��
	nWidth = wr.right - wr.left;
	nHeight = wr.bottom - wr.top;

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
		// �����Ƿ���ڿ���̨
		HWND hwnd = ::GetConsoleWindow();
		if (hwnd)
		{
			// ���ÿ���̨�رհ�ť
			HMENU hmenu = ::GetSystemMenu(hwnd, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			// Ĭ�����ؿ���̨
			if (!s_bShowConsole)
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}
	}
	else
	{
		::UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		::MessageBox(nullptr, L"Create Window Failed!", L"Error", MB_OK);
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

void e2d::Window::setSize(UINT32 width, UINT32 height)
{
	// ���㴰�ڴ�С
	DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// ��ȡ�µĿ��
	width = wr.right - wr.left;
	height = wr.bottom - wr.top;
	// ��ȡ��Ļ�ֱ���
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::Window::setTitle(String title)
{
	// ���ô��ڱ���
	::SetWindowText(s_HWnd, title);
}

void e2d::Window::setIcon(LPCTSTR pIconID)
{
	
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	HICON hIcon = (HICON)::LoadImage(hInstance, pIconID, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	// ���ô��ڵ�ͼ��
	::SendMessage(s_HWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessage(s_HWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

e2d::String e2d::Window::getTitle()
{
	wchar_t wszTitle[MAX_PATH] = { 0 };
	::GetWindowText(s_HWnd, wszTitle, MAX_PATH);
	return wszTitle;
}

void e2d::Window::showConsole(bool show /* = true */)
{
	s_bShowConsole = show;
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
				MessageBox(nullptr, L"Alloc Console Failed!", L"Error", MB_OK);
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

void e2d::Window::setTypewritingEnable(bool bEnable)
{
	static HIMC hImc = nullptr;

	if (bEnable)
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


LRESULT e2d::Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

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
	break;

	// �ػ洰��
	case WM_PAINT:
	{
		e2d::Renderer::__render();
		ValidateRect(hWnd, NULL);
	}
	result = 0;
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
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	result = 1;
	break;

	default:
	{
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	}

	return result;
}