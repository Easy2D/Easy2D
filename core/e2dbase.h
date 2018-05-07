#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"


// Base Classes

namespace e2d
{


class Game
{
public:
	// ��ʼ����Ϸ
	static bool init(
		const String& name = L""	/* ��ϷӢ������ */
	);

	// ������Ϸ
	static int start(
		bool autoRelease = true		/* ��Ϸ����ʱ�Զ�������Դ */
	);

	// ��ͣ��Ϸ
	static void pause();

	// ������Ϸ
	static void resume();

	// ������Ϸ
	static void quit();

	// ������Ϸ��Դ
	static void destroy();

	// ��Ϸ�Ƿ���ͣ
	static bool isPaused();

	// �������̻�����
	static bool createMutex(
		const String& sMutexName,			/* ���������� */
		const String& sWindowTitle = L""	/* ���ڱ��� */
	);

	// ��ȡ��Ϸ����
	static String getName();
};


// ���ƴ�������
class Window
{
	friend Game;

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


// ������Ϸʱ��
class Time
{
	friend Game;

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

	// ���ü�ʱ����
	static void __uninit();

	// �Ƿ�ﵽ����ʱ��
	static bool __isReady();

	// ���µ�ǰʱ��
	static void __updateNow();

	// ����ʱ����Ϣ
	static void __updateLast();

	// �����߳�
	static void __sleep();
};


// ���Ƽ��̺���������
class Input
{
	friend Game;

public:
	// ������ĳ�����Ƿ���������
	static bool isKeyDown(
		KeyCode key
	);

	// ������ĳ�����Ƿ񱻵��
	static bool isKeyPress(
		KeyCode key
	);

	// ������ĳ�����Ƿ������ɿ�
	static bool isKeyRelease(
		KeyCode key
	);

	// ����������Ƿ���������
	static bool isMouseLButtonDown();

	// �������Ҽ��Ƿ���������
	static bool isMouseRButtonDown();

	// �������м��Ƿ���������
	static bool isMouseMButtonDown();

	// ����������Ƿ񱻵��
	static bool isMouseLButtonPress();

	// �������Ҽ��Ƿ񱻵��
	static bool isMouseRButtonPress();

	// �������м��Ƿ񱻵��
	static bool isMouseMButtonPress();

	// ����������Ƿ������ɿ�
	static bool isMouseLButtonRelease();

	// �������Ҽ��Ƿ������ɿ�
	static bool isMouseRButtonRelease();

	// �������м��Ƿ������ɿ�
	static bool isMouseMButtonRelease();

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

private:
	// ��ʼ�� DirectInput �Լ���������豸
	static bool __init();

	// ˢ��������Ϣ
	static void __update();

	// ˢ���豸״̬
	static void __updateDeviceState();

	// ж�� DirectInput
	static void __uninit();
};


// ��Ⱦ��
class Renderer
{
	friend Game;
	friend Window;

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

	// ��ȡ�Զ����������Ⱦ��
	static CustomTextRenderer * getCustomTextRenderer();

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

}