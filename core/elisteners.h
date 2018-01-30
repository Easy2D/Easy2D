#pragma once
#include "ebase.h"
#include "egeometry.h"

namespace e2d
{

class ENode;
class EPhysicsManager;

// ������
class EListener :
	public EObject
{
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

	// ���ü�����������Ϣ
	void setSwallow(
		bool bSwallow
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

	// ��ȡ������״̬�Ƿ����
	virtual bool _isReady() const;

protected:
	EString		m_sName;
	bool		m_bRunning;
	bool		m_bAlways;
	bool		m_bSwallow;
	ENode *		m_pParentNode;
};


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
		const PhysLsnrCallback &callback
	);

	EListenerPhysics(
		const EString &name,
		const PhysLsnrCallback &callback
	);

	// ���ü������ص�����
	void setCallback(
		const PhysLsnrCallback &callback
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
	PhysLsnrCallback m_Callback;
};


// �������ͻ��Ϣ������
class EListenerPhysicsCollision :
	public EListenerPhysics
{
public:
	EListenerPhysicsCollision();

	EListenerPhysicsCollision(
		const EString &name
	);

	EListenerPhysicsCollision(
		const ClsLsnrCallback &callback
	);

	EListenerPhysicsCollision(
		const EString &name,
		const ClsLsnrCallback &callback
	);

protected:
	// ִ�м������ص�����
	virtual void _callOn() override;

protected:
	ClsLsnrCallback m_Callback;
};

}