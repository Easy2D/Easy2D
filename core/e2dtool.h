#pragma once
#include "e2dbase.h"
#include <random>

namespace e2d
{

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
	friend Game;

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

private:
	static bool __init();

	static void __uninit();

#if HIGHER_THAN_VS2010

public:
	// ��ȡ IXAudio2 ����
	static IXAudio2 * getIXAudio2();

	// ��ȡ IXAudio2MasteringVoice ����
	static IXAudio2MasteringVoice * getIXAudio2MasteringVoice();

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
class Timer
{
	friend Game;

public:
	// ������ʱ��
	static void start(
		Function func,			/* ִ�к��� */
		String name				/* ��ʱ������ */
	);

	// ������ʱ��
	static void start(
		Function func,			/* ִ�к��� */
		double delay = 0,		/* ʱ�������룩 */
		int times = -1,			/* ִ�д������� -1 Ϊ����ִ�У� */
		bool paused = false,	/* �Ƿ���ͣ */
		String name = L""		/* ��ʱ������ */
	);

	// ������ִ��һ�εĶ�ʱ��
	static void startOnce(
		Function func,		/* ִ�еĺ��� */
		double timeOut		/* �ȴ���ʱ�����룩 */
	);

	// ��ͣ������ͬ���ƵĶ�ʱ��
	static void pause(
		String name
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void resume(
		String name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stop(
		String name
	);

	// ��ͣ���ж�ʱ��
	static void pauseAll();

	// �������ж�ʱ��
	static void resumeAll();

	// ֹͣ���ж�ʱ��
	static void stopAll();

private:
	// ���¶�ʱ��
	static void __update();

	// ���ö�ʱ��״̬
	static void __resetAll();

	// ��ն�ʱ��
	static void __uninit();
};


// ���ݹ�����
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