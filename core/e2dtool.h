#pragma once
#include "e2dbase.h"

namespace e2d
{

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
class Music
{
	friend Game;

public:
	// Ԥ����������Դ
	static bool preload(
		const String& filePath	/* �����ļ�·�� */
	);

	// Ԥ����������Դ
	static bool preload(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ��������
	static bool play(
		const String& filePath,	/* �����ļ�·�� */
		int nLoopCount = 0		/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��������
	static bool play(
		int resNameId,			/* ������Դ���� */
		const String& resType,	/* ������Դ���� */
		int nLoopCount = 0		/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ����
	static void pause(
		const String& filePath	/* �����ļ�·�� */
	);

	// ��ͣ����
	static void pause(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ������������
	static void resume(
		const String& filePath	/* �����ļ�·�� */
	);

	// ������������
	static void resume(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ֹͣ����
	static void stop(
		const String& filePath	/* �����ļ�·�� */
	);

	// ֹͣ����
	static void stop(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ��ȡ���ֲ���״̬
	static bool isPlaying(
		const String& filePath	/* �����ļ�·�� */
	);

	// ��ȡ���ֲ���״̬
	static bool isPlaying(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ��ȡ����
	static double getVolume();

	// ��������
	static void setVolume(
		double fVolume			/* ������ΧΪ -224 ~ 224��0 �Ǿ�����1 ���������� */
	);

	// ��ͣ��������
	static void pauseAll();

	// ����������������
	static void resumeAll();

	// ֹͣ��������
	static void stopAll();

	// ��ȡ IXAudio2 ����
	static IXAudio2 * getIXAudio2();

	// ��ȡ IXAudio2MasteringVoice ����
	static IXAudio2MasteringVoice * getIXAudio2MasteringVoice();

private:
	static bool __init();

	static void __uninit();
};


// ��ʱ��
class Timer
{
	friend Game;

public:
	// ������ʱ����ÿִ֡��һ�Σ�
	static void start(
		const Function& func,		/* ִ�к��� */
		const String& name = L""	/* ��ʱ������ */
	);

	// ������ʱ��
	static void start(
		const Function& func,		/* ִ�к��� */
		double delay,				/* ʱ�������룩 */
		int times = -1,				/* ִ�д������� -1 Ϊ����ִ�У� */
		bool paused = false,		/* �Ƿ���ͣ */
		const String& name = L""	/* ��ʱ������ */
	);

	// ������ִ��һ�εĶ�ʱ��
	static void startOnce(
		const Function& func,		/* ִ�еĺ��� */
		double timeOut				/* �ȴ���ʱ�����룩 */
	);

	// ��ͣ������ͬ���ƵĶ�ʱ��
	static void pause(
		const String& name
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void resume(
		const String& name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stop(
		const String& name
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
		const String& key,					/* ��ֵ */
		int value,							/* ���� */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ���� double ���͵�ֵ
	static void saveDouble(
		const String& key,					/* ��ֵ */
		double value,						/* ���� */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ���� bool ���͵�ֵ
	static void saveBool(
		const String& key,					/* ��ֵ */
		bool value,							/* ���� */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ���� �ַ��� ���͵�ֵ
	static void saveString(
		const String& key,					/* ��ֵ */
		const String& value,				/* ���� */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ int ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static int getInt(
		const String& key,					/* ��ֵ */
		int defaultValue,					/* Ĭ��ֵ */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ double ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static double getDouble(
		const String& key,					/* ��ֵ */
		double defaultValue,				/* Ĭ��ֵ */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ bool ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static bool getBool(
		const String& key,					/* ��ֵ */
		bool defaultValue,					/* Ĭ��ֵ */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// ��ȡ �ַ��� ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static String getString(
		const String& key,					/* ��ֵ */
		const String& defaultValue,			/* Ĭ��ֵ */
		const String& field = L"Defalut"	/* �ֶ����� */
	);

	// �޸������ļ�������
	static void setDataFileName(
		const String& filePath				/* �ļ����� */
	);

	// ��ȡ�����ļ�������·��
	static String getDataFilePath();
};


// ·������
class Path
{
	friend Game;

public:
	// ��ȡ���ݵ�Ĭ�ϱ���·��
	static String getDefaultSavePath();

	// ��ȡ��ʱ�ļ�Ŀ¼
	static String getTempPath();

	// ��ȡ�ļ���չ��
	static String getFileExtension(
		const String& filePath
	);

	// �򿪱����ļ��Ի���
	static String getSaveFilePath(
		const String& title = L"���浽",		/* �Ի������ */
		const String& defExt = L""			/* Ĭ����չ�� */
	);

	// �����ļ���
	static bool createFolder(
		const String& dirPath	/* �ļ���·�� */
	);

private:
	// ��ʼ��
	static bool __init();
};

}