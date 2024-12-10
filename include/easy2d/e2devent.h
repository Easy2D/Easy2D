#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{

// �¼�
struct Event
{
	enum Type : UINT
	{
		Unknown = 0,

		MouseMove,		// ����ƶ�
		MouseDown,		// ��갴��
		MouseUp,		// ���̧��
		MouseWheel,		// �����ֻ���
		KeyDown,		// ��������
		KeyUp,			// ����̧��

		Last = KeyUp,
	};

	const UINT type;
	Object* target;

	Event(UINT type);

	virtual ~Event();
};

// ����ֵ
struct MouseCode
{
	enum Value : int
	{
		Left,		/* ������ */
		Right,		/* ����Ҽ� */
		Middle		/* ����м� */
	};
};


// ���̼�ֵ
struct KeyCode
{
	enum Value : int
	{
		Unknown = 0,
		Up = VK_UP,				// ����� - ��
		Left = VK_LEFT,			// ����� - ��
		Right = VK_RIGHT,		// ����� - ��
		Down = VK_DOWN,			// ����� - ��
		Enter = VK_RETURN,		// �س���
		Space = VK_SPACE,		// �ո��
		Esc = VK_ESCAPE,		// Esc ��
		Ctrl = VK_CONTROL,		// ���� ctrl ��
		LCtrl = VK_LCONTROL,	// �� ctrl ��
		RCtrl = VK_RCONTROL,	// �� ctrl ��
		Shift = VK_SHIFT,		// ���� shift ��
		LShift = VK_LSHIFT,		// �� shift ��
		RShift = VK_RSHIFT,		// �� shift ��
		Alt = VK_MENU,			// ���� alt ��
		LAlt = VK_LMENU,		// �� alt ��
		RAlt = VK_RMENU,		// �� alt ��
		Tab = VK_TAB,			// Tab ��
		Delete = VK_DELETE,		// ɾ����
		Back = VK_BACK,			// �˸��

		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Num0 = 0x30,			// ���ּ�
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,	// С�������ּ�
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,

		F1 = VK_F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
	};
};


struct MouseMoveEvent
	: public Event
{
	MouseMoveEvent(float x, float y);

	float x, y;
};

struct MouseDownEvent
	: public Event
{
	MouseDownEvent(float x, float y, MouseCode::Value btn);

	float x, y;
	MouseCode::Value button;
};

struct MouseUpEvent
	: public Event
{
	MouseUpEvent(float x, float y, MouseCode::Value btn);

	float x, y;
	MouseCode::Value button;
};

struct MouseWheelEvent
	: public Event
{
	MouseWheelEvent(float x, float y, float delta);

	float x, y;
	float delta;
};

struct KeyDownEvent
	: public Event
{
	KeyDownEvent(KeyCode::Value key, int count);

	KeyCode::Value key;
	int count;
};

struct KeyUpEvent
	: public Event
{
	KeyUpEvent(KeyCode::Value key, int count);

	KeyCode::Value key;
	int count;
};

}
