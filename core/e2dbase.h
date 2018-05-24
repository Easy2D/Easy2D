#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"
#include "e2dcustom.h"


// Base Classes

namespace e2d
{


// ��Ϸ����
class Game
{
public:
	// ��ʼ����Ϸ
	static bool init(
		const String& name = L"",		/* ��ϷӢ������ */
		const String& mutexName = L""	/* ���̻��������� */
	);

	// ������Ϸ
	static void start(
		bool autoRelease = true			/* ��Ϸ����ʱ�Զ�������Դ */
	);

	// ��ͣ��Ϸ
	static void pause();

	// ������Ϸ
	static void resume();

	// ������Ϸ
	static void quit();

	// ������Ϸ��Դ
	static void destroy();

	// ������Ϸ�ڲ���ʱ
	static void reset();

	// ��Ϸ�Ƿ���ͣ
	static bool isPaused();

	// ��ȡ��Ϸ����
	static String getName();
};


// ���ڿ���
class Window
{
	friend class Game;

public:
	// ���ָ����ʽ
	enum class Cursor : int
	{
		NORMAL,		/* Ĭ��ָ����ʽ */
		HAND,		/* ��״ָ�� */
		NO,			/* ��ָֹ�� */
		WAIT,		/* ɳ©ָ�� */
		ARROW_WAIT	/* Ĭ��ָ���Сɳ© */
	};

public:
	// �޸Ĵ��ڴ�С
	static void setSize(
		int width,			/* ���ڿ�� */
		int height			/* ���ڸ߶� */
	);

	// ���ô��ڱ���
	static void setTitle(
		const String& title	/* ���ڱ��� */
	);

	// ���ô���ͼ��
	static void setIcon(
		int iconID
	);

	// �������ָ����ʽ
	static void setCursor(
		Cursor cursor
	);

	// ��ȡ���ڱ���
	static String getTitle();

	// ��ȡ���ڿ��
	static double getWidth();

	// ��ȡ���ڸ߶�
	static double getHeight();

	// ��ȡ���ڴ�С
	static Size getSize();

	// ��ȡ���ھ��
	static HWND getHWnd();

	// ��/���ؿ���̨
	static void showConsole(
		bool show
	);

	// �Ƿ�������Ӧ���뷨
	static void setTypewritingEnable(
		bool enable
	);

	// ������ʾ����
	static void info(
		const String& text,					/* ���� */
		const String& title = L"Infomation"	/* ���ڱ��� */
	);

	// �������洰��
	static void warning(
		const String& text,					/* ���� */
		const String& title = L"Warning"	/* ���ڱ��� */
	);

	// �������󴰿�
	static void error(
		const String& text,					/* ���� */
		const String& title = L"Error"		/* ���ڱ��� */
	);

private:
	// ��ʼ������
	static bool __init();

	// ���ô�������
	static void __uninit();

	// ��������Ϣ
	static void __poll();

	// Win32 ������Ϣ�ص�����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// ʱ�����
class Time
{
	friend class Game;

public:
	// ��ȡ��һ֡�뵱ǰ֡��ʱ�������룩
	static double getDeltaTime();

	// ��ȡ��һ֡�뵱ǰ֡��ʱ���������룩
	static unsigned int getDeltaTimeMilliseconds();

	// ��ȡ��Ϸ��ʱ�����룩
	static double getTotalTime();

	// ��ȡ��Ϸ��ʱ�������룩
	static unsigned int getTotalTimeMilliseconds();

private:
	// ��ʼ����ʱ����
	static bool __init();

	// �Ƿ�ﵽ����ʱ��
	static bool __isReady();

	// ���µ�ǰʱ��
	static void __updateNow();

	// ����ʱ����Ϣ
	static void __updateLast();

	// ����ʱ����Ϣ
	static void __reset();

	// �����߳�
	static void __sleep();
};


class Listener;

// �������
class Input
{
	friend class Game;

public:
	// ����ֵ
	enum class Mouse : int
	{
		LEFT,		/* ������ */
		RIGHT,		/* ����Ҽ� */
		MIDDLE		/* ����м� */
	};


	// ���̼�ֵ
	enum class Key : int
	{
		UP = 0xC8,
		LEFT = 0xCB,
		RIGHT = 0xCD,
		DOWN = 0xD0,
		ENTER = 0x1C,
		SPACE = 0x39,
		ESC = 0x01,
		BACK = 0x0E,
		TAB = 0x0F,
		PAUSE = 0xC5,
		Q = 0x10,
		W = 0x11,
		E = 0x12,
		R = 0x13,
		T = 0x14,
		Y = 0x15,
		U = 0x16,
		I = 0x17,
		O = 0x18,
		P = 0x19,
		A = 0x1E,
		S = 0x1F,
		D = 0x20,
		F = 0x21,
		G = 0x22,
		H = 0x23,
		J = 0x24,
		K = 0x25,
		L = 0x26,
		Z = 0x2C,
		X = 0x2D,
		C = 0x2E,
		V = 0x2F,
		B = 0x30,
		N = 0x31,
		M = 0x32,
		NUM1 = 0x02,
		NUM2 = 0x03,
		NUM3 = 0x04,
		NUM4 = 0x05,
		NUM5 = 0x06,
		NUM6 = 0x07,
		NUM7 = 0x08,
		NUM8 = 0x09,
		NUM9 = 0x0A,
		NUM0 = 0x0B,
		NUMPAD7 = 0x47,
		NUMPAD8 = 0x48,
		NUMPAD9 = 0x49,
		NUMPAD4 = 0x4B,
		NUMPAD5 = 0x4C,
		NUMPAD6 = 0x4D,
		NUMPAD1 = 0x4F,
		NUMPAD2 = 0x50,
		NUMPAD3 = 0x51,
		NUMPAD0 = 0x52
	};

public:
	// ������ĳ�����Ƿ���������
	static bool isDown(
		Key key
	);

	// ������ĳ�����Ƿ񱻵��
	static bool isPress(
		Key key
	);

	// ������ĳ�����Ƿ������ɿ�
	static bool isRelease(
		Key key
	);

	// �����갴���Ƿ���������
	static bool isDown(
		Mouse code
	);

	// �����갴���Ƿ񱻵��
	static bool isPress(
		Mouse code
	);

	// �����갴���Ƿ������ɿ�
	static bool isRelease(
		Mouse code
	);

	// ������X������ֵ
	static double getMouseX();

	// ������Y������ֵ
	static double getMouseY();

	// ����������ֵ
	static Point getMousePos();

	// ������X����������
	static double getMouseDeltaX();

	// ������Y����������
	static double getMouseDeltaY();

	// ������Z�ᣨ�����֣���������
	static double getMouseDeltaZ();

	// ����������
	static Listener * addListener(
		const Function& func,		/* �������û�����ʱ��ִ�к��� */
		const String& name = L"",	/* ���������� */
		bool paused = false			/* �Ƿ���ͣ */
	);

	// �����ײ����
	static void addListener(
		Listener * listener			/* ������ */
	);

	// �Ƴ�������
	static void removeListener(
		Listener * listener			/* ������ */
	);

	// �����������
	static void startListener(
		const String& name
	);

	// ֹͣ�������
	static void stopListener(
		const String& name
	);

	// �Ƴ��������
	static void removeListener(
		const String& name
	);

	// �������м�����
	static void startAllListeners();

	// ֹͣ���м�����
	static void stopAllListeners();

	// �Ƴ����м�����
	static void removeAllListeners();

private:
	// ��ʼ�� DirectInput �Լ���������豸
	static bool __init();

	// ˢ��������Ϣ
	static void __update();

	// ˢ���豸״̬
	static void __updateDeviceState();

	// ���¼�����
	static void __updateListeners();

	// ж�� DirectInput
	static void __uninit();

	// ��ռ�����
	static void __clearListeners();
};


// ��Ⱦ��
class Renderer
{
	friend class Game;
	friend class Window;

public:
	// ��ȡ����ɫ
	static Color getBackgroundColor();

	// �޸ı���ɫ
	static void setBackgroundColor(
		Color color
	);

	// ��ʾ FPS
	static void showFps(
		bool show = true
	);

	// ��ȡϵͳ DPI ����
	static float getDpiScaleX();

	// ��ȡϵͳ DPI ����
	static float getDpiScaleY();

	// ��ȡ ID2D1Factory ����
	static ID2D1Factory * getID2D1Factory();

	// ��ȡ ID2D1HwndRenderTarget ����
	static ID2D1HwndRenderTarget * getRenderTarget();

	// ��ȡ ID2D1SolidColorBrush ����
	static ID2D1SolidColorBrush * getSolidColorBrush();

	// ��ȡ IWICImagingFactory ����
	static IWICImagingFactory * getIWICImagingFactory();

	// ��ȡ IDWriteFactory ����
	static IDWriteFactory * getIDWriteFactory();

	// ��ȡ������Ⱦ��
	static TextRenderer * getTextRenderer();

private:
	// ��Ⱦ��Ϸ����
	static void __render();

	// �����豸�޹���Դ
	static bool __createDeviceIndependentResources();

	// �����豸�����Դ
	static bool __createDeviceResources();

	// ɾ���豸�����Դ
	static void __discardDeviceResources();

	// ɾ��������Ⱦ�����Դ
	static void __discardResources();
};


// ��������װ��
class GC
{
	friend class Game;
	friend class Object;

public:
	// �������Զ������ڴ�Ķ���
	template <typename Type, typename... Args>
	static inline Type * create(Args&&... args)
	{
		auto newObj = new (std::nothrow) Type(std::forward<Args>(args)...);
		if (newObj)
		{
			newObj->autorelease();
			return newObj;
		}
		return nullptr;
	}

	// ��������
	template <typename Type>
	static inline void retain(Type*& p)
	{
		if (p != nullptr)
		{
			p->retain();
		}
	}

	// �ͷŶ���
	template <typename Type>
	static inline void release(Type*& p)
	{
		if (p != nullptr)
		{
			p->release();
			p = nullptr;
		}
	}

	// ֪ͨ GC ���������ڴ�
	static void notify();

	// �ֶ����������ڴ�
	static void flush();

private:
	// ��������� GC
	static void __add(
		Object * pObject
	);

	// ���� GC
	static void __update();

	// ������ж���
	static void __clear();
};

}