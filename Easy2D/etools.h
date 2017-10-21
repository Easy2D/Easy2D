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
		const TIMER_CALLBACK &callback,	/* ��ʱ���ص����� */
		int repeatTimes = -1,			/* ��ʱ��ִ�д��� */
		LONGLONG interval = 0LL,		/* ʱ���������룩 */
		bool atOnce = false				/* �Ƿ�����ִ�� */
	);

	ETimer(
		const EString &name,			/* ��ʱ������ */
		const TIMER_CALLBACK &callback,	/* ��ʱ���ص����� */
		int repeatTimes = -1,			/* ��ʱ��ִ�д��� */
		LONGLONG interval = 0LL,		/* ʱ���������룩 */
		bool atOnce = false				/* �Ƿ�����ִ�� */
	);

	// ��ȡ��ʱ��״̬
	bool isRunning() const;

	// ������ʱ��
	void start();

	// ֹͣ��ʱ��
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

	// ���ö�ʱ���ص�����
	void setCallback(
		const TIMER_CALLBACK & callback
	);

	// ���ö�ʱ���ظ�ִ�д���
	void setRepeatTimes(
		int repeatTimes
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

	// �ж��Ƿ�ﵽִ��״̬
	bool _isReady();

protected:
	EString			m_sName;
	bool			m_bRunning;
	bool			m_bAtOnce;
	int				m_nRunTimes;
	int				m_nRepeatTimes;
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

	// �������ڽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		ENode * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		ENode * pParentNode
	);

	// �������ж�ʱ��
	static void startAllTimers();

	// ֹͣ���ж�ʱ��
	static void stopAllTimers();

private:
	// ��ն�ʱ��������
	static void _clearManager();

	// ��հ��ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void _clearAllTimersBindedWith(
		EScene * pParentScene
	);

	// ��հ��ڽڵ��ϵ����ж�ʱ��
	static void _clearAllTimersBindedWith(
		ENode * pParentNode
	);

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

	// �������ڽڵ��ϵ����ж���
	static void startAllActionsBindedWith(
		ENode * pTargetNode
	);

	// ��ͣ���ڽڵ��ϵ����ж���
	static void pauseAllActionsBindedWith(
		ENode * pTargetNode
	);

	// ֹͣ���ڽڵ��ϵ����ж���
	static void stopAllActionsBindedWith(
		ENode * pTargetNode
	);

	// �������ж���
	static void startAllActions();

	// ��ͣ���ж���
	static void pauseAllActions();

	// ֹͣ���ж���
	static void stopAllActions();

private:
	// ��ն���������
	static void _clearManager();

	// ��հ��ڽڵ��ϵ����ж���
	static void _clearAllActionsBindedWith(
		ENode * pTargetNode
	);

	// �������ж���״̬
	static void _resetAllActions();

	// ����ִ�г���
	static void ActionProc();
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
	static inline T between(T min, T max) { return e2d::ERandom::randomInt(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline float between(float min, float max) { return e2d::ERandom::randomReal(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline double between(double min, double max) { return e2d::ERandom::randomReal(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline long double between(long double min, long double max) { return e2d::ERandom::randomReal(min, max); }

	// ȡ�����ͷ�Χ�ڵ�һ�������
	template<typename T>
	static T randomInt(
		T min, 
		T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(getEngine());
	}

	// ȡ�ø��������ͷ�Χ�ڵ�һ�������
	template<typename T>
	static T randomReal(
		T min, 
		T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(getEngine());
	}

	// ��ȡ�����������
	static std::default_random_engine &getEngine();
};

}