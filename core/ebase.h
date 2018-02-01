#pragma once
#include "emacros.h"
#include "ecommon.h"


// Base Classes

namespace e2d
{


class EGame
{
public:
	// ��ʼ����Ϸ
	static bool init(
		LPCTSTR sTitle,				/* ���ڱ��� */
		UINT32 nWidth,				/* ���ڿ�� */
		UINT32 nHeight,				/* ���ڸ߶� */
		LPCTSTR pIconID = nullptr,	/* ����ͼ�� */
		LPCTSTR sAppname = nullptr	/* AppName */
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
	static EString getAppName();

private:
	// ������Ϸ����
	static void __update();
};


// ���ƴ�������
class EWindow
{
	friend EGame;

public:
	// ��ȡ���ڱ���
	static EString getTitle();

	// ��ȡ���ڿ��
	static float getWidth();

	// ��ȡ���ڸ߶�
	static float getHeight();

	// ��ȡ���ڴ�С
	static ESize getSize();

	// ��ȡ���ھ��
	static HWND getHWnd();

	// �޸Ĵ��ڴ�С
	static void setSize(
		UINT32 nWidth,
		UINT32 nHeight
	);

	// ���ô��ڱ���
	static void setTitle(
		const EString & sTitle
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
		LPCTSTR sTitle,
		UINT32 nWidth,
		UINT32 nHeight,
		LPCTSTR pIconID
	);

	// ���ô�������
	static void __uninit();

	// ��������Ϣ
	static void __poll();

	// Win32 ������Ϣ�ص�����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// ������Ϸʱ��
class ETime
{
	friend EGame;

public:
	// ��ȡ��һ֡�뵱ǰ֡��ʱ���������룩
	static int getDeltaTime();

	// ��ȡ��Ϸ��ʼʱ�����룩
	static float getTotalTime();

private:
	// ��ʼ����ʱ����
	static bool __init();

	// ���ü�ʱ����
	static void __uninit();

	// ���µ�ǰʱ��
	static void __updateNow();

	// ����ʱ����Ϣ
	static void __updateLast();

	// �����߳�
	static void __sleep();
};


// ���Ƽ��̺���������
class EInput
{
	friend EGame;

public:
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
	static float getMouseX();

	// ������Y������ֵ
	static float getMouseY();

	// ����������ֵ
	static EPoint getMousePos();

	// ������X����������
	static float getMouseDeltaX();

	// ������Y����������
	static float getMouseDeltaY();

	// ������Z�ᣨ�����֣���������
	static float getMouseDeltaZ();

private:
	// ��ʼ�� DirectInput �Լ���������豸
	static bool __init();

	// ���������Ϣ
	static void __updateDeviceState();

	// ж�� DirectInput
	static void __uninit();
};


// ��Ⱦ��
class ERenderer
{
	friend EGame;
	friend EWindow;

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