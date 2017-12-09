#pragma once
#include "emacros.h"
#include "ecommon.h"


// Base Classes

namespace e2d
{

class EScene;
class ENode;
class EObjectManager;
class EListenerMouse;
class EListenerKeyboard;
class EAction;
class ETransition;

class EApp
{
public:
	// ��ȡ����ʵ��
	static EApp * getInstance();

	// ��ʼ����Ϸ����
	static bool init(
		const EString &title,	/* ���ڱ��� */
		UINT32 width,			/* ���ڿ�� */
		UINT32 height			/* ���ڸ߶� */
	);

	// ��ʼ����Ϸ����
	static bool init(
		const EString &title,	/* ���ڱ��� */
		UINT32 width,			/* ���ڿ�� */
		UINT32 height,			/* ���ڸ߶� */
		EWindowStyle wStyle		/* ������ʽ */
	);

	// ��������
	static int run();

	// ��ͣ��Ϸ
	static void pause();

	// ������Ϸ
	static void resume();

	// ������Ϸ
	static void quit();

	// �л�����
	static void enterScene(
		EScene * scene,						/* ��һ��������ָ�� */
		ETransition * transition = nullptr,	/* �����л����� */
		bool saveCurrentScene = true		/* �Ƿ񱣴浱ǰ���� */
	);

	// ������һ����
	static void backScene(
		ETransition * transition = nullptr	/* �����л����� */
	);

	// ��ձ�������г���
	static void clearScene();

	// ���ش���
	static void hideWindow();

	// ��ʾ����
	static void showWindow();

	// �Ƿ�򿪿���̨
	static void showConsole(
		bool show = true
	);

	// ��Ϸ�Ƿ���ͣ
	static bool isPaused();

	// ��ȡ���ڱ���
	static EString getTitle();

	// ��ȡ���ڿ��
	static float getWidth();

	// ��ȡ���ڸ߶�
	static float getHeight();

	// ��ȡ���ڴ�С
	static ESize getSize();

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

public:
	// ��д��������������ڴ��ڼ���ʱִ��
	virtual bool onActivate();

	// ��д��������������ڴ��ڷǼ���ʱִ��
	virtual bool onInactive();

	// ��д��������������ڹرմ���ʱִ��
	virtual bool onCloseWindow();

private:
	EApp();

	virtual ~EApp();

	// ��Ϸ��ѭ��
	void _mainLoop();

	// ��Ϸ��������
	void _update();

	// ��Ⱦ��Ϸ����
	void _render();

	// ������һ����
	void _enterNextScene();

	// ˢ����Ϸʱ��
	void _updateTime();

	// ���ڳ���
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

private:
	bool	m_bEnd;
	bool	m_bPaused;
	bool	m_bManualPaused;
	bool	m_bTopMost;
	bool	m_bShowConsole;
	EString	m_sTitle;
	EString	m_sAppName;
	UINT32	m_ClearColor;
	LONGLONG m_nAnimationInterval;
	EScene * m_pCurrentScene;
	EScene * m_pNextScene;
	ETransition * m_pTransition;
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
		ENode * child
	);

	// ɾ����ͬ���Ƶ��ӽڵ�
	void remove(
		const EString &childName
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
	void _render();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bGeometryVisiable;
	ENode * m_pRoot;
};

}