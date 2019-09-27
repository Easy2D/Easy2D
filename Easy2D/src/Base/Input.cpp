#include <unordered_map>
#include <e2dbase.h>
#include <e2dmanager.h>

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

#define BUFFER_SIZE	256

using namespace easy2d;

namespace
{
	IDirectInput8* s_pDirectInput = nullptr;			// DirectInput 接口对象
	IDirectInputDevice8* s_KeyboardDevice = nullptr;	// 键盘设备接口
	char s_KeyBuffer[BUFFER_SIZE] = { 0 };				// 用于保存键盘按键信息缓冲区
	char s_KeyRecordBuffer[BUFFER_SIZE] = { 0 };		// 键盘消息二级缓冲区

	IDirectInputDevice8* s_MouseDevice = nullptr;		// 鼠标设备接口
	DIMOUSESTATE s_MouseState;							// 鼠标信息存储结构体
	DIMOUSESTATE s_MouseRecordState;					// 鼠标信息二级缓冲
	POINT s_MousePosition;								// 鼠标位置存储结构体

	const std::unordered_map<int, int> s_KeyboardMapping = {
		{ KeyCode::Unknown, 0x00 },
		{ KeyCode::Up, DIK_UP },
		{ KeyCode::Left, DIK_LEFT },
		{ KeyCode::Right, DIK_RIGHT },
		{ KeyCode::Down, DIK_DOWN },
		{ KeyCode::Enter, DIK_RETURN },
		{ KeyCode::Space, DIK_SPACE },
		{ KeyCode::Esc, DIK_ESCAPE },
		{ KeyCode::Ctrl, DIK_LCONTROL },
		{ KeyCode::Shift, DIK_LSHIFT },
		{ KeyCode::Alt, DIK_LMENU },
		{ KeyCode::Tab, DIK_TAB },
		{ KeyCode::Delete, DIK_DELETE },
		{ KeyCode::Back, DIK_BACK },

		{ KeyCode::A, DIK_A },
		{ KeyCode::B, DIK_B },
		{ KeyCode::C, DIK_C },
		{ KeyCode::D, DIK_D },
		{ KeyCode::E, DIK_E },
		{ KeyCode::F, DIK_F },
		{ KeyCode::G, DIK_G },
		{ KeyCode::H, DIK_H },
		{ KeyCode::I, DIK_I },
		{ KeyCode::J, DIK_J },
		{ KeyCode::K, DIK_K },
		{ KeyCode::L, DIK_L },
		{ KeyCode::M, DIK_M },
		{ KeyCode::N, DIK_N },
		{ KeyCode::O, DIK_O },
		{ KeyCode::P, DIK_P },
		{ KeyCode::Q, DIK_Q },
		{ KeyCode::R, DIK_R },
		{ KeyCode::S, DIK_S },
		{ KeyCode::T, DIK_T },
		{ KeyCode::U, DIK_U },
		{ KeyCode::V, DIK_V },
		{ KeyCode::W, DIK_W },
		{ KeyCode::X, DIK_X },
		{ KeyCode::Y, DIK_Y },
		{ KeyCode::Z, DIK_Z },

		{ KeyCode::Num0, DIK_0 },
		{ KeyCode::Num1, DIK_1 },
		{ KeyCode::Num2, DIK_2 },
		{ KeyCode::Num3, DIK_3 },
		{ KeyCode::Num4, DIK_4 },
		{ KeyCode::Num5, DIK_5 },
		{ KeyCode::Num6, DIK_6 },
		{ KeyCode::Num7, DIK_7 },
		{ KeyCode::Num8, DIK_8 },
		{ KeyCode::Num9, DIK_9 },

		{ KeyCode::Numpad0, DIK_NUMPAD0 },
		{ KeyCode::Numpad1, DIK_NUMPAD1 },
		{ KeyCode::Numpad2, DIK_NUMPAD2 },
		{ KeyCode::Numpad3, DIK_NUMPAD3 },
		{ KeyCode::Numpad4, DIK_NUMPAD4 },
		{ KeyCode::Numpad5, DIK_NUMPAD5 },
		{ KeyCode::Numpad6, DIK_NUMPAD6 },
		{ KeyCode::Numpad7, DIK_NUMPAD7 },
		{ KeyCode::Numpad8, DIK_NUMPAD8 },
		{ KeyCode::Numpad9, DIK_NUMPAD9 },

		{ KeyCode::F1, DIK_F1 },
		{ KeyCode::F2, DIK_F2 },
		{ KeyCode::F3, DIK_F3 },
		{ KeyCode::F4, DIK_F4 },
		{ KeyCode::F5, DIK_F5 },
		{ KeyCode::F6, DIK_F6 },
		{ KeyCode::F7, DIK_F7 },
		{ KeyCode::F8, DIK_F8 },
		{ KeyCode::F9, DIK_F9 },
		{ KeyCode::F10, DIK_F10 },
		{ KeyCode::F11, DIK_F11 },
		{ KeyCode::F12, DIK_F12 },
	};
}

bool Input::__init()
{
	ZeroMemory(s_KeyBuffer, sizeof(s_KeyBuffer));
	ZeroMemory(s_KeyRecordBuffer, sizeof(s_KeyRecordBuffer));
	ZeroMemory(&s_MouseState, sizeof(s_MouseState));
	ZeroMemory(&s_MouseRecordState, sizeof(s_MouseRecordState));

	// 初始化接口对象
	HRESULT hr = DirectInput8Create(
		HINST_THISCOMPONENT,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pDirectInput,
		nullptr
	);

	if (SUCCEEDED(hr))
	{
		// 初始化键盘设备
		hr = s_pDirectInput->CreateDevice(
			GUID_SysKeyboard,
			&s_KeyboardDevice,
			nullptr
		);

		if (SUCCEEDED(hr))
		{
			s_KeyboardDevice->SetCooperativeLevel(
				Window::getHWnd(),
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);
			s_KeyboardDevice->SetDataFormat(
				&c_dfDIKeyboard);
			s_KeyboardDevice->Acquire();
			s_KeyboardDevice->Poll();
		}
		else
		{
			E2D_WARNING(L"Keyboard not found!");
			return false;
		}
	}

	if (SUCCEEDED(hr))
	{
		// 初始化鼠标设备
		hr = s_pDirectInput->CreateDevice(GUID_SysMouse, &s_MouseDevice, nullptr);

		if (SUCCEEDED(hr))
		{
			s_MouseDevice->SetCooperativeLevel(Window::getHWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			s_MouseDevice->SetDataFormat(&c_dfDIMouse);
			s_MouseDevice->Acquire();
			s_MouseDevice->Poll();
		}
		else
		{
			E2D_WARNING(L"MouseCode not found!");
			return false;
		}
	}

	return SUCCEEDED(hr);
}

void Input::__uninit()
{
	if (s_KeyboardDevice)
		s_KeyboardDevice->Unacquire();
	if (s_MouseDevice)
		s_MouseDevice->Unacquire();

	SafeRelease(s_MouseDevice);
	SafeRelease(s_KeyboardDevice);
	SafeRelease(s_pDirectInput);
}

void easy2d::Input::__update()
{
	if (s_KeyboardDevice)
	{
		HRESULT hr = s_KeyboardDevice->Poll();
		if (FAILED(hr))
		{
			hr = s_KeyboardDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = s_KeyboardDevice->Acquire();
		}
		else
		{
			for (int i = 0; i < BUFFER_SIZE; ++i)
				s_KeyRecordBuffer[i] = s_KeyBuffer[i];

			s_KeyboardDevice->GetDeviceState(sizeof(s_KeyBuffer), (void**)& s_KeyBuffer);
		}
	}

	if (s_MouseDevice)
	{
		HRESULT hr = s_MouseDevice->Poll();
		if (FAILED(hr))
		{
			hr = s_MouseDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = s_MouseDevice->Acquire();
		}
		else
		{
			s_MouseRecordState = s_MouseState;
			s_MouseDevice->GetDeviceState(sizeof(s_MouseState), (void**)& s_MouseState);
		}
	}

	GetCursorPos(&s_MousePosition);
	ScreenToClient(Window::getHWnd(), &s_MousePosition);
}

bool Input::isDown(KeyCode::Value key)
{
	int dik = s_KeyboardMapping.at(key);
	if (s_KeyBuffer[dik] & 0x80)
		return true;
	return false;
}

bool Input::isPress(KeyCode::Value key)
{
	int dik = s_KeyboardMapping.at(key);
	if ((s_KeyBuffer[dik] & 0x80) &&
		!(s_KeyRecordBuffer[dik] & 0x80))
		return true;
	return false;
}

bool Input::isRelease(KeyCode::Value key)
{
	int dik = s_KeyboardMapping.at(key);
	if (!(s_KeyBuffer[dik] & 0x80) &&
		(s_KeyRecordBuffer[dik] & 0x80))
		return true;
	return false;
}

bool easy2d::Input::isDown(MouseCode::Value code)
{
	if (s_MouseState.rgbButtons[static_cast<int>(code)] & 0x80)
		return true;
	return false;
}

bool easy2d::Input::isPress(MouseCode::Value code)
{
	if ((s_MouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		!(s_MouseRecordState.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

bool easy2d::Input::isRelease(MouseCode::Value code)
{
	if (!(s_MouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		(s_MouseRecordState.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

float Input::getMouseX()
{
	return (float)s_MousePosition.x;
}

float Input::getMouseY()
{
	return (float)s_MousePosition.y;
}

Point Input::getMousePos()
{
	return Point((float)s_MousePosition.x, (float)s_MousePosition.y);
}

float Input::getMouseDeltaX()
{
	return (float)s_MouseState.lX;
}

float Input::getMouseDeltaY()
{
	return (float)s_MouseState.lY;
}

float Input::getMouseDeltaZ()
{
	return (float)s_MouseState.lZ;
}
