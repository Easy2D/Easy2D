#pragma once
#include "ebase.h"

namespace e2d
{

class EMsgManager;


// �����Ϣ
class EMouseMsg
{
	friend EMsgManager;

public:
	// �����Ϣ����
	enum class MOUSE_MSG
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
	static MOUSE_MSG getMsg();

protected:
	UINT m_nMsg = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};


// ������
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


// �����Ϣ������
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
	void setCallback(
		const MOUSE_LISTENER_CALLBACK &callback
	);

protected:
	MOUSE_LISTENER_CALLBACK m_callback;
};


// �������Ϣ������
class EMouseClickListener :
	public EMouseListener
{
public:
	EMouseClickListener();

	EMouseClickListener(
		EString name
	);

	EMouseClickListener(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	EMouseClickListener(
		EString name,
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	// ִ�м������ص�����
	virtual void runCallback() override;

	// ���ü������ص�����
	void setCallback(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

protected:
	MOUSE_CLICK_LISTENER_CALLBACK m_callback;
};


// ����϶���Ϣ������
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
	void setCallback(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

protected:
	EPoint m_Begin;
	EPoint m_End;
	MOUSE_DRAG_LISTENER_CALLBACK m_callback;
};


// ������Ϣ
class EKeyMsg
{
	friend EMsgManager;

public:
	// ������Ϣ���ͼ���
	enum class KEYBOARD_MSG
	{
		KEY_DOWN = 0x0100,	// ����
		KEY_UP				// ̧��
	};

	// ������ֵ����
	enum class KEY
	{
		A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,					// ��ĸ��ֵ
		NUM0 = '0', NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,									// ���ּ�ֵ
		NUMPAD0 = 0x60, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,	// ����С���̼�ֵ
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,											// F����ֵ
		MULTIPLY,		// �˺ż���ֵ
		ADD,			// �Ӻż���ֵ
		SEPARATOR,		// �ָ����ֵ
		SUBTRACT,		// ���ż���ֵ
		DECIMAL,		// С�������ֵ
		DIVIDE,			// ���ż���ֵ
		TAB = 0x09,		// TAB ����ֵ
		ENTER = 0x0D,	// �س�����ֵ
		SHIFT, CTRL, 	// SHIFT ����ֵ
		ESC = 0x1B, 	// ESCAPE ����ֵ
		SPACE = 0x20, 	// �ո����ֵ
		PAGE_UP, 		// PageUp ����ֵ
		PAGE_DOWN, 		// PageDown ����ֵ
		END, 			// End ����ֵ
		HOME, 			// Home ����ֵ
		LEFT, 			// �����ֵ
		UP, 			// �ϼ���ֵ
		RIGHT, 			// �Ҽ���ֵ
		DOWN			// �¼���ֵ
	};

	// ��ȡ������Ϣ����
	static KEYBOARD_MSG getMsg();

	// ��ȡ��ֵ
	static KEY getVal();

	// ��ȡ������Ϣ�ļ���
	static DWORD getCount();

	// ��ȡ�ض�������״̬
	static bool isKeyDown(
		KEY key
	);

	// ��ȡ��Сд����״̬
	static bool isCapitalLockOn();

	// ��ȡ����С��������״̬
	static bool isNumpadLockOn();

	// ��ȡ��������״̬
	static bool isScrollLockOn();

protected:
	UINT m_nMsg = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};


// ������Ϣ������
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
	virtual void runCallback();

	// ���ü������ص�����
	void setCallback(
		const KEY_LISTENER_CALLBACK &callback
	);

protected:
	KEY_LISTENER_CALLBACK m_callback;
};


// ����������Ϣ����
class EKeyPressListener :
	public EKeyListener
{
public:
	EKeyPressListener();

	EKeyPressListener(
		EString name
	);

	EKeyPressListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyPressListener(
		EString name,
		const KEY_LISTENER_CALLBACK &callback
	);

	// ִ�м������ص�����
	virtual void runCallback() override;
};


// ��Ϣ������
class EMsgManager
{
	friend EApp;

public:
	// ��������Ϣ������
	static void addListener(
		EMouseListener * listener,
		EScene * pParentScene
	);

	// ��Ӱ�����Ϣ������
	static void addListener(
		EKeyListener * listener,
		EScene * pParentScene
	);

	// ����������ͬ���Ƶļ�����
	static void startListener(
		EString name
	);

	// ֹͣ������ͬ���Ƶļ�����
	static void stopListener(
		EString name
	);

	// ɾ��������ͬ���Ƶļ�����
	static void delListener(
		EString name
	);

	// ���𳡾��ϵ����м�����
	static void waitAllListenersOnScene(
		EScene * scene
	);

	// ���������ϵ����м�����
	static void notifyAllListenersOnScene(
		EScene * scene
	);

private:
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
};

}