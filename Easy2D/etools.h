#pragma once
#include "ebase.h"
#include <chrono>
#include <random>

namespace e2d
{

class ETimerManager;
class EAction;

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
	// ִ�лص�����
	virtual void _callOn();

protected:
	EString			m_sName;
	bool			m_bRunning;
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
	// ��ն�ʱ��������
	static void _clearManager();

	// ���ö�ʱ��״̬
	static void _resetAllTimers();

	// ��ʱ��ִ�г���
	static void TimerProc();
};


// ����������
class EActionManager
{
	friend EApp;
	friend EScene;
	friend ENode;

public:
	// ��Ӷ���
	static void addAction(
		EAction * action
	);

	// �������ڳ����ӽڵ��ϵ����ж���
	static void startAllActionsBindedWith(
		EScene * pParentScene
	);

	// ֹͣ���ڳ����ӽڵ��ϵ����ж���
	static void stopAllActionsBindedWith(
		EScene * pParentScene
	);

	// ��հ��ڳ����ӽڵ��ϵ����ж���
	static void clearAllActionsBindedWith(
		EScene * pParentScene
	);

	// �������ڽڵ��ϵ����ж���
	static void startAllActionsBindedWith(
		ENode * pTargetNode
	);

	// ֹͣ���ڽڵ��ϵ����ж���
	static void stopAllActionsBindedWith(
		ENode * pTargetNode
	);

	// ��հ��ڽڵ��ϵ����ж���
	static void clearAllActionsBindedWith(
		ENode * pTargetNode
	);

	// �������ж���
	static void startAllActions();

	// ֹͣ���ж���
	static void stopAllActions();

	// ������ж���
	static void clearAllActions();

private:
	// ��ն���������
	static void _clearManager();

	// �������ж���״̬
	static void _resetAllActions();

	// ����ִ�г���
	static void ActionProc();
};


class EMusicUtils
{
public:
	// ������Ч
	static void play(LPCTSTR musicFileName, bool loop = false);
};


class EFileUtils
{
public:
	// ��ȡϵͳ�� AppData\Local ·��
	static EString getLocalAppDataPath();

	// ��ȡĬ�ϵı���·��
	static EString getDefaultSavePath();

	// ���� int �͵�ֵ
	static void saveInt(LPCTSTR key, int value);

	// ���� double �͵�ֵ
	static void saveDouble(LPCTSTR key, double value);

	// ���� �ַ��� �͵�ֵ����Ҫ�� Unicode �ַ����±��������ַ���
	static void saveString(LPCTSTR key, EString value);

	// ��ȡ int �͵�ֵ�����������򷵻� default ������ֵ��
	static int getInt(LPCTSTR key, int default);

	// ��ȡ double �͵�ֵ�����������򷵻� default ������ֵ��
	static double getDouble(LPCTSTR key, double default);

	// ��ȡ �ַ��� �͵�ֵ�����������򷵻� default ������ֵ��
	static EString geTString(LPCTSTR key, EString default);

	// �õ��ļ���չ����Сд��
	static EString getFileExtension(const EString & filePath);

	/**
	*  �򿪱����ļ��Ի��򣬵õ���Ч����·������ true
	*  �����������ļ�·�����ַ��������ڱ��⣬������չ�����ˣ�����Ĭ����չ��
	*/
	static EString getSaveFilePath(LPCTSTR title = L"���浽", LPCTSTR defExt = NULL);
};


class ERandom
{
public:
	// ȡ�����ͷ�Χ�ڵ�һ�������
	template<typename T>
	static T randomInt(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(getEngine());
	}

	// ȡ�ø��������ͷ�Χ�ڵ�һ�������
	template<typename T>
	static T randomReal(T min, T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(getEngine());
	}

	// ��ȡ�����������
	static std::default_random_engine &getEngine();
};

template<typename T>
inline T random(T min, T max) { return e2d::ERandom::randomInt(min, max); }
inline float random(float min, float max) { return e2d::ERandom::randomReal(min, max); }
inline double random(double min, double max) { return e2d::ERandom::randomReal(min, max); }
inline long double random(long double min, long double max) { return e2d::ERandom::randomReal(min, max); }

}