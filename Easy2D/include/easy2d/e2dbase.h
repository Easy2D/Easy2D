#pragma once
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dcommon.h>


// Base Classes

namespace easy2d
{


// ��Ϸ����
class Game
{
public:
	// ��ʼ����Ϸ
	static bool init(
		const String& title = L"Easy2D",	/* ���ڱ��� */
		int width = 640,					/* ���ڿ�� */
		int height = 480,					/* ���ڸ߶� */
		const String& mutexName = L""		/* ���̻��������� */
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
	static float getWidth();

	// ��ȡ���ڸ߶�
	static float getHeight();

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
	static bool __init(
		const String& title,
		int width,
		int height
	);

	// ���ô�������
	static void __uninit();

	// ��������Ϣ
	static void __poll();

	// ����ָ��
	static void __updateCursor();

	// Win32 ������Ϣ�ص�����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// ʱ�����
class Time
{
	friend class Game;

public:
	// ��ȡ��һ֡�뵱ǰ֡��ʱ�������룩
	static float getDeltaTime();

	// ��ȡ��һ֡�뵱ǰ֡��ʱ���������룩
	static unsigned int getDeltaTimeMilliseconds();

	// ��ȡ��Ϸ��ʱ�����룩
	static float getTotalTime();

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
	// ������ĳ�����Ƿ���������
	static bool isDown(
		KeyCode::Value key
	);

	// ������ĳ�����Ƿ񱻵��
	static bool isPress(
		KeyCode::Value key
	);

	// ������ĳ�����Ƿ������ɿ�
	static bool isRelease(
		KeyCode::Value key
	);

	// �����갴���Ƿ���������
	static bool isDown(
		MouseCode::Value code
	);

	// �����갴���Ƿ񱻵��
	static bool isPress(
		MouseCode::Value code
	);

	// �����갴���Ƿ������ɿ�
	static bool isRelease(
		MouseCode::Value code
	);

	// ������X������ֵ
	static float getMouseX();

	// ������Y������ֵ
	static float getMouseY();

	// ����������ֵ
	static Point getMousePos();

	// ������X����������
	static float getMouseDeltaX();

	// ������Y����������
	static float getMouseDeltaY();

	// ������Z�ᣨ�����֣���������
	static float getMouseDeltaZ();

private:
	// ��ʼ�� DirectInput �Լ���������豸
	static bool __init();

	// ˢ��������Ϣ
	static void __update();

	// ж�� DirectInput
	static void __uninit();
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
public:
	// ��������� GC ��
	static void trace(
		Object* pObject
	);

	// �������
	static void clear();

	// �������Ƿ��� GC ����
	static bool isInPool(Object* pObject);

	// GC ��״̬
	static bool isClearing();

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
};


//
// GC macros
//

namespace __gc_helper
{
	class GCNewHelper
	{
	public:
		template <typename _Ty>
		inline _Ty* operator<< (_Ty* newObj) const
		{
			if (newObj)
			{
				newObj->autorelease();
			}
			return newObj;
		}

		// ʹ�� static ���������� static inline ����
		// �� IntelliSense ��������
		static GCNewHelper instance;
	};
}

#ifndef gcnew
#	define gcnew __gc_helper::GCNewHelper::instance << new (std::nothrow)
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