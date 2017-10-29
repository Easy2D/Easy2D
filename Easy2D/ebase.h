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
class EAction;
class ETransition;

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
		EWindowStyle wStyle,	/* ������ʽ */
		bool showConsole = false/* �Ƿ���ʾ����̨ */
	);

	// ��������
	void run();

	// ��д��������������ڴ��ڼ���ʱִ��
	virtual bool onActivate();

	// ��д��������������ڴ��ڷǼ���ʱִ��
	virtual bool onInactive();

	// ��д��������������ڹرմ���ʱִ��
	virtual bool onCloseWindow();

	// ��ȡ����ʵ��
	static EApp * get();

	// ��ͣ��Ϸ
	static void pause();

	// ������Ϸ
	static void resume();

	// ��ȡ��Ϸ�Ƿ���ͣ
	static bool isPaused();

	// �л�����
	static void enterScene(
		EScene * scene,					/* ��һ��������ָ�� */
		bool saveCurrentScene = true	/* �Ƿ񱣴浱ǰ���� */
	);

	// �л�����
	static void enterScene(
		EScene * scene,					/* ��һ��������ָ�� */
		ETransition * transition,		/* �����л����� */
		bool saveCurrentScene = true	/* �Ƿ񱣴浱ǰ���� */
	);

	// ������һ����
	static void backScene();

	// ������һ����
	static void backScene(
		ETransition * transition		/* �����л����� */
	);

	// ��ձ�������г���
	static void clearScene();

	// ���ش���
	static void hideWindow();

	// ��ʾ����
	static void showWindow();

	// �Ƿ�򿪿���̨
	static void showConsole(
		bool show
	);

	// ��ֹ����
	static void quit();

	// ��ֹ����
	static void end();

	// ��ȡ���ڱ���
	static EString getTitle();

	// ��ȡ���ڿ��
	static float getWidth();

	// ��ȡ���ڸ߶�
	static float getHeight();

	// ��ȡ��ǰ����
	static EScene * getCurrentScene();

	// ��ȡ���ھ��
	static HWND getHWnd();

	// ��ȡ����Ϸ��ʼ����ǰ�����ĺ�����
	static LONGLONG getTotalDurationFromStart();

	// ��ȡ AppName
	static EString getAppName();

	// �޸Ĵ��ڴ�С
	static void setWindowSize(
		UINT32 width,
		UINT32 height
	);

	// ���ô��ڱ���
	static void setWindowTitle(
		const EString & title
	);

	// ���� AppName
	static void setAppName(
		const EString &appname
	);

	// �޸Ĵ��ڱ���ɫ
	static void setBkColor(
		UINT32 color
	);

	// ���ó����Ƿ���Ӧ���뷨
	static void setKeyboardLayoutEnable(
		bool value
	);

	// Ԥ�軭��֡��
	static void setFPS(
		UINT32 fps
	);

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
	void _onRender();

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
	bool	m_bEnd;
	bool	m_bPaused;
	bool	m_bManualPaused;
	bool	m_bTransitional;
	bool	m_bTopMost;
	EString	m_sTitle;
	EString	m_sAppName;
	UINT32	m_ClearColor;
	LONGLONG nAnimationInterval;
	EScene * m_pCurrentScene;
	EScene * m_pNextScene;
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


class EScene :
	public EObject
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
	virtual bool onActivate();

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

	// ��ȡ���ڵ�
	ENode * getRoot() const;

	// ��������ӳ�Ա
	void clearAllChildren();

	// ִ�ж���
	void runAction(
		EAction * action
	);

	// ��������ͼ�ε���Ⱦ
	void setGeometryVisiable(
		bool visiable
	);

protected:
	// ��Ⱦ��������
	void _onRender();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bGeometryVisiable;
	ENode * m_pRoot;
};

}