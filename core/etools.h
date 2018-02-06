#pragma once
#include "ebase.h"
#include <random>

namespace e2d
{

class ETimerManager;
class EMusicManager;

// �����������
class ERandom
{
public:
	// ȡ�����ͷ�Χ�ڵ�һ�������
	template<typename T>
	static inline T range(T min, T max) { return e2d::ERandom::randomInt(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline float range(float min, float max) { return e2d::ERandom::randomReal(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline double range(double min, double max) { return e2d::ERandom::randomReal(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline long double range(long double min, long double max) { return e2d::ERandom::randomReal(min, max); }

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


// ��ʱ��
class ETimer :
	public EObject
{
	friend ETimerManager;

public:
	ETimer();

	ETimer(
		const TimerCallback &callback,	/* ��ʱ���ص����� */
		int repeatTimes = -1,			/* ��ʱ��ִ�д��� */
		float interval = 0LL,			/* ʱ�������룩 */
		bool atOnce = false				/* �Ƿ�����ִ�� */
	);

	ETimer(
		const EString &name,			/* ��ʱ������ */
		const TimerCallback &callback,	/* ��ʱ���ص����� */
		int repeatTimes = -1,			/* ��ʱ��ִ�д��� */
		float interval = 0,				/* ʱ�������룩 */
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

	// ���ö�ʱ��ִ�м�����룩
	void setInterval(
		float interval
	);

	// ���ö�ʱ���ص�����
	void setCallback(
		const TimerCallback & callback
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
	float			m_fInterval;
	float			m_fLast;
	ENode *			m_pParentNode;
	TimerCallback	m_Callback;
};


// ���ݹ�����
class EData
{
public:
	// ���� int ���͵�ֵ
	static void saveInt(
		const EString & key,
		int value
	);

	// ���� float ���͵�ֵ
	static void saveFloat(
		const EString & key,
		float value
	);

	// ���� �ַ��� ���͵�ֵ
	static void saveString(
		const EString & key,
		const EString & value
	);

	// ��ȡ int ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static int getInt(
		const EString & key,
		int defaultValue
	);

	// ��ȡ float ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static float getFloat(
		const EString & key,
		float defaultValue
	);

	// ��ȡ �ַ��� ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static EString getString(
		const EString & key,
		const EString & defaultValue
	);
};


// �ļ�������
class EFile
{
public:
	// ��ȡϵͳ�� AppData Local ·��
	static EString getLocalAppDataPath();

	// ��ȡ��ʱ�ļ�Ŀ¼
	static EString getTempPath();

	// ��ȡĬ�ϵı���·��
	static EString getDefaultSavePath();

	// ��ȡ�ļ���չ��
	static EString getFileExtension(
		const EString & filePath
	);

	// �򿪱����ļ��Ի���
	static EString getSaveFilePath(
		const EString & title = L"���浽",	/* �Ի������ */
		const EString & defExt = L""		/* Ĭ����չ�� */
	);
};


// ���ֲ�����
class EMusic
{
	friend EMusicManager;

public:
	// ����
	bool play(
		int nLoopCount = 0	/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ
	bool pause();

	// ����
	bool resume();

	// ֹͣ
	bool stop();

	// ��ȡ���ֲ���״̬
	bool isPlaying();

	// ��ȡ����
	float getVolume() const;

	// ��������
	bool setVolume(
		float fVolume	/* ������ΧΪ -224 ~ 224������ 0 �Ǿ�����1 ���������� */
	);

	// ��ȡƵ�ʱ�
	float getFrequencyRatio() const;

	// ����Ƶ�ʱ�
	bool setFrequencyRatio(
		float fFrequencyRatio	/* Ƶ�ʱȷ�ΧΪ 1/1024.0f ~ 1024.0f������ 1.0 Ϊ�������� */
	);

	// ��ȡ IXAudio2SourceVoice ����
	IXAudio2SourceVoice* getIXAudio2SourceVoice() const;

protected:
	EMusic();

	virtual ~EMusic();

	EMusic(const EMusic &) = delete;
	
	EMusic &operator =(const EMusic &) = delete;

	bool _open(LPWSTR strFileName);

	void _close();

	bool _readMMIO();

	bool _resetFile();

	bool _read(BYTE* pBuffer, DWORD dwSizeToRead);

	bool _findMediaFileCch(WCHAR* strDestPath, int cchDest, LPCWSTR strFilename);

protected:
	bool m_bOpened;
	bool m_bPlaying;
	DWORD m_dwSize;
	CHAR* m_pResourceBuffer;
	BYTE* m_pbWaveData;
	HMMIO m_hmmio;
	MMCKINFO m_ck;
	MMCKINFO m_ckRiff;
	WAVEFORMATEX* m_pwfx;
	IXAudio2SourceVoice* m_pSourceVoice;
};

}