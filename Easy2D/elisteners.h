#pragma once
#include "ebase.h"

namespace e2d
{

class ENode;
class EMsgManager;


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

	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ��ȡ����������
	EString getName() const;

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
	// ִ�м������ص�����
	virtual void _callOn() = 0;

protected:
	EString		m_sName;
	bool		m_bRunning;
	bool		m_bAlways;
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

}