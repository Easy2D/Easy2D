#pragma once
#include "ebase.h"
#include "egeometry.h"

namespace e2d
{

class ENode;
class EMsgManager;
class EPhysicsManager;

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

	// ���ü���������Ϸ��ͣʱ��������
	void setAlwaysWorking(
		bool bAlways
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
class EListenerMouse :
	public EListener
{
	friend EMsgManager;

public:
	EListenerMouse();

	EListenerMouse(
		const EString &name
	);

	EListenerMouse(
		const MOUSE_LISTENER_CALLBACK &callback
	);

	EListenerMouse(
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
class EListenerMousePress :
	public EListenerMouse
{
public:
	EListenerMousePress();

	EListenerMousePress(
		const EString &name
	);

	EListenerMousePress(
		const MOUSE_PRESS_LISTENER_CALLBACK &callback
	);

	EListenerMousePress(
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
class EListenerMouseClick :
	public EListenerMouse
{
public:
	EListenerMouseClick();

	EListenerMouseClick(
		const EString &name
	);

	EListenerMouseClick(
		const MOUSE_CLICK_LISTENER_CALLBACK &callback
	);

	EListenerMouseClick(
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
class EListenerMouseDoubleClick :
	public EListenerMouse
{
public:
	EListenerMouseDoubleClick();

	EListenerMouseDoubleClick(
		const EString &name
	);

	EListenerMouseDoubleClick(
		const MOUSE_DBLCLK_LISTENER_CALLBACK &callback
	);

	EListenerMouseDoubleClick(
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
class EListenerMouseDrag :
	public EListenerMouse
{
public:
	EListenerMouseDrag();

	EListenerMouseDrag(
		const EString &name
	);

	EListenerMouseDrag(
		const MOUSE_DRAG_LISTENER_CALLBACK &callback
	);

	EListenerMouseDrag(
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
class EListenerKeyboard :
	public EListener
{
	friend EMsgManager;

public:
	EListenerKeyboard();

	EListenerKeyboard(
		const EString &name
	);

	EListenerKeyboard(
		const KEY_LISTENER_CALLBACK &callback
	);

	EListenerKeyboard(
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
class EListenerKeyboardPress :
	public EListenerKeyboard
{
	friend EMsgManager;

public:
	EListenerKeyboardPress();

	EListenerKeyboardPress(
		const EString &name
	);

	EListenerKeyboardPress(
		const KEY_LISTENER_CALLBACK &callback
	);

	EListenerKeyboardPress(
		const EString &name,
		const KEY_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;
};


// ����������Ϣ�������ص�����������������������������������ϵ��
typedef std::function<void(ENode *active, ENode *passive, int relation)> PHYSICS_LISTENER_CALLBACK;

// ��ײ��Ϣ�������ص�����������������������������
typedef std::function<void(ENode *active, ENode *passive)> COLLISION_LISTENER_CALLBACK;

// ����������Ϣ������
class EListenerPhysics :
	public EListener
{
	friend EPhysicsManager;

public:
	EListenerPhysics();

	EListenerPhysics(
		const EString &name
	);

	EListenerPhysics(
		const PHYSICS_LISTENER_CALLBACK &callback
	);

	EListenerPhysics(
		const EString &name,
		const PHYSICS_LISTENER_CALLBACK &callback
	);

	// ���ü������ص�����
	void setCallback(
		const PHYSICS_LISTENER_CALLBACK &callback
	);

	// ���������볡����
	virtual void bindWith(
		EScene * pParentScene
	) override;

	// ����������ڵ��
	virtual void bindWith(
		ENode * pParentNode
	) override;

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	PHYSICS_LISTENER_CALLBACK m_Callback;
};


class EListenerPhysicsContact :
	public EListenerPhysics
{
	friend EMsgManager;

public:
	EListenerPhysicsContact();

	EListenerPhysicsContact(
		const EString &name
	);

	EListenerPhysicsContact(
		const COLLISION_LISTENER_CALLBACK &callback
	);

	EListenerPhysicsContact(
		const EString &name,
		const COLLISION_LISTENER_CALLBACK &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	COLLISION_LISTENER_CALLBACK m_Callback;
};

}