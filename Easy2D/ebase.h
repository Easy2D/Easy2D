#pragma once
#include "emacros.h"
#include "ecommon.h"
#include <vector>

// Base Classes

namespace e2d
{

class EScene;
class ENode;
class EObjectManager;

class EApp
{
public:
	EApp();

	~EApp();

	// ��ȡ����ʵ��
	static EApp * get();

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		e2d::EString title,
		e2d::ESize size,
		bool bShowConsole = false
	);

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		e2d::EString title,
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
		e2d::ESize size
	);

	// ���ô��ڱ���
	void setWindowTitle(
		e2d::EString title
	);

	// ��ȡ���ڱ���
	e2d::EString getTitle();

	// ��ȡ���ڴ�С
	e2d::ESize getSize();

	// ��ȡ���ڿ��
	UINT32 getWidth();

	// ��ȡ���ڸ߶�
	UINT32 getHeight();

	// �л�����
	void enterScene(
		EScene * scene,
		bool save = true
	);

	// ������һ����
	void backScene();

	// ��ձ�������г���
	void clearScene();

	// ��ȡ��ǰ����
	EScene * getCurrentScene();

	// ��ȡ�����ڼ����еĳ���
	EScene * getLoadingScene();

	// ���������ڼ����еĳ���
	void setLoadingScene(EScene * scene);

	// ��ȡ AppName
	e2d::EString getAppName();

	// ���� AppName
	void setAppName(
		e2d::EString appname
	);

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
	HRESULT _createDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT _createDeviceResources();

	// Release device-dependent resource.
	void _discardDeviceResources();

	void _mainLoop();

	void _onControl();

	// Draw content.
	bool _onRender();

	void _enterNextScene();

	// ReSize the render target.
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
	EColor::Enum m_ClearColor;

	EScene *	m_pCurrentScene;
	EScene *	m_pNextScene;
	EScene *	m_pLoadingScene;
};


class EScene
{
public:

	EScene();

	~EScene();

	// ��д�����������ʼ���������
	virtual void init();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter();

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit();

	// ����ӳ�Ա������
	void add(
		e2d::ENode * child, 
		int zOrder = 0
	);

	// ɾ���ӳ�Ա
	bool del(
		e2d::ENode * child
	);

	// ��ȡ�����ӽڵ�
	std::vector<e2d::ENode*> &getChildren();

	// ��������ӳ�Ա
	void clearAllChildren();

protected:
	friend EApp;
	std::vector<e2d::ENode*> m_vChildren;

protected:
	void _exec();

	void _onDraw();
};


class EObject
{
public:
	EObject();

	virtual ~EObject();

	// ���ü�����һ
	void retain();

	// ���ü�����һ
	void release();

	// �������Զ��ͷ��������
	void autoRelease();

private:
	friend EObjectManager;
	int m_nRefCount;
	bool m_bManaged;
	bool m_bAutoRelease;
};

}