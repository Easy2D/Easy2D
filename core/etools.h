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
	static inline double range(double min, double max) { return e2d::Random::randomReal(min, max); }

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


// ��ʱ��
class Timer :
	public Object
{
	friend TimerManager;

public:
	Timer();

	Timer(
		TimerCallback callback,		/* ��ʱ���ص����� */
		double interval = 0,		/* ʱ�������룩 */
		int times = -1,				/* ִ�д������� -1 Ϊ����ִ�У� */
		bool atOnce = false,		/* �Ƿ�����ִ�� */
		bool autoRelease = false	/* �Զ���� */
	);

	Timer(
		const String &name,					/* ��ʱ������ */
		TimerCallback callback = nullptr,	/* ��ʱ���ص����� */
		double interval = 0,				/* ʱ�������룩 */
		int times = -1,						/* ִ�д������� -1 Ϊ����ִ�У� */
		bool atOnce = false,				/* �Ƿ�����ִ�� */
		bool autoRelease = false			/* �Զ���� */
	);

	// ������ʱ��
	void start();

	// ֹͣ��ʱ��
	void stop();

	// ֹͣ������ö�ʱ��
	void stopAndClear();

	// ���¶�ʱ��
	void update();

	// ��ȡ��ʱ��״̬
	bool isRunning() const;

	// �ж��Ƿ�ﵽִ��״̬
	bool isReady() const;

	// ��ȡ��ʱ������
	String getName() const;

	// ���ö�ʱ������
	void setName(
		const String &name
	);

	// ���ö�ʱ��ִ�м��
	void setInterval(
		double fInterval	/* ʱ�������룩 */
	);

	// ���ö�ʱ���ص�����
	void setCallback(
		TimerCallback callback
	);

	// ���ö�ʱ��ִ�д���
	void setUpdateTimes(
		int nUpdateTimes	/* ִ�д������� -1 Ϊ����ִ�У� */
	);

	// ���ö�ʱ���ڰ󶨺�����ִ��һ��
	virtual void setRunAtOnce(
		bool bAtOnce
	);

protected:
	String			m_sName;
	bool			m_bRunning;
	bool			m_bAtOnce;
	bool			m_bAutoRelease;
	bool			m_bClear;
	int				m_nRunTimes;
	int				m_nUpdateTimes;
	double			m_fInterval;
	double			m_fLast;
	TimerCallback	m_Callback;
};


// ���ݹ�����
class Data
{
public:
	// ���� int ���͵�ֵ
	static void saveInt(
		const String & key,
		int value,
		const String & fileName = L""
	);

	// ���� double ���͵�ֵ
	static void saveDouble(
		const String & key,
		double value,
		const String & fileName = L""
	);

	// ���� bool ���͵�ֵ
	static void saveBool(
		const String & key,
		bool value,
		const String & fileName = L""
	);

	// ���� �ַ��� ���͵�ֵ
	static void saveString(
		const String & key,
		const String & value,
		const String & fileName = L""
	);

	// ��ȡ int ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static int getInt(
		const String & key,
		int defaultValue,
		const String & fileName = L""
	);

	// ��ȡ double ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static double getDouble(
		const String & key,
		double defaultValue,
		const String & fileName = L""
	);

	// ��ȡ bool ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static bool getBool(
		const String & key,
		bool defaultValue,
		const String & fileName = L""
	);

	// ��ȡ �ַ��� ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static String getString(
		const String & key,
		const String & defaultValue,
		const String & fileName = L""
	);

	// �޸������ļ���Ĭ������
	static void setDefaultFileName(
		const String & fileName
	);

	// ��ȡ�����ļ�������·��
	static String getDataFilePath(
		const String & fileName
	);
};


// ·������
class Path
{
public:
	// ��ȡϵͳ�� AppData Local ·��
	static String getLocalAppDataPath();

	// ��ȡ��ʱ�ļ�Ŀ¼
	static String getTempPath();

	// ��ȡ���ݵ�Ĭ�ϱ���·��
	static String getDefaultSavePath();

	// ��ȡ�ļ���չ��
	static String getFileExtension(
		const String & filePath
	);

	// �򿪱����ļ��Ի���
	static String getSaveFilePath(
		const String & title = L"���浽",	/* �Ի������ */
		const String & defExt = L""			/* Ĭ����չ�� */
	);

	// �����ļ���
	static bool createFolder(
		const String & strDirPath	/* �ļ���·�� */
	);
};


// ���ֲ�����
class Music : 
	public Object
{
	friend MusicManager;

public:
	Music();

	Music(
		const String & strFileName	/* �����ļ�·�� */
	);

	virtual ~Music();

	// �������ļ�
	bool open(
		const String & strFileName	/* �����ļ�·�� */
	);

	// ����
	bool play(
		int nLoopCount = 0	/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ
	void pause();

	// ����
	void resume();

	// ֹͣ
	void stop();

	// �ر������ļ�
	void close();

	// ��ȡ���ֲ���״̬
	bool isPlaying();

	// ��ȡ����
	double getVolume() const;

	// ��ȡƵ�ʱ�
	double getFrequencyRatio() const;

	// ��������
	bool setVolume(
		double fVolume	/* ������ΧΪ -224 ~ 224������ 0 �Ǿ�����1 ���������� */
	);

	// ����Ƶ�ʱ�
	bool setFrequencyRatio(
		double fFrequencyRatio	/* Ƶ�ʱȷ�ΧΪ 1/1024.0f ~ 1024.0f������ 1.0 Ϊ�������� */
	);

	// ��ȡ IXAudio2SourceVoice ����
	IXAudio2SourceVoice* getIXAudio2SourceVoice() const;

protected:
	bool _readMMIO();

	bool _resetFile();

	bool _read(
		BYTE* pBuffer, 
		DWORD dwSizeToRead
	);

	bool _findMediaFileCch(
		wchar_t* strDestPath, 
		int cchDest, 
		const wchar_t * strFilename
	);

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