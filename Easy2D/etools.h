#pragma once
#include "ebase.h"
#include <chrono>
#include <random>

namespace e2d
{

class ETimerManager;
class EAction;


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
		int nRepeatTimes
	);

	// ���ö�ʱ���ڰ󶨺�����ִ��һ��
	virtual void setRunAtOnce(
		bool bAtOnce
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
	ENode *			m_pParentNode;
	TIMER_CALLBACK	m_Callback;
	LONGLONG		m_nInterval;
	std::chrono::steady_clock::time_point m_tLast;
};


class EFileUtils
{
public:
	// ��ȡϵͳ�� AppData Local ·��
	static EString getLocalAppDataPath();

	// ��ȡ��ʱ�ļ�Ŀ¼
	static EString getTempPath();

	// ��ȡĬ�ϵı���·��
	static EString getDefaultSavePath();

	// ���� int �͵�ֵ
	static void saveInt(
		const EString & key,
		int value
	);

	// ���� float �͵�ֵ
	static void saveFloat(
		const EString & key,
		float value
	);

	// ���� �ַ��� �͵�ֵ����Ҫ�� Unicode �ַ����±��������ַ���
	static void saveString(
		const EString & key,
		const EString & value
	);

	// ��ȡ int �͵�ֵ�����������򷵻� default ������ֵ��
	static int getInt(
		const EString & key,
		int default
	);

	// ��ȡ float �͵�ֵ�����������򷵻� default ������ֵ��
	static float getFloat(
		const EString & key,
		float default
	);

	// ��ȡ �ַ��� �͵�ֵ�����������򷵻� default ������ֵ��
	static EString getString(
		const EString & key,
		const EString & default
	);

	// �õ��ļ���չ����Сд��
	static EString getFileExtension(
		const EString & filePath
	);

	/**
	*  �򿪱����ļ��Ի��򣬵õ���Ч����·������ true
	*  �����������ļ�·�����ַ��������ڱ��⣬������չ�����ˣ�����Ĭ����չ��
	*/
	static EString getSaveFilePath(
		const EString & title = L"���浽",
		const EString & defExt = NULL
	);
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


class EMusicUtils
{
public:
	// ��������
	static UINT playMusic(
		const EString & musicFilePath,		/* �����ļ�·�� */
		int repeatTimes = 1
	);

	// ��������
	static UINT playMusic(
		const EString & musicResourceName,	/* ��Դ���� */
		const EString & musicResourceType,	/* ��Դ��� */
		const EString & musicExtension,		/* ָ����Դ����չ�� */
		int repeatTimes = 1
	);

	// ��ͣ����
	static bool pauseMusic(
		UINT musicId
	);

	// ��ͣ����
	static bool pauseMusic(
		const EString& musicName
	);

	// ������������
	static bool resumeMusic(
		UINT musicId
	);

	// ������������
	static bool resumeMusic(
		const EString& musicName
	);

	// ֹͣ����
	static bool stopMusic(
		UINT musicId
	);

	// ֹͣ����
	static bool stopMusic(
		const EString& musicName
	);

	// Ԥ��������
	static UINT preloadMusic(
		const EString & musicFilePath
	);

	// Ԥ��������
	static UINT preloadMusic(
		const EString & musicResourceName,	/* ��Դ���� */
		const EString & musicResourceType,	/* ��Դ��� */
		const EString & musicExtension		/* ָ����Դ����չ�� */
	);

	// ��ͣ��������
	static void pauseAllMusics();

	// ����������������
	static void resumeAllMusics();

	// ֹͣ��������
	static void stopAllMusics();
};

}