#pragma once
#include "ebase.h"

namespace e2d
{

class EMsgManager;


class EObjectManager
{
	friend EApp;

public:
	// ��һ���ڵ�����ͷų�
	static void add(
		e2d::EObject * nptr
	);

	// ɾ�����нڵ�
	static void clearAllObjects();

private:
	// ˢ���ڴ��
	static void __flush();
};


class EMouseMsg
{
	friend EMsgManager;

public:
	// �����Ϣ����
	enum MSG
	{
		MOVE = 0x0200,	// ����ƶ�
		LBUTTON_DOWN,	// ����������
		LBUTTON_UP,		// ������̧��
		LBUTTON_DBLCLK,	// ������˫��
		RBUTTON_DOWN,	// ����Ҽ�����
		RBUTTON_UP,		// ����Ҽ�̧��
		RBUTTON_DBLCLK,	// ����Ҽ�˫��
		MBUTTON_DOWN,	// ����м�����
		MBUTTON_UP,		// ����м�̧��
		MBUTTON_DBLCLK,	// ����м�˫��
		WHEEL			// ��������
	};

	// ��ȡ��������
	static DWORD getX();

	// ��ȡ���������
	static DWORD getY();

	// ��ȡ�������
	static EPoint getPos();

	// ��ȡ����������״̬
	static bool isLButtonDown();

	// ��ȡ����м�����״̬
	static bool isMButtonDown();

	// ��ȡ����Ҽ�����״̬
	static bool isRButtonDown();

	// ��ȡ Shift ����״̬
	static bool isShiftDown();

	// ��ȡ Ctrl ����״̬
	static bool isCtrlDown();

	// ��ȡ������ֵ
	static DWORD getWheelDelta();

	// ��ȡ��ǰ�����Ϣ
	static MSG getMsg();

protected:
	UINT m_nMsg = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};


class EListener :
	public EObject
{
public:
	EListener();

	EListener(
		EString name
	);

	// ��ȡ������״̬
	bool isRunning() const;

	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ����ȴ�״̬
	void wait();

	// ����
	void notify();

	// ��ȡ����������
	EString getName() const;

	// ��ȡ���������ڳ���
	EScene * getParentScene() const;

	// ���ü���������
	void setName(
		EString name
	);

	// ���ü��������ڳ���
	void setParentScene(
		EScene * scene
	);

protected:
	EString				m_sName;
	bool				m_bRunning;
	bool				m_bWaiting;
	EScene *			m_pParentScene;
};


class EMouseListener :
	public EListener
{
public:
	EMouseListener();

	EMouseListener(
		EString name
	);

	EMouseListener(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EMouseListener(
		EString name,
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// ִ�м������ص�����
	virtual void runCallback();

	// ���ü������ص�����
	void setCallback(const MOUSE_LISTENER_CALLBACK &callback);

protected:
	MOUSE_LISTENER_CALLBACK m_callback;
};


class EMouseClickListener :
	public EMouseListener
{
public:
	EMouseClickListener();

	EMouseClickListener(
		EString name
	);

	EMouseClickListener(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EMouseClickListener(
		EString name,
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// ִ�м������ص�����
	virtual void runCallback() override;
};


class EMouseDraggedListener :
	public EMouseListener
{
public:
	EMouseDraggedListener();

	EMouseDraggedListener(
		EString name
		);

	EMouseDraggedListener(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
		);

	EMouseDraggedListener(
		EString name,
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
		);

	// ִ�м������ص�����
	virtual void runCallback() override;

	// ���ü������ص�����
	void setCallback(const MOUSE_DRAG_LISTENER_CALLBACK &callback);

protected:
	EPoint m_Begin;
	EPoint m_End;
	MOUSE_DRAG_LISTENER_CALLBACK m_callback;
};


class EKeyMsg
{
public:
	// �����Ϣ����
	enum MESSAGE
	{
		MOVE = 0x0200,	// ����ƶ�
		LBUTTON_DOWN,	// ����������
		LBUTTON_UP,		// ������̧��
		LBUTTON_DBLCLK,	// ������˫��
		RBUTTON_DOWN,	// ����Ҽ�����
		RBUTTON_UP,		// ����Ҽ�̧��
		RBUTTON_DBLCLK,	// ����Ҽ�˫��
		MBUTTON_DOWN,	// ����м�����
		MBUTTON_UP,		// ����м�̧��
		MBUTTON_DBLCLK,	// ����м�˫��
		WHEEL			// ��������
	};
};


class EKeyListener :
	public EListener
{
public:
	EKeyListener();

	EKeyListener(
		EString name
		);

	EKeyListener(
		const KEY_LISTENER_CALLBACK &callback
		);

	EKeyListener(
		EString name,
		const KEY_LISTENER_CALLBACK &callback
		);

	// ִ�м������ص�����
	void runCallback();

	// ���ü������ص�����
	void setCallback(const KEY_LISTENER_CALLBACK &callback);

protected:
	KEY_LISTENER_CALLBACK	m_callback;
};


class EMsgManager
{
	friend EApp;

public:
	static void MouseProc(
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	);

	static void KeyboardProc(
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	);

	static void addListener(
		EMouseListener * listener
	);

private:
	
	static void __exec();
};

}