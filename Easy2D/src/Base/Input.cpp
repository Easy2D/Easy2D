#include <e2dbase.h>
#include <e2dtool.h>
#include <e2dmanager.h>

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

#define BUFFER_SIZE	256

namespace
{
	IDirectInput8* s_pDirectInput = nullptr;			// DirectInput �ӿڶ���
	IDirectInputDevice8* s_KeyboardDevice = nullptr;	// �����豸�ӿ�
	char s_KeyBuffer[BUFFER_SIZE] = { 0 };				// ���ڱ�����̰�����Ϣ������
	char s_KeyRecordBuffer[BUFFER_SIZE] = { 0 };		// ������Ϣ����������

	IDirectInputDevice8* s_MouseDevice = nullptr;		// ����豸�ӿ�
	DIMOUSESTATE s_MouseState;							// �����Ϣ�洢�ṹ��
	DIMOUSESTATE s_MouseRecordState;					// �����Ϣ��������
	POINT s_MousePosition;								// ���λ�ô洢�ṹ��

	std::vector<easy2d::Listener*> s_vListeners;		// ����������
}

using namespace easy2d;


bool Input::__init()
{
	ZeroMemory(s_KeyBuffer, sizeof(s_KeyBuffer));
	ZeroMemory(s_KeyRecordBuffer, sizeof(s_KeyRecordBuffer));
	ZeroMemory(&s_MouseState, sizeof(s_MouseState));
	ZeroMemory(&s_MouseRecordState, sizeof(s_MouseRecordState));

	// ��ʼ���ӿڶ���
	HRESULT hr = DirectInput8Create(
		HINST_THISCOMPONENT,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pDirectInput,
		nullptr
	);

	if (SUCCEEDED(hr))
	{
		// ��ʼ�������豸
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
		// ��ʼ������豸
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
			E2D_WARNING(L"Mouse not found!");
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
	Input::__updateDeviceState();
	Input::__updateListeners();
}

void Input::__updateDeviceState()
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

			s_KeyboardDevice->GetDeviceState(sizeof(s_KeyBuffer), (void**)&s_KeyBuffer);
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
			s_MouseDevice->GetDeviceState(sizeof(s_MouseState), (void**)&s_MouseState);
		}
	}

	GetCursorPos(&s_MousePosition);
	ScreenToClient(Window::getHWnd(), &s_MousePosition);
}

bool Input::isDown(Key key)
{
	if (s_KeyBuffer[static_cast<int>(key)] & 0x80)
		return true;
	return false;
}

bool Input::isPress(Key key)
{
	if ((s_KeyBuffer[static_cast<int>(key)] & 0x80) && 
		!(s_KeyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool Input::isRelease(Key key)
{
	if (!(s_KeyBuffer[static_cast<int>(key)] & 0x80) && 
		(s_KeyRecordBuffer[static_cast<int>(key)] & 0x80))
		return true;
	return false;
}

bool easy2d::Input::isDown(Mouse code)
{
	if (s_MouseState.rgbButtons[static_cast<int>(code)] & 0x80)
		return true;
	return false;
}

bool easy2d::Input::isPress(Mouse code)
{
	if ((s_MouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		!(s_MouseRecordState.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

bool easy2d::Input::isRelease(Mouse code)
{
	if (!(s_MouseState.rgbButtons[static_cast<int>(code)] & 0x80) && 
		(s_MouseRecordState.rgbButtons[static_cast<int>(code)] & 0x80))
		return true;
	return false;
}

double Input::getMouseX()
{
	return (double)s_MousePosition.x;
}

double Input::getMouseY()
{
	return (double)s_MousePosition.y;
}

Point Input::getMousePos()
{
	return Point((double)s_MousePosition.x, (double)s_MousePosition.y);
}

double Input::getMouseDeltaX()
{
	return (double)s_MouseState.lX;
}

double Input::getMouseDeltaY()
{
	return (double)s_MouseState.lY;
}

double Input::getMouseDeltaZ()
{
	return (double)s_MouseState.lZ;
}

easy2d::Listener * easy2d::Input::addListener(const Function<void()>& func, const String& name, bool paused)
{
	auto listener = gcnew Listener(func, name, paused);
	GC::retain(listener);
	s_vListeners.push_back(listener);
	return listener;
}

void easy2d::Input::addListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(s_vListeners.begin(), s_vListeners.end(), listener);
		if (iter == s_vListeners.end())
		{
			GC::retain(listener);
			s_vListeners.push_back(listener);
		}
	}
}

void easy2d::Input::removeListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(s_vListeners.begin(), s_vListeners.end(), listener);
		if (iter != s_vListeners.end())
		{
			GC::release(listener);
			s_vListeners.erase(iter);
		}
	}
}

void easy2d::Input::stopListener(const String& name)
{
	if (s_vListeners.empty() || name.empty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->stop();
		}
	}
}

void easy2d::Input::startListener(const String& name)
{
	if (s_vListeners.empty() || name.empty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->start();
		}
	}
}

void easy2d::Input::removeListener(const String& name)
{
	if (s_vListeners.empty() || name.empty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->_stopped = true;
		}
	}
}

void easy2d::Input::stopAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->stop();
	}
}

void easy2d::Input::startAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->start();
	}
}

void easy2d::Input::removeAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->_stopped = true;
	}
}

void easy2d::Input::__updateListeners()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); ++i)
	{
		auto listener = s_vListeners[i];
		// �����ֹͣ�ļ�����
		if (listener->_stopped)
		{
			GC::release(listener);
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ���¼�����
			listener->_update();
			++i;
		}
	}
}

void easy2d::Input::__clearListeners()
{
	for (auto listener : s_vListeners)
	{
		GC::release(listener);
	}
	s_vListeners.clear();
}