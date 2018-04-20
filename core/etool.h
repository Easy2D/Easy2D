#pragma once
#include "ebase.h"
#include <random>

namespace e2d
{

class TimerManager;
class MusicManager;
class InputManager;
class ColliderManager;

// �����������
class Random
{
public:
	// ȡ�÷�Χ�ڵ�һ�����������
	template<typename T>
	static inline T range(T min, T max) 
	{ 
		return e2d::Random::__randomInt(min, max); 
	}

	// ȡ�÷�Χ�ڵ�һ�������������
	static inline double range(float min, float max) 
	{ 
		return e2d::Random::__randomReal(min, max); 
	}

	// ȡ�÷�Χ�ڵ�һ�������������
	static inline double range(double min, double max) 
	{ 
		return e2d::Random::__randomReal(min, max); 
	}

private:
	template<typename T>
	static T __randomInt(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(Random::__getEngine());
	}

	template<typename T>
	static T __randomReal(T min, T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(Random::__getEngine());
	}

	// ��ȡ�����������
	static std::default_random_engine &__getEngine();
};


// ���ֲ�����
class Music :
	public Object
{
	friend MusicManager;

public:
	Music();

	Music(
		String strFileName	/* �����ļ�·�� */
	);

	virtual ~Music();

	// �������ļ�
	bool open(
		String strFileName	/* �����ļ�·�� */
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
	bool isPlaying() const;

	// ��ȡ����
	double getVolume() const;

	// ��ȡƵ�ʱ�
	double getFrequencyRatio() const;

	// ��������
	bool setVolume(
		double fVolume			/* ������ΧΪ -224 ~ 224������ 0 �Ǿ�����1 ���������� */
	);

	// ����Ƶ�ʱ�
	bool setFrequencyRatio(
		double fFrequencyRatio	/* Ƶ�ʱȷ�ΧΪ 1/1024.0f ~ 1024.0f������ 1.0 Ϊ�������� */
	);

#if HIGHER_THAN_VS2010

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
	mutable bool m_bPlaying;
	DWORD m_dwSize;
	CHAR* m_pResourceBuffer;
	BYTE* m_pbWaveData;
	HMMIO m_hmmio;
	MMCKINFO m_ck;
	MMCKINFO m_ckRiff;
	WAVEFORMATEX* m_pwfx;
	IXAudio2SourceVoice* m_pSourceVoice;

#else

protected:
	void _sendCommand(int nCommand, DWORD_PTR param1 = 0, DWORD_PTR parma2 = 0);

	static LRESULT WINAPI MusicProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

protected:
	MCIDEVICEID m_dev;
	HWND        m_wnd;
	UINT		m_nMusicID;
	bool        m_bPlaying;
	int			m_nRepeatTimes;

#endif
};


// ��ʱ��
class Timer :
	public Object
{
	friend TimerManager;

public:
	Timer(
		String name = L"",			/* �������� */
		Function func = nullptr,	/* ִ�к��� */
		double interval = 0,		/* ʱ�������룩 */
		int times = -1,				/* ִ�д������� -1 Ϊ����ִ�У� */
		bool atOnce = false,		/* �Ƿ�����ִ�� */
		bool autoRelease = false	/* ִ�н���ʱ�Զ���� */
	);

	// ������ʱ��
	void start();

	// ������ʱ������ִ��ָ������
	void start(
		int times		/* ִ�д������� -1 Ϊ����ִ�У� */
	);

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
		String name
	);

	// ���ö�ʱ��ִ�м��
	void setInterval(
		double fInterval	/* ʱ�������룩 */
	);

	// ���ö�ʱ����ִ�к���
	void setFunc(
		Function func
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
	String		m_sName;
	bool		m_bRunning;
	bool		m_bAtOnce;
	bool		m_bAutoRelease;
	bool		m_bClear;
	int			m_nRunTimes;
	int			m_nUpdateTimes;
	double		m_fInterval;
	double		m_fLast;
	Function	m_Callback;
};


// ������
class Listener
	: public Object
{
public:
	Listener();

	Listener(
		String name		/* ���������� */
	);

	// ����
	virtual void start();

	// ֹͣ
	virtual void stop();

	// ֹͣ�����
	virtual void stopAndClear();

	// ��ȡ����״̬
	virtual bool isRunning();

	// ��ȡ����
	virtual String getName();

	// �޸�����
	virtual void setName(
		String name
	);

protected:
	// ���¼�����״̬
	virtual void _update() = 0;

protected:
	String m_sName;
	bool m_bRunning;
	bool m_bClear;
};


// ���������
class InputListener
	: public Listener
{
	friend InputManager;

public:
	InputListener();

	InputListener(
		Function func	/* �������û�����ʱ��ִ�к��� */
	);

	InputListener(
		Function func,	/* �������û�����ʱ��ִ�к��� */
		String name		/* ���������� */
	);

	// ���ü������û�����ʱ��ִ�к���
	virtual void setFunc(
		Function func
	);

protected:
	// ���¼�����״̬
	virtual void _update();

protected:
	Function m_callback;
};


// ��ײ������
class CollisionListener
	: public Listener
{
	friend ColliderManager;

public:
	CollisionListener();

	CollisionListener(
		Function func	/* �������û�����ʱ��ִ�к��� */
	);

	CollisionListener(
		Function func,	/* �������û�����ʱ��ִ�к��� */
		String name		/* ���������� */
	);

protected:
	// ���¼�����״̬
	virtual void _update();

protected:
	Function m_callback;
};


// ���ݹ�������
class Data
{
public:
	// ���� int ���͵�ֵ
	static void saveInt(
		String key,					/* ��ֵ */
		int value,					/* ���� */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ���� double ���͵�ֵ
	static void saveDouble(
		String key,					/* ��ֵ */
		double value,				/* ���� */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ���� bool ���͵�ֵ
	static void saveBool(
		String key,					/* ��ֵ */
		bool value,					/* ���� */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ���� �ַ��� ���͵�ֵ
	static void saveString(
		String key,					/* ��ֵ */
		String value,				/* ���� */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ int ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static int getInt(
		String key,					/* ��ֵ */
		int defaultValue,			/* Ĭ��ֵ */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ double ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static double getDouble(
		String key,					/* ��ֵ */
		double defaultValue,		/* Ĭ��ֵ */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ bool ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static bool getBool(
		String key,					/* ��ֵ */
		bool defaultValue,			/* Ĭ��ֵ */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ �ַ��� ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static String getString(
		String key,					/* ��ֵ */
		String defaultValue,		/* Ĭ��ֵ */
		String field = L"Defalut"	/* �ֶ����� */
	);

	// �޸������ļ�������
	static void setDataFileName(
		String strFileName			/* �ļ����� */
	);

	// ��ȡ�����ļ�������·��
	static String getDataFilePath();
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
		String filePath
	);

	// �򿪱����ļ��Ի���
	static String getSaveFilePath(
		const String title = L"���浽",		/* �Ի������ */
		const String defExt = L""			/* Ĭ����չ�� */
	);

	// �����ļ���
	static bool createFolder(
		String strDirPath	/* �ļ���·�� */
	);
};

}