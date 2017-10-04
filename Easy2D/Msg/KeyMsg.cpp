#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <conio.h>

// ���������ص�����������
static std::vector<KeyMsg*> s_vKeyMsg;

// �����ֵ�Ķ���
const VK_KEY KeyMsg::A = 'A';
const VK_KEY KeyMsg::B = 'B';
const VK_KEY KeyMsg::C = 'C';
const VK_KEY KeyMsg::D = 'D';
const VK_KEY KeyMsg::E = 'E';
const VK_KEY KeyMsg::F = 'F';
const VK_KEY KeyMsg::G = 'G';
const VK_KEY KeyMsg::H = 'H';
const VK_KEY KeyMsg::I = 'I';
const VK_KEY KeyMsg::J = 'J';
const VK_KEY KeyMsg::K = 'K';
const VK_KEY KeyMsg::L = 'L';
const VK_KEY KeyMsg::M = 'M';
const VK_KEY KeyMsg::N = 'N';
const VK_KEY KeyMsg::O = 'O';
const VK_KEY KeyMsg::P = 'P';
const VK_KEY KeyMsg::Q = 'Q';
const VK_KEY KeyMsg::R = 'R';
const VK_KEY KeyMsg::S = 'S';
const VK_KEY KeyMsg::T = 'T';
const VK_KEY KeyMsg::U = 'U';
const VK_KEY KeyMsg::V = 'V';
const VK_KEY KeyMsg::W = 'W';
const VK_KEY KeyMsg::X = 'X';
const VK_KEY KeyMsg::Y = 'Y';
const VK_KEY KeyMsg::Z = 'Z';
const VK_KEY KeyMsg::NUM_0 = '0';
const VK_KEY KeyMsg::NUM_1 = '1';
const VK_KEY KeyMsg::NUM_2 = '2';
const VK_KEY KeyMsg::NUM_3 = '3';
const VK_KEY KeyMsg::NUM_4 = '4';
const VK_KEY KeyMsg::NUM_5 = '5';
const VK_KEY KeyMsg::NUM_6 = '6';
const VK_KEY KeyMsg::NUM_7 = '7';
const VK_KEY KeyMsg::NUM_8 = '8';
const VK_KEY KeyMsg::NUM_9 = '9';
const VK_KEY KeyMsg::NUMPAD_0 = VK_NUMPAD0;
const VK_KEY KeyMsg::NUMPAD_1 = VK_NUMPAD1;
const VK_KEY KeyMsg::NUMPAD_2 = VK_NUMPAD2;
const VK_KEY KeyMsg::NUMPAD_3 = VK_NUMPAD3;
const VK_KEY KeyMsg::NUMPAD_4 = VK_NUMPAD4;
const VK_KEY KeyMsg::NUMPAD_5 = VK_NUMPAD5;
const VK_KEY KeyMsg::NUMPAD_6 = VK_NUMPAD6;
const VK_KEY KeyMsg::NUMPAD_7 = VK_NUMPAD7;
const VK_KEY KeyMsg::NUMPAD_8 = VK_NUMPAD8;
const VK_KEY KeyMsg::NUMPAD_9 = VK_NUMPAD9;
const VK_KEY KeyMsg::Enter = VK_RETURN;
const VK_KEY KeyMsg::Space = VK_SPACE;
const VK_KEY KeyMsg::Ctrl = VK_CONTROL;
const VK_KEY KeyMsg::LCtrl = VK_LCONTROL;
const VK_KEY KeyMsg::RCtrl = VK_RCONTROL;
const VK_KEY KeyMsg::Shift = VK_SHIFT;
const VK_KEY KeyMsg::LShift = VK_LSHIFT;
const VK_KEY KeyMsg::RShift = VK_RSHIFT;
const VK_KEY KeyMsg::Up = VK_UP;
const VK_KEY KeyMsg::Down = VK_DOWN;
const VK_KEY KeyMsg::Left = VK_LEFT;
const VK_KEY KeyMsg::Right = VK_RIGHT;
const VK_KEY KeyMsg::Esc = VK_ESCAPE;
const VK_KEY KeyMsg::F1 = VK_F1;
const VK_KEY KeyMsg::F2 = VK_F2;
const VK_KEY KeyMsg::F3 = VK_F3;
const VK_KEY KeyMsg::F4 = VK_F4;
const VK_KEY KeyMsg::F5 = VK_F5;
const VK_KEY KeyMsg::F6 = VK_F6;
const VK_KEY KeyMsg::F7 = VK_F7;
const VK_KEY KeyMsg::F8 = VK_F8;
const VK_KEY KeyMsg::F9 = VK_F9;
const VK_KEY KeyMsg::F10 = VK_F10;
const VK_KEY KeyMsg::F11 = VK_F11;
const VK_KEY KeyMsg::F12 = VK_F12;

static VK_KEY convert(int ascii);

KeyMsg::KeyMsg(TString name, const KEY_CALLBACK & callback)
{
	m_sName = name;
	m_callback = callback;
}

KeyMsg::~KeyMsg()
{
}

void KeyMsg::onKbHit(VK_KEY key)
{
	m_callback(key);
}

void KeyMsg::__exec()
{
	if (_kbhit())						// ������ް�����Ϣ
	{
		VK_KEY key = convert(_getch());	// ��ȡ������Ϣ

		for (auto k : s_vKeyMsg)		// �ַ�����Ϣ
		{
			k->onKbHit(key);			// ִ�а����ص�����
		}
	}
}

void KeyMsg::addListener(TString name, const KEY_CALLBACK & callback)
{
	// �����µļ�������
	auto key = new KeyMsg(name, callback);
	// ����µİ����ص�����
	s_vKeyMsg.push_back(key);
}

bool KeyMsg::delListener(TString name)
{
	// ����������
	std::vector<KeyMsg*>::iterator iter;
	// ѭ���������м�����
	for (iter = s_vKeyMsg.begin(); iter != s_vKeyMsg.end(); iter++)
	{
		// ������ͬ���Ƶļ�����
		if ((*iter)->m_sName == name)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			s_vKeyMsg.erase(iter);
			return true;
		}
	}
	// ��δ�ҵ�ͬ�����Ƶļ����������� false
	return false;
}

void KeyMsg::clearAllListeners()
{
	// ɾ�����м�����
	for (auto k : s_vKeyMsg)
	{
		delete k;
	}
	// �������
	s_vKeyMsg.clear();
}

bool KeyMsg::isKeyDown(VK_KEY key)
{
	// ��ȡ key �İ������
	return (GetAsyncKeyState(key) & 0x8000);
}


VK_KEY convert(int ascii)
{
	// ��ĸ��
	if (ascii >= 'a' && ascii <= 'z')
	{
		return VK_KEY(ascii - ('a' - 'A'));
	}
	else if (ascii >= 'A' && ascii <= 'Z')
	{
		return VK_KEY(ascii);
	}
	// ���ּ�
	else if (ascii >= '0' && ascii <= '9')
	{
		return VK_KEY(ascii);
	}
	// �س����ո�Esc��
	else if (ascii == 0x0D || ascii == 0x20 || ascii == 0x1B)
	{
		return VK_KEY(ascii);
	}
	// ���ܼ�
	else if (ascii == 0 || ascii == 0xE0)
	{
		switch (_getch())
		{
		case 72:
			return KeyMsg::Up;
		case 75:
			return KeyMsg::Left;
		case 77:
			return KeyMsg::Right;
		case 80:
			return KeyMsg::Down;
		case 59:
			return KeyMsg::F1;
		case 60:
			return KeyMsg::F2;
		case 61:
			return KeyMsg::F3;
		case 62:
			return KeyMsg::F4;
		case 63:
			return KeyMsg::F5;
		case 64:
			return KeyMsg::F6;
		case 65:
			return KeyMsg::F7;
		case 66:
			return KeyMsg::F8;
		case 67:
			return KeyMsg::F9;
		case 133:
			return KeyMsg::F10;
		case 134:
			return KeyMsg::F11;
		default:
			return 0;
		}
	}
	return 0;
}