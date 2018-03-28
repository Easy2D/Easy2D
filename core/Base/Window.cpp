#include "..\ebase.h"
#include "..\emanagers.h"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// ���ھ��
static HWND s_HWnd = nullptr;
// �Ƿ�򿪿���̨
static bool s_bShowConsole = false;


bool e2d::Window::__init(String& sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID /*= nullptr*/)
{
	if (!Window::__initMutex(sTitle))
	{
		return false;
	}

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
	// ���ó���ͼ��
	if (pIconID)
	{
		wcex.hIcon = (HICON)::LoadImage(
			GetModuleHandle(NULL),
			pIconID,
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}

	RegisterClassEx(&wcex);

	// ��Ϊ CreateWindow ����ʹ�õ������ش�С����ȡϵͳ�� DPI ��ʹ��
	// ��Ӧ��������
	float dpiX, dpiY;

	// ���������ص�ǰ��ϵͳ DPI�����ֵҲ��������������
	Renderer::getID2D1Factory()->GetDesktopDpi(&dpiX, &dpiY);

	nWidth = static_cast<UINT>(ceil(nWidth * dpiX / 96.f));
	nHeight = static_cast<UINT>(ceil(nHeight * dpiY / 96.f));

	// ��ȡ��Ļ�ֱ���
	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));
	// ������Ĵ��ڴ�С�ȷֱ��ʴ�ʱ����������
	WARN_IF(screenWidth < nWidth || screenHeight < nHeight, "The window is larger than screen!");
	// ȡ��Сֵ
	nWidth = min(nWidth, screenWidth);
	nHeight = min(nHeight, screenHeight);

	// ��������
	s_HWnd = CreateWindow(
		L"Easy2DApp",
		sTitle,
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, nWidth, nHeight,
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
		UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create Window Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

bool e2d::Window::__initMutex(String& sTitle)
{
	// �������̻�����
	HANDLE m_hMutex = ::CreateMutex(NULL, TRUE, L"Easy2DApp-" + sTitle);

	if (m_hMutex == nullptr)
	{
		WARN_IF(true, "CreateMutex Failed!");
		return true;
	}

	// ��������Ѿ����ڲ�����������
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// ��ȡ���ھ��
		HWND hProgramWnd = ::FindWindow(L"Easy2DApp", sTitle);
		if (hProgramWnd)
		{
			// ��ȡ������ʾ״̬
			WINDOWPLACEMENT wpm;
			::GetWindowPlacement(hProgramWnd, &wpm);
			// �����еĳ��򴰿ڻ�ԭ������״̬
			wpm.showCmd = SW_SHOW;
			::SetWindowPlacement(hProgramWnd, &wpm);
			::SetWindowPos(hProgramWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}

		// �رս��̻�����
		CloseHandle(m_hMutex);
		m_hMutex = nullptr;
		return false;
	}

	return true;
}

void e2d::Window::__uninit()
{
	// �رտ���̨
	if (::GetConsoleWindow())
	{
		::FreeConsole();
	}
	// �رմ���
	DestroyWindow(s_HWnd);
	s_HWnd = nullptr;
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
	return Renderer::getRenderTarget()->GetSize().width;
}

double e2d::Window::getHeight()
{
	return Renderer::getRenderTarget()->GetSize().height;
}

e2d::Size e2d::Window::getSize()
{
	D2D1_SIZE_F size = Renderer::getRenderTarget()->GetSize();
	return Size(size.width, size.height);
}

HWND e2d::Window::getHWnd()
{
	return s_HWnd;
}

void e2d::Window::setSize(UINT32 width, UINT32 height)
{
	// ��ȡ��Ļ�ֱ���
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// ��ȡ���ڴ�С�������˵�����
	tagRECT rcWindow;
	::GetWindowRect(s_HWnd, &rcWindow);
	// ��ȡ�ͻ�����С
	tagRECT rcClient;
	::GetClientRect(s_HWnd, &rcClient);
	// ����߿��С
	width += (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	height += (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	// �޸Ĵ��ڴ�С�������ô�������Ļ����
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::Window::setTitle(String&title)
{
	// ���ô��ڱ���
	::SetWindowText(s_HWnd, title);
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
		Renderer::getRenderTarget()->Resize(D2D1::SizeU(width, height));
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
			DestroyWindow(hWnd);
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