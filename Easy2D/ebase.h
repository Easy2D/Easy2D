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
class EMouseListener;
class EKeyboardListener;

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
	static void setWindowSize(
		int width,
		int height
	);

	// �޸Ĵ��ڴ�С
	static void setWindowSize(
		e2d::ESize size
	);

	// ���ô��ڱ���
	static void setWindowTitle(
		e2d::EString title
	);

	// ��ȡ���ڱ���
	static e2d::EString getTitle();

	// ��ȡ���ڴ�С
	static e2d::ESize getSize();

	// ��ȡ���ڿ��
	static UINT32 getWidth();

	// ��ȡ���ڸ߶�
	static UINT32 getHeight();

	// �л�����
	static void enterScene(
		EScene * scene,
		bool save = true
	);

	// ������һ����
	static void backScene();

	// ��ձ�������г���
	static void clearScene();

	// ��ȡ��ǰ����
	static EScene * getCurrentScene();

	// ��ȡ AppName
	static e2d::EString getAppName();

	// ���� AppName
	static void setAppName(
		e2d::EString appname
	);

	// �޸Ĵ��ڱ���ɫ
	static void setBkColor(
		EColor::Enum color
	);

	// �ͷ������ڴ���Դ
	static void free();

	// �رմ���
	static void close();

	// ��ʾ����
	static void show();

	// ��ֹ����
	static void quit();

	// ��ֹ����
	static void end();

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
	void _onRender();

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
	EString	m_sTitle;
	EString	m_sAppName;
	EColor::Enum m_ClearColor;

	EScene *	m_pCurrentScene;
	EScene *	m_pNextScene;
};


class EScene
{
	friend EApp;

public:
	EScene();

	~EScene();

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
	bool remove(
		e2d::ENode * child,
		bool autoRelease = true
	);

	// ��ȡ�����ӽڵ�
	std::vector<e2d::ENode*> &getChildren();

	// ��ȡ�ӽڵ�����
	size_t getChildrenCount() const;

	// �������ƻ�ȡ�ӽڵ�
	ENode * getChild(
		EString childName
	) const;

	// ��������ӳ�Ա
	void clearAllChildren();

	// �������Ϣ������
	void bindListener(EMouseListener * listener);

	// �󶨰�����Ϣ������
	void bindListener(EKeyboardListener * listener);

protected:
	// ��Ⱦ��������
	void _onRender();

	// �ӽڵ�����
	void _sortChildren();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	std::vector<e2d::ENode*> m_vChildren;
};


class EObject
{
	friend EObjectManager;

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
	int m_nRefCount;
	bool m_bManaged;
	bool m_bAutoRelease;
};

}