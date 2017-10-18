#pragma once
#include "ebase.h"

namespace e2d
{

class ENode;
class EMsgManager;

// �����Ϣ
class EMouseMsg
{
	friend EMsgManager;

public:
	// �����Ϣ����
	enum MOUSE_MSG
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
	static DWORD getPosX();

	// ��ȡ���������
	static DWORD getPosY();

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


// ������Ϣ
class EKeyboardMsg
{
	friend EMsgManager;

public:
	// ������Ϣ���ͼ���
	enum KEYBOARD_MSG
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


// ������
class EListener :
	public EObject
{
	friend EMsgManager;

public:
	EListener();

	EListener(
		const EString &name
	);

	// ��ȡ������״̬
	bool isRunning() const;

	// ��ȡ����������״̬
	bool isWaiting() const;

	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ��ȡ����������
	EString getName() const;

	// ��ȡ���������ڳ���
	EScene * getParentScene() const;

	// ��ȡ���������ڽڵ�
	ENode * getParentNode() const;

	// ���ü���������
	void setName(
		const EString &name
	);

	// �󶨼�����������
	virtual void bindWith(
		EScene * pParentScene
	) = 0;

	// �󶨼��������ڵ�
	virtual void bindWith(
		ENode * pParentNode
	) = 0;

protected:
	// ����ȴ�״̬
	void _wait();

	// ����
	void _notify();

	// ִ�м������ص�����
	virtual void _callOn() = 0;

protected:
	EString		m_sName;
	bool		m_bRunning;
	bool		m_bWaiting;
	EScene *	m_pParentScene;
	ENode *		m_pParentNode;
};


// �����Ϣ������
class EMouseListener :
	public EListener
{
	friend EMsgManager;

public:
	EMouseListener();

	EMouseListener(
		const EString &name
	);

	EMouseListener(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EMouseListener(
		const EString &name,
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	// �󶨼�����������
	virtual void bindWith(
		EScene * pParentScene
	) override;

	// �󶨼��������ڵ�
	virtual void bindWith(
		ENode * pParentNode
	) override;

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	MOUSE_LISTENER_CALLBACK m_Callback;
};


// ��갴����Ϣ������
class EMousePressListener :
	public EMouseListener
{
public:
	EMousePressListener();

	EMousePressListener(
		const EString &name
	);

	EMousePressListener(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	EMousePressListener(
		const EString &name,
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	MOUSE_PRESS_LISTENER_CALLBACK m_Callback;
};


// �������Ϣ������
class EMouseClickListener :
	public EMouseListener
{
public:
	EMouseClickListener();

	EMouseClickListener(
		const EString &name
	);

	EMouseClickListener(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	EMouseClickListener(
		const EString &name,
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	bool m_bPressed;
	MOUSE_CLICK_LISTENER_CALLBACK m_Callback;
};


// �������Ϣ������
class EMouseDoubleClickListener :
	public EMouseListener
{
public:
	EMouseDoubleClickListener();

	EMouseDoubleClickListener(
		const EString &name
	);

	EMouseDoubleClickListener(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	EMouseDoubleClickListener(
		const EString &name,
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	bool m_bPressed;
	MOUSE_DBLCLK_LISTENER_CALLBACK m_Callback;
};


// ����϶���Ϣ������
class EMouseDragListener :
	public EMouseListener
{
public:
	EMouseDragListener();

	EMouseDragListener(
		const EString &name
	);

	EMouseDragListener(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	EMouseDragListener(
		const EString &name,
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	EPoint	m_Begin;
	MOUSE_DRAG_LISTENER_CALLBACK m_Callback;
};


// ������Ϣ������
class EKeyboardListener :
	public EListener
{
	friend EMsgManager;

public:
	EKeyboardListener();

	EKeyboardListener(
		const EString &name
	);

	EKeyboardListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyboardListener(
		const EString &name,
		const KEY_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const KEY_LISTENER_CALLBACK &callback
	);

	// �󶨼�����������
	virtual void bindWith(
		EScene * pParentScene
	) override;

	// �󶨼��������ڵ�
	virtual void bindWith(
		ENode * pParentNode
	) override;

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	KEY_LISTENER_CALLBACK m_Callback;
};


// ����������Ϣ����
class EKeyboardPressListener :
	public EKeyboardListener
{
	friend EMsgManager;

public:
	EKeyboardPressListener();

	EKeyboardPressListener(
		const EString &name
	);

	EKeyboardPressListener(
		const KEY_LISTENER_CALLBACK &callback
	);

	EKeyboardPressListener(
		const EString &name,
		const KEY_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;
};


// ��Ϣ������
class EMsgManager
{
	friend EApp;
	friend EScene;
	friend ENode;

public:
	// �������Ϣ������������
	static void bindListener(
		EMouseListener * listener,
		EScene * pParentScene
	);

	// �������Ϣ���������ڵ�
	static void bindListener(
		EMouseListener * listener,
		ENode * pParentNode
	);

	// ����������ͬ���Ƶ������Ϣ������
	static void startMouseListeners(
		const EString &name
	);

	// ֹͣ������ͬ���Ƶ������Ϣ������
	static void stopMouseListeners(
		const EString &name
	);

	// ɾ��������ͬ���Ƶ������Ϣ������
	static void delMouseListeners(
		const EString &name
	);

	// �������ڳ��������ӽڵ��ϵ����������Ϣ������
	static void startAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// ֹͣ���ڳ��������ӽڵ��ϵ����������Ϣ������
	static void stopAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// ������ڳ��������ӽڵ��ϵ����������Ϣ������
	static void clearAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// �������ڽڵ��ϵ����������Ϣ������
	static void startAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����������Ϣ������
	static void stopAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// ������ڽڵ��ϵ����������Ϣ������
	static void clearAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// �������������Ϣ������
	static void startAllMouseListeners();

	// ֹͣ���������Ϣ������
	static void stopAllMouseListeners();

	// ������������Ϣ������
	static void clearAllMouseListeners();

	// �󶨰�����Ϣ������������
	static void bindListener(
		EKeyboardListener * listener,
		EScene * pParentScene
	);

	// �󶨰�����Ϣ���������ڵ�
	static void bindListener(
		EKeyboardListener * listener,
		ENode * pParentNode
	);

	// ����������ͬ�İ�����Ϣ������
	static void startKeyboardListeners(
		const EString &name
	);

	// ֹͣ������ͬ�İ�����Ϣ������
	static void stopKeyboardListeners(
		const EString &name
	);

	// ɾ��������ͬ�İ�����Ϣ������
	static void delKeyboardListeners(
		const EString &name
	);

	// �������ڳ��������ӽڵ��ϵ����а�����Ϣ������
	static void startAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// ֹͣ���ڳ��������ӽڵ��ϵ����а�����Ϣ������
	static void stopAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// ������ڳ��������ӽڵ��ϵ����а�����Ϣ������
	static void clearAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// �������ڽڵ��ϵ����а�����Ϣ������
	static void startAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����а�����Ϣ������
	static void stopAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// ������ڽڵ��ϵ����а�����Ϣ������
	static void clearAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// �������а�����Ϣ������
	static void startAllKeyboardListeners();

	// ֹͣ���а�����Ϣ������
	static void stopAllKeyboardListeners();

	// ������а�����Ϣ������
	static void clearAllKeyboardListeners();

private:
	// ������ڳ����ϵ����������Ϣ������
	static void _waitAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// �������ڳ����ϵ����������Ϣ������
	static void _notifyAllMouseListenersBindedWith(
		EScene * pParentScene
	);

	// ������ڽڵ��ϵ����������Ϣ������
	static void _waitAllMouseListenersBindedWith(
		ENode * pParentNode
	);

	// �������ڽڵ��ϵ����������Ϣ������
	static void _notifyAllMouseListenersBindedWith(
		ENode * pParentNode
	);


	// ������ڳ��������ӽڵ��ϵ����а���������
	static void _waitAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// �������ڳ��������ӽڵ��ϵ����а���������
	static void _notifyAllKeyboardListenersBindedWith(
		EScene * pParentScene
	);

	// ������ڽڵ��ϵ����а���������
	static void _waitAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// �������ڽڵ��ϵ����а���������
	static void _notifyAllKeyboardListenersBindedWith(
		ENode * pParentNode
	);

	// �����Ϣ����
	static void MouseProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	// ������Ϣ����
	static void KeyboardProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
};

}