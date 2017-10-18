#pragma once
#include "emacros.h"
#include "ecommon.h"


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

	virtual ~EApp();

	// ��ʼ����Ϸ����
	bool init(
		const EString &title,	/* ���ڱ��� */
		UINT32 width,			/* ���ڿ�� */
		UINT32 height,			/* ���ڸ߶� */
		bool showConsole = false/* �Ƿ���ʾ����̨ */
	);

	// ��ʼ����Ϸ����
	bool init(
		const EString &title,	/* ���ڱ��� */
		UINT32 width,			/* ���ڿ�� */
		UINT32 height,			/* ���ڸ߶� */
		int windowStyle,		/* ������ʽ */
		bool showConsole = false/* �Ƿ���ʾ����̨ */
	);

	// ��������
	void run();

	// Ԥ�軭��֡��
	void setFPS(
		UINT32 fps
	);

	// ��д��������������ڴ��ڼ���ʱִ��
	virtual void onActivate();

	// ��д��������������ڴ��ڷǼ���ʱִ��
	virtual bool onInactive();

	// ��д��������������ڹرմ���ʱִ��
	virtual bool onCloseWindow();

	// �ͷ������ڴ���Դ
	void free();

	// ��ȡ����ʵ��
	static EApp * get();

	// ��ʾ�����ؿ���̨��Ĭ�����أ�
	static void showConsole(
		bool show
	);

	// �޸Ĵ��ڴ�С
	static void setWindowSize(
		UINT32 width,
		UINT32 height
	);

	// ���ô��ڱ���
	static void setWindowTitle(
		const EString &title
	);

	// ��ȡ���ڱ���
	static EString getTitle();

	// ��ȡ���ڿ��
	static UINT32 getWidth();

	// ��ȡ���ڸ߶�
	static UINT32 getHeight();

	// �л�����
	static void enterScene(
		EScene * scene,
		bool saveCurrentScene = true
	);

	// ������һ����
	static void backScene();

	// ��ձ�������г���
	static void clearScene();

	// ��ȡ��ǰ����
	static EScene * getCurrentScene();

	// ��ȡ���ھ��
	static HWND getHWnd();

	// ��ȡ����Ϸ��ʼ����ǰ�����ĺ�����
	static LONGLONG getTotalDurationFromStart();

	// ��ȡ AppName
	static EString getAppName();

	// ���� AppName
	static void setAppName(
		const EString &appname
	);

	// �޸Ĵ��ڱ���ɫ
	static void setBkColor(
		EColor color
	);

	// ���ó����Ƿ���Ӧ���뷨
	static void setKeyboardLayoutEnable(
		bool value
	);

	// ���ش���
	static void hideWindow();

	// ��ʾ����
	static void showWindow();

	// ��ֹ����
	static void quit();

	// ��ֹ����
	static void end();

protected:
	// �����豸�޹���Դ
	HRESULT _createDeviceIndependentResources();

	// �����豸�����Դ
	HRESULT _createDeviceResources();

	// �ͷ��豸�����Դ
	void _discardDeviceResources();

	// ��Ϸ��ѭ��
	void _mainLoop();

	// ��Ϸ��������
	void _onControl();

	// ��Ⱦ��Ϸ����
	bool _onRender();

	// ������һ����
	void _enterNextScene();

	// �ض� render target ��С
	void _onResize(
		UINT32 width,
		UINT32 height
	);

	// ���ڳ���
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

protected:
	bool	m_bRunning;
	bool	m_bPaused;
	EString	m_sTitle;
	EString	m_sAppName;
	EColor	m_ClearColor;
	LONGLONG nAnimationInterval;

	EScene * m_pCurrentScene;
	EScene * m_pNextScene;
};


class EScene
{
	friend EApp;

public:
	EScene();

	virtual ~EScene();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter();

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit();

	// ��д��������������ڴ��ڼ���ʱִ��
	virtual void onActivate();

	// ��д��������������ڴ��ڷǼ���ʱִ��
	virtual bool onInactive();

	// ��д��������������ڹرմ���ʱִ��
	virtual bool onCloseWindow();

	// ����ӽڵ㵽����
	void add(
		ENode * child, 
		int zOrder = 0
	);

	// ɾ���ӽڵ�
	bool remove(
		ENode * child,
		bool release = false
	);

	// ��������ɾ���ӽڵ�
	void remove(
		const EString &childName,
		bool release = false
	);

	// ��ȡ�����ӽڵ�
	EVector<e2d::ENode*> &getChildren();

	// ��ȡ�ӽڵ�����
	size_t getChildrenCount() const;

	// �������ƻ�ȡ�ӽڵ�
	ENode * getChild(
		const EString &childName
	);

	// ��������ӳ�Ա
	void clearAllChildren();

	// �������Ϣ������
	void bindListener(EMouseListener * listener);

	// �󶨰�����Ϣ������
	void bindListener(EKeyboardListener * listener);

protected:
	// ��Ⱦ��������
	void _onRender();

	// ���볡��ʱ����øú���
	virtual void _onEnter();

	// �˳�����ʱ����øú���
	virtual void _onExit();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	ENode * const m_Root;
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