/******************************************************
* Easy2D Game Engine
* 
* Website: http://www.easy2d.cn
* Source Code: https://gitee.com/werelone/Easy2D
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows 7 or later.
#define WINVER 0x0700       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0700 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used items from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <wchar.h>

#include "emacros.h"
#include "ecommon.h"


#if defined(UNICODE) && (_DEBUG)
	#pragma comment(lib, "Easy2Ddw.lib")
#elif !defined(UNICODE) && (_DEBUG)
	#pragma comment(lib, "Easy2Dd.lib")
#elif defined(UNICODE)
	#pragma comment(lib, "Easy2Dw.lib")
#elif !defined(UNICODE)
	#pragma comment(lib, "Easy2D.lib")
#endif


// Classes Declare

namespace e2d
{
	class EApp;
	class Scene;
	class Object;
}


// Classes

namespace e2d
{

class EApp
{
public:
	EApp();
	~EApp();

	// ��ȡ����ʵ��
	static EApp * get();

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		EString title,
		ESize size,
		bool bShowConsole = false
	);

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		EString title,
		UINT32 width,
		UINT32 height,
		bool bShowConsole = false
	);

	// ��������
	void run();

	// �޸Ĵ��ڴ�С
	void setWindowSize(
		int width, 
		int height
	);

	// �޸Ĵ��ڴ�С
	void setWindowSize(
		ESize size
	);

	// ���ô��ڱ���
	void setWindowTitle(
		EString title
	);

	// ��ȡ���ڱ���
	EString getTitle();

	// ��ȡ���ڿ��
	int getWidth();

	// ��ȡ���ڸ߶�
	int getHeight();
	
	// �л�����
	void enterScene(
		Scene * scene, 
		bool save = true
	);

	// ������һ����
	void backScene();

	// ��ձ�������г���
	void clearScene();

	// ��ȡ��ǰ����
	Scene * getCurrentScene();

	// ��ȡ�����ڼ����еĳ���
	Scene * getLoadingScene();

	// ���� AppName
	void setAppName(
		EString appname
	);

	// ��ȡ AppName
	EString getAppName();

	// �޸Ĵ��ڱ���ɫ
	void setBkColor(
		EColor::Enum color
	);

	// �ͷ������ڴ���Դ
	void free();

	// �رմ���
	void close();

	// ��ʾ����
	void show();

	// ��ֹ����
	void quit();

	// ��ֹ����
	void end();

protected:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	void _mainLoop();

	void _onControl();

	// Draw content.
	bool _onRender();

	void _enterNextScene();

	// Resize the render target.
	void _onResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

protected:
	bool	m_bRunning;
	bool	m_bSaveScene;
	EString	m_sTitle;
	EString	m_sAppName;
	EColor::Enum	m_ClearColor;
	
	Scene *	m_pCurrentScene;
	Scene *	m_pNextScene;
	Scene *	m_pLoadingScene;
};

class Scene
{
	friend EApp;
public:
	Scene();
	~Scene();

	// ��д�����������ʼ���������
	virtual void init();
	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter();
	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit();
	// ����ӳ�Ա������
	void add(Node * child, int zOrder = 0);
	// ɾ���ӳ�Ա
	bool del(Node * child);
	// ��������ӳ�Ա
	void clearAllChildren();

protected:
	//std::vector<Node*> m_vChildren;

protected:
	void _exec();
	void _onDraw();
};

class Object
{
	friend FreePool;
public:
	Object();
	virtual ~Object();

	// �����������
	void retain();
	// �ͷ��������
	void release();
	// �������Զ��ͷ��������
	void autoRelease();

protected:
	int m_nRefCount;
	bool m_bAutoRelease;
};

}	// End of easy2d namespace


// Functions Declare

using namespace e2d;