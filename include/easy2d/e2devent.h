#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{

// 事件
struct Event
{
	enum Type : UINT
	{
		Unknown = 0,

		MouseMove,		// 鼠标移动
		MouseDown,		// 鼠标按下
		MouseUp,		// 鼠标抬起
		MouseWheel,		// 鼠标滚轮滑动
		KeyDown,		// 按键按下
		KeyUp,			// 按键抬起

		Last = KeyUp,
	};

	const UINT type;
	Object* target;

	Event(UINT type);

	virtual ~Event();
};

// 鼠标键值
struct MouseCode
{
	enum Value : int
	{
		Left,		/* 鼠标左键 */
		Right,		/* 鼠标右键 */
		Middle		/* 鼠标中键 */
	};
};


// 键盘键值
struct KeyCode
{
	enum Value : int
	{
		Unknown = 0,
		Up = VK_UP,				// 方向键 - 上
		Left = VK_LEFT,			// 方向键 - 左
		Right = VK_RIGHT,		// 方向键 - 右
		Down = VK_DOWN,			// 方向键 - 下
		Enter = VK_RETURN,		// 回车键
		Space = VK_SPACE,		// 空格键
		Esc = VK_ESCAPE,		// Esc 键
		Ctrl = VK_CONTROL,		// 任意 ctrl 键
		LCtrl = VK_LCONTROL,	// 左 ctrl 键
		RCtrl = VK_RCONTROL,	// 右 ctrl 键
		Shift = VK_SHIFT,		// 任意 shift 键
		LShift = VK_LSHIFT,		// 左 shift 键
		RShift = VK_RSHIFT,		// 右 shift 键
		Alt = VK_MENU,			// 任意 alt 键
		LAlt = VK_LMENU,		// 左 alt 键
		RAlt = VK_RMENU,		// 右 alt 键
		Tab = VK_TAB,			// Tab 键
		Delete = VK_DELETE,		// 删除键
		Back = VK_BACK,			// 退格键

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

		Num0 = 0x30,			// 数字键
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,	// 小键盘数字键
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
