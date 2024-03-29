#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>
#include <easy2d/e2dnode.h>
#include <Windowsx.h>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

// 窗口句柄
static HWND s_HWnd = nullptr;
static easy2d::Window::Cursor s_currentCursor = easy2d::Window::Cursor::Normal;

namespace easy2d
{
class CustomCursor
{
public:
	void update(Window::Cursor cursor)
	{
		if (_cursor != cursor)
		{
			_cursor = cursor;
			loadCursor();
		}
	}

	void loadCursor()
	{
		if (_cursorFunc)
		{
			auto newCursor = _cursorFunc(_cursor);
			if (newCursor != _cursorNode)
			{
				clear();
				_cursorNode = newCursor;
				if (_cursorNode)
				{
					_cursorNode->retain();
				}
			}
		}
		else
		{
			clear();
		}
	}

	void setCursorFunc(const Function<Node* (Window::Cursor)>& f)
	{
		_cursorFunc = f;
		_cursor = Window::Cursor(-1);
	}

	Node* getCursorNode() const
	{
		return _cursorNode;
	}

	void clear()
	{
		if (_cursorNode)
		{
			_cursorNode->release();
			_cursorNode = nullptr;
		}
	}

	operator bool() const
	{
		return _cursorNode != nullptr;
	}

private:
	Window::Cursor _cursor = Window::Cursor(-1);
	Function<Node* (Window::Cursor)> _cursorFunc = nullptr;
	Node* _cursorNode = nullptr;
};
}

static easy2d::CustomCursor s_customCursor;

bool easy2d::Window::__init(const String& title, int nWidth, int nHeight)
{
	// 注册窗口类
	WNDCLASSEXA wcex	= { 0 };
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpszClassName	= "Easy2DApp";
	wcex.hIcon			= nullptr;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= Window::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= HINST_THISCOMPONENT;
	wcex.hbrBackground	= nullptr;
	wcex.lpszMenuName	= nullptr;
	wcex.hCursor		= ::LoadCursor(HINST_THISCOMPONENT, IDC_ARROW);

	RegisterClassExA(&wcex);

	// 计算窗口大小
	DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX &~ WS_THICKFRAME;
	RECT wr = { 0, 0, static_cast<LONG>(nWidth), static_cast<LONG>(nHeight) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// 获取新的宽高
	nWidth = static_cast<int>(wr.right - wr.left);
	nHeight = static_cast<int>(wr.bottom - wr.top);

	// 获取屏幕分辨率
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	// 创建窗口
	s_HWnd = ::CreateWindowExA(
		NULL,
		"Easy2DApp",
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
		// 禁用输入法
		Window::setTypewritingEnable(false);
		// 禁用控制台关闭按钮
		HWND consoleHWnd = ::GetConsoleWindow();
		if (consoleHWnd)
		{
			HMENU hmenu = ::GetSystemMenu(consoleHWnd, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
	else
	{
		::UnregisterClassA("Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		Window::error("Create Window Failed!");
	}

	return SUCCEEDED(hr);
}

void easy2d::Window::__uninit()
{
	s_customCursor.clear();
	// 关闭控制台
	if (::GetConsoleWindow())
	{
		::FreeConsole();
	}
	// 关闭窗口
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
	s_customCursor.update(s_currentCursor);
	if (s_customCursor)
	{
		::SetCursor(NULL); // 保证不显示默认指针
		return;
	}

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
	else
	{
		::SetCursor(NULL);
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
		// 获取客户区大小
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
	// 计算窗口大小
	DWORD dwStyle = WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX &~ WS_THICKFRAME;
	RECT wr = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRectEx(&wr, dwStyle, FALSE, NULL);
	// 获取新的宽高
	width = static_cast<int>(wr.right - wr.left);
	height = static_cast<int>(wr.bottom - wr.top);
	// 获取屏幕分辨率
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// 当输入的窗口大小比分辨率大时，给出警告
	if (screenWidth < width || screenHeight < height)
		E2D_WARNING("The window is larger than screen!");
	// 取最小值
	width = min(width, screenWidth);
	height = min(height, screenHeight);
	// 修改窗口大小，并设置窗口在屏幕居中
	::MoveWindow(s_HWnd, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void easy2d::Window::setTitle(const String& title)
{
	// 设置窗口标题
	::SetWindowTextA(s_HWnd, title.c_str());
}

void easy2d::Window::setIcon(int iconID)
{
	HINSTANCE hInstance = ::GetModuleHandleA(nullptr);
	HICON hIcon = (HICON)::LoadImageA(hInstance, MAKEINTRESOURCEA(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	// 设置窗口的图标
	::SendMessageA(s_HWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	::SendMessageA(s_HWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

void easy2d::Window::setCursor(Cursor cursor)
{
	s_currentCursor = cursor;
}

void easy2d::Window::setCustomCursor(Node* cursor)
{
	setCustomCursor([=](Cursor) -> Node*
		{
			return cursor;
		});
}

void easy2d::Window::setCustomCursor(Function<Node* (Cursor)> cursorFunc)
{
	s_customCursor.setCursorFunc(cursorFunc);
	__updateCursor();
}

easy2d::Node* easy2d::Window::getCustomCursor()
{
	return s_customCursor.getCursorNode();
}

easy2d::String easy2d::Window::getTitle()
{
	char wszTitle[MAX_PATH] = { 0 };
	::GetWindowTextA(s_HWnd, wszTitle, MAX_PATH);
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
	::MessageBoxA(s_HWnd, text.c_str(), title.c_str(), MB_ICONINFORMATION | MB_OK);
	Game::reset();
}

void easy2d::Window::warning(const String& title, const String& text)
{
	::MessageBoxA(s_HWnd, text.c_str(), title.c_str(), MB_ICONWARNING | MB_OK);
	Game::reset();
}

void easy2d::Window::error(const String & text, const String & title)
{
	::MessageBoxA(s_HWnd, text.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
	Game::reset();
}

easy2d::KeyCode::Value GetMappedKeyCode(WPARAM wParam, LPARAM lParam)
{
	using easy2d::KeyCode;
	KeyCode::Value vk = KeyCode::Value(wParam);
	bool extended = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED;
	BYTE scancode = LOBYTE(HIWORD(lParam));

	switch (vk) {
	case VK_SHIFT:
		vk = KeyCode::Value(MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX));
		break;
	case VK_CONTROL:
		vk = extended ? KeyCode::RCtrl : KeyCode::LCtrl;
		break;
	case VK_MENU:
		vk = extended ? KeyCode::RAlt : KeyCode::LAlt;
		break;
	default:
		// not a key we map from generic to left/right specialized
		//  just return it.
		break;
	}
	return vk;
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
		KeyCode::Value vk = KeyCode::Value(wParam);
		KeyDownEvent evt(vk, int(lParam & 0xFF));
		SceneManager::dispatch(&evt);

		KeyCode::Value newVk = GetMappedKeyCode(wParam, lParam);
		if (vk != newVk)
		{
			KeyDownEvent evt(newVk, int(lParam & 0xFF));
			SceneManager::dispatch(&evt);
		}
	}
	break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		KeyCode::Value vk = KeyCode::Value(wParam);
		KeyUpEvent evt(vk, int(lParam & 0xFF));
		SceneManager::dispatch(&evt);

		KeyCode::Value newVk = GetMappedKeyCode(wParam, lParam);
		if (vk != newVk)
		{
			KeyUpEvent evt(newVk, int(lParam & 0xFF));
			SceneManager::dispatch(&evt);
		}
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

	// 处理窗口大小变化消息
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
		// 目标适当。它可能会调用失败，但是这里可以忽略有可能的
		// 错误，因为这个错误将在下一次调用 EndDraw 时产生
		auto pRT = Renderer::getRenderTarget();
		if (pRT) pRT->Resize(D2D1::SizeU(width, height));
	}
	break;

	// 处理分辨率变化消息
	case WM_DISPLAYCHANGE:
	{
		// 重绘客户区
		InvalidateRect(hWnd, nullptr, FALSE);
	}
	result = 0;
	hasHandled = true;
	break;

	case WM_SETCURSOR:
	{
		Window::__updateCursor();
	}
	break;

	// 窗口关闭消息
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

	// 窗口销毁消息
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
		result = DefWindowProcA(hWnd, message, wParam, lParam);
	}

	return result;
}