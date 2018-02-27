#pragma once
#include "ebase.h"
#include <random>

namespace e2d
{

class TimerManager;
class MusicManager;

// �����������
class Random
{
public:
	// ȡ�����ͷ�Χ�ڵ�һ�������
	template<typename T>
	static inline T range(T min, T max) { return e2d::Random::randomInt(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline float range(float min, float max) { return e2d::Random::randomReal(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline double range(double min, double max) { return e2d::Random::randomReal(min, max); }

	// ȡ�ø�������Χ�ڵ�һ�������
	static inline long double range(long double min, long double max) { return e2d::Random::randomReal(min, max); }

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
class Timer :
	public Obj
{
	friend TimerManager;

public:
	Timer();

	Timer(
		const TimerCallback &callback,	/* ��ʱ���ص����� */
		float interval = 0,				/* ʱ�������룩 */
		int repeatTimes = -1,			/* ��ʱ��ִ�д��� */
		bool atOnce = false				/* �Ƿ�����ִ�� */
	);

	Timer(
		const String &name,			/* ��ʱ������ */
		const TimerCallback &callback,	/* ��ʱ���ص����� */
		float interval = 0,				/* ʱ�������룩 */
		int repeatTimes = -1,			/* ��ʱ��ִ�д��� */
		bool atOnce = false				/* �Ƿ�����ִ�� */
	);

	// ������ʱ��
	void start();

	// ֹͣ��ʱ��
	void stop();

	// ��ȡ��ʱ��״̬
	bool isRunning() const;

	// ��ȡ��ʱ������
	String getName() const;

	// ��ȡ��ʱ�����ڽڵ�
	Node * getParentNode() const;

	// ���ö�ʱ������
	void setName(
		const String &name
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

protected:
	// ִ�лص�����
	void _callOn();

	// �ж��Ƿ�ﵽִ��״̬
	bool _isReady() const;

protected:
	String			m_sName;
	bool			m_bRunning;
	bool			m_bAtOnce;
	int				m_nRunTimes;
	int				m_nRepeatTimes;
	float			m_fInterval;
	float			m_fLast;
	Node *			m_pParentNode;
	TimerCallback	m_Callback;
};


// ���ݹ�����
class Data
{
public:
	// ���� int ���͵�ֵ
	static void saveInt(
		const String & key,
		int value
	);

	// ���� float ���͵�ֵ
	static void saveFloat(
		const String & key,
		float value
	);

	// ���� �ַ��� ���͵�ֵ
	static void saveString(
		const String & key,
		const String & value
	);

	// ��ȡ int ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static int getInt(
		const String & key,
		int defaultValue
	);

	// ��ȡ float ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static float getFloat(
		const String & key,
		float defaultValue
	);

	// ��ȡ �ַ��� ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static String getString(
		const String & key,
		const String & defaultValue
	);
};


// �ļ�������
class File
{
public:
	// ��ȡϵͳ�� AppData Local ·��
	static String getLocalAppDataPath();

	// ��ȡ��ʱ�ļ�Ŀ¼
	static String getTempPath();

	// ��ȡĬ�ϵı���·��
	static String getDefaultSavePath();

	// ��ȡ�ļ���չ��
	static String getFileExtension(
		const String & filePath
	);

	// �򿪱����ļ��Ի���
	static String getSaveFilePath(
		const String & title = L"���浽",	/* �Ի������ */
		const String & defExt = L""		/* Ĭ����չ�� */
	);
};


// ���ֲ�����
class Music
{
	friend MusicManager;

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
	Music();

	virtual ~Music();

	bool _open(LPCWSTR strFileName);

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