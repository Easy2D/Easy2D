#pragma once
#include <e2dmacros.h>
#include <e2dcommon.h>


// Base Classes

namespace easy2d
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
	static void start();

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
		Normal,		/* Ĭ��ָ����ʽ */
		Hand,		/* ��״ָ�� */
		No,			/* ��ָֹ�� */
		Wait,		/* ɳ©ָ�� */
		ArrowWait	/* Ĭ��ָ���Сɳ© */
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
		Left,		/* ������ */
		Right,		/* ����Ҽ� */
		Middle		/* ����м� */
	};


	// ���̼�ֵ
	enum class Key : int
	{
		Up = 0xC8,
		Left = 0xCB,
		Right = 0xCD,
		Down = 0xD0,
		Enter = 0x1C,
		Space = 0x39,
		Esc = 0x01,
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
		Num1 = 0x02,
		Num2 = 0x03,
		Num3 = 0x04,
		Num4 = 0x05,
		Num5 = 0x06,
		Num6 = 0x07,
		Num7 = 0x08,
		Num8 = 0x09,
		Num9 = 0x0A,
		Num0 = 0x0B,
		Numpad7 = 0x47,
		Numpad8 = 0x48,
		Numpad9 = 0x49,
		Numpad4 = 0x4B,
		Numpad5 = 0x4C,
		Numpad6 = 0x4D,
		Numpad1 = 0x4F,
		Numpad2 = 0x50,
		Numpad3 = 0x51,
		Numpad0 = 0x52
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
		const Function<void()>& func,		/* �������û�����ʱ��ִ�к��� */
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

	// ����������Ⱦ��ʽ
	static void SetTextStyle(
		const Color& fillColor,
		bool hasOutline,
		const Color& outlineColor,
		float outlineWidth,
		LineJoin outlineJoin
	);

	// ��Ⱦ���ֲ���
	static void DrawTextLayout(
		IDWriteTextLayout* layout
	);

	// ��ȡ Miter ��ʽ�� ID2D1StrokeStyle
	static ID2D1StrokeStyle * getMiterID2D1StrokeStyle();

	// ��ȡ Bevel ��ʽ�� ID2D1StrokeStyle
	static ID2D1StrokeStyle * getBevelID2D1StrokeStyle();

	// ��ȡ Round ��ʽ�� ID2D1StrokeStyle
	static ID2D1StrokeStyle * getRoundID2D1StrokeStyle();

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


// ��־
class Logger
{
public:
	// ������־��¼
	static void enable();

	// �ر���־��¼
	static void disable();

	// �����־��Ϣ
	static void messageln(String format, ...);

	// �������
	static void warningln(String format, ...);

	// �������
	static void errorln(String format, ...);

	// ��/�رտ���̨
	static void showConsole(bool show = true);
};


// ��������װ��
class GC
{
	friend class Game;
	friend class Object;

public:
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


//
// GC macros
//

class E2D_GCNewHelper
{
public:
	static inline E2D_GCNewHelper& GetInstance()
	{
		static E2D_GCNewHelper helper;
		return helper;
	}

	template <typename _Ty>
	_Ty* operator- (_Ty* newObj) const
	{
		if (newObj)
		{
			newObj->autorelease();
		}
		return newObj;
	}
};

#ifndef gcnew
#	define gcnew E2D_GCNewHelper::GetInstance() - new (std::nothrow)
#endif


//
// Log macros
//

#ifndef E2D_LOG
#	ifdef E2D_DEBUG
#		define E2D_LOG(FORMAT, ...) easy2d::Logger::messageln(FORMAT, __VA_ARGS__)
#	else
#		define E2D_LOG __noop
#	endif
#endif

#ifndef E2D_WARNING
#	define E2D_WARNING(FORMAT, ...) easy2d::Logger::warningln(FORMAT, __VA_ARGS__)
#endif

#ifndef E2D_ERROR
#	define E2D_ERROR(FORMAT, ...) easy2d::Logger::errorln(FORMAT, __VA_ARGS__)
#endif

#ifndef E2D_ERROR_IF_FAILED
#	define E2D_ERROR_IF_FAILED(HR, FORMAT, ...) do { if (FAILED(HR)) { E2D_ERROR(FORMAT, __VA_ARGS__); } } while (0)
#endif

}