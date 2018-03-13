#pragma once
#include "emacros.h"
#include "ecommon.h"


// Base Classes

namespace e2d
{


class Game
{
public:
	// ��ʼ����Ϸ
	static bool init(
		const String & sTitle,			/* ���ڱ��� */
		UINT32 nWidth,					/* ���ڿ�� */
		UINT32 nHeight,					/* ���ڸ߶� */
		LPCTSTR pIconID = nullptr,		/* ����ͼ�� */
		const String & sAppname = L""	/* AppName */
	);

	// ������Ϸ
	static int run();

	// ��ͣ��Ϸ
	static void pause();

	// ������Ϸ
	static void resume();

	// ������Ϸ
	static void quit();

	// ������Ϸ��Դ
	static void uninit();

	// ��Ϸ�Ƿ���ͣ
	static bool isPaused();

	// ��ȡ AppName
	static String getAppName();
};


// ���ƴ�������
class Window
{
	friend Game;

public:
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

	// �޸Ĵ��ڴ�С
	static void setSize(
		UINT32 nWidth,
		UINT32 nHeight
	);

	// ���ô��ڱ���
	static void setTitle(
		const String & sTitle
	);

	// ��/���ؿ���̨
	static void showConsole(
		bool bShow = true
	);

	// �Ƿ�������Ӧ���뷨
	static void setTypewritingEnable(
		bool bEnable
	);

private:
	// ��ʼ������
	static bool __init(
		const String & sTitle,
		UINT32 nWidth,
		UINT32 nHeight,
		LPCTSTR pIconID
	);

	// �������̻�����
	static bool __initMutex(
		const String & sTitle
	);

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
	// ��ȡ��һ֡�뵱ǰ֡��ʱ���������룩
	static int getDeltaTime();

	// ��ȡ��Ϸ��ʼʱ�����룩
	static double getTotalTime();

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
	friend Listener;

public:
	// ����������
	static void add(
		ListenerCallback callback,	/* �ص����� */
		const String & name = L""	/* ���������� */
	);

	// �����������
	static void start(
		const String & name
	);

	// ֹͣ�������
	static void stop(
		const String & name
	);

	// ����������
	static void clear(
		const String & name
	);

	// �������м�����
	static void startAll();

	// ֹͣ���м�����
	static void stopAll();

	// ������м�����
	static void clearAll();

	// ��ȡ������
	static std::vector<Listener*> get(
		const String & name
	);

	// ��ȡȫ��������
	static std::vector<Listener*> getAll();

	// ������ĳ�����Ƿ���������
	static bool isKeyDown(
		int nKeyCode
	);

	// ������ĳ�����Ƿ񱻵��
	static bool isKeyPress(
		int nKeyCode
	);

	// ������ĳ�����Ƿ������ɿ�
	static bool isKeyRelease(
		int nKeyCode
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

	// ����������
	static void __add(
		Listener * pListener
	);

	// ж�� DirectInput
	static void __uninit();
};


// ��Ⱦ��
class Renderer
{
	friend Game;
	friend Window;

public:
	// �޸ı���ɫ
	static void setBackgroundColor(
		UINT32 color
	);

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