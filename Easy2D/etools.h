#pragma once
#include "ebase.h"
#include <chrono>

namespace e2d
{

class ETimerManager;

// ���������
class EObjectManager
{
	friend EApp;

public:
	// ��һ���ڵ�����ڴ��
	static void add(
		e2d::EObject * nptr
	);

	// ɾ�����нڵ�
	static void clearAllObjects();

	// ֪ͨ�ڴ��ˢ��
	static void notifyFlush();

private:
	// ˢ���ڴ��
	static void __flush();
};


// ��ʱ��
class ETimer :
	public EObject
{
	friend ETimerManager;

public:
	ETimer();

	ETimer(
		const EString &name				/* ��ʱ������ */
	);

	ETimer(
		const TIMER_CALLBACK &callback,	/* ��ʱ���ص����� */
		LONGLONG delay = 20LL			/* ʱ���� */
	);

	ETimer(
		const EString &name,			/* ��ʱ������ */
		const TIMER_CALLBACK &callback,	/* ��ʱ���ص����� */
		LONGLONG delay = 20LL			/* ʱ���� */
	);

	// ��ȡ��ʱ��״̬
	bool isRunning() const;

	// ��ȡ��ʱ������״̬
	bool isWaiting() const;

	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ��ȡ��ʱ������
	EString getName() const;

	// ��ȡ��ʱ�����ڳ���
	EScene * getParentScene() const;

	// ��ȡ��ʱ�����ڽڵ�
	ENode * getParentNode() const;

	// ���ö�ʱ������
	void setName(
		const EString &name
	);

	// ���ö�ʱ��ִ�м��
	void setInterval(
		LONGLONG interval
	);

	// �󶨶�ʱ��������
	virtual void bindWith(
		EScene * pParentScene
	);

	// �󶨶�ʱ�����ڵ�
	virtual void bindWith(
		ENode * pParentNode
	);

protected:
	// ����ȴ�״̬
	void _wait();

	// ����
	void _notify();

	// ִ�лص�����
	virtual void _runCallback();

protected:
	EString			m_sName;
	bool			m_bRunning;
	bool			m_bWaiting;
	int				m_nRunTimes;
	EScene *		m_pParentScene;
	ENode *			m_pParentNode;
	TIMER_CALLBACK	m_Callback;
	LONGLONG		m_nInterval;
	std::chrono::steady_clock::time_point m_tLast;
};


// ��ʱ��������
class ETimerManager
{
	friend EApp;
	friend EScene;
	friend ENode;

public:
	// �󶨶�ʱ��������
	static void bindTimer(
		ETimer * timer,
		EScene * pParentScene
	);

	// �󶨶�ʱ�����ڵ�
	static void bindTimer(
		ETimer * timer,
		ENode * pParentNode
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void startTimers(
		const EString &name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stopTimers(
		const EString &name
	);

	// ɾ��������ͬ���ƵĶ�ʱ��
	static void delTimers(
		const EString &name
	);

	// �������ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		EScene * pParentScene
	);

	// ֹͣ���ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		EScene * pParentScene
	);

	// ��հ��ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void clearAllTimersBindedWith(
		EScene * pParentScene
	);

	// �������ڽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		ENode * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		ENode * pParentNode
	);

	// ��հ��ڽڵ��ϵ����ж�ʱ��
	static void clearAllTimersBindedWith(
		ENode * pParentNode
	);

	// �������ж�ʱ��
	static void startAllTimers();

	// ֹͣ���ж�ʱ��
	static void stopAllTimers();

	// ������ж�ʱ��
	static void clearAllTimers();

private:
	// ������ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void _waitAllTimersBindedWith(
		EScene * pParentScene
	);

	// �������ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void _notifyAllTimersBindedWith(
		EScene * pParentScene
	);

	// ������ڽڵ��ϵ����ж�ʱ��
	static void _waitAllTimersBindedWith(
		ENode * pParentNode
	);

	// �������ڽڵ��ϵ����ж�ʱ��
	static void _notifyAllTimersBindedWith(
		ENode * pParentNode
	);

	// ��ʱ��ִ�г���
	static void TimerProc();
};

}