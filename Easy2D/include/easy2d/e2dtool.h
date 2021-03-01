#pragma once
#include <easy2d/e2dbase.h>

namespace easy2d
{


// �����������
class Random
{
public:
	// ȡ�÷�Χ�ڵ�һ�����������
	template<typename T>
	static inline T range(T min, T max) 
	{ 
		return easy2d::Random::__randomInt(min, max); 
	}

	// ȡ�÷�Χ�ڵ�һ�������������
	static inline float range(float min, float max) 
	{ 
		return easy2d::Random::__randomReal(min, max); 
	}

	// ȡ�÷�Χ�ڵ�һ�������������
	static inline double range(double min, double max)
	{ 
		return easy2d::Random::__randomReal(min, max); 
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


// ����
class Music :
	public Object
{
	friend class Game;

public:
	Music();

	explicit Music(
		const String& filePath		/* �����ļ�·�� */
	);

	Music(
		int resNameId,				/* ������Դ���� */
		const String& resType		/* ������Դ���� */
	);

	virtual ~Music();

	// �������ļ�
	bool open(
		const String& filePath		/* �����ļ�·�� */
	);

	// ��������Դ
	bool open(
		int resNameId,				/* ������Դ���� */
		const String& resType		/* ������Դ���� */
	);

	// ����
	bool play(
		int nLoopCount = 0
	);

	// ��ͣ
	void pause();

	// ����
	void resume();

	// ֹͣ
	void stop();

	// �رղ�������Դ
	void close();

	// �Ƿ����ڲ���
	bool isPlaying() const;

	// ��������
	bool setVolume(
		float volume
	);

private:
	static bool __init();

	static void __uninit();

private:
	class Media;
	Media* _media;
};


// ���ֲ�����
class MusicPlayer
{
	friend class Game;

public:
	// Ԥ����������Դ
	static Music* preload(
		const String& filePath	/* �����ļ�·�� */
	);

	// ��������
	static bool play(
		const String& filePath,	/* �����ļ�·�� */
		int nLoopCount = 0		/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ����
	static void pause(
		const String& filePath	/* �����ļ�·�� */
	);

	// ������������
	static void resume(
		const String& filePath	/* �����ļ�·�� */
	);

	// ֹͣ����
	static void stop(
		const String& filePath	/* �����ļ�·�� */
	);

	// ��ȡ���ֲ���״̬
	static bool isPlaying(
		const String& filePath	/* �����ļ�·�� */
	);

	// Ԥ����������Դ
	static Music* preload(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ��������
	static bool play(
		int resNameId,			/* ������Դ���� */
		const String& resType,	/* ������Դ���� */
		int nLoopCount = 0		/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ����
	static void pause(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ������������
	static void resume(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ֹͣ����
	static void stop(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ��ȡ���ֲ���״̬
	static bool isPlaying(
		int resNameId,			/* ������Դ���� */
		const String& resType	/* ������Դ���� */
	);

	// ��ȡ����
	static float getVolume();

	// ��������
	static void setVolume(
		float volume			/* ������ΧΪ -224 ~ 224��0 �Ǿ�����1 ���������� */
	);

	// ��ͣ��������
	static void pauseAll();

	// ����������������
	static void resumeAll();

	// ֹͣ��������
	static void stopAll();

private:
	static void __uninit();
};


// ��ʱ��
class Timer
{
	friend class Game;

public:
	// ��Ӷ�ʱ����ÿִ֡��һ�Σ�
	static void add(
		const Function<void()>& func,	/* ִ�к��� */
		const String& name = L""		/* ��ʱ������ */
	);

	// ��Ӷ�ʱ��
	static void add(
		const Function<void()>& func,	/* ִ�к��� */
		float delay,					/* ʱ�������룩 */
		int times = -1,					/* ִ�д������� -1 Ϊ����ִ�У� */
		bool paused = false,			/* �Ƿ���ͣ */
		const String& name = L""		/* ��ʱ������ */
	);

	// ���㹻�ӳٺ�ִ��ָ������
	static void start(
		float timeout,					/* �ȴ���ʱ�����룩 */
		const Function<void()>& func	/* ִ�еĺ��� */
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void start(
		const String& name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stop(
		const String& name
	);

	// �Ƴ�������ͬ���ƵĶ�ʱ��
	static void remove(
		const String& name
	);

	// �������ж�ʱ��
	static void startAll();

	// ֹͣ���ж�ʱ��
	static void stopAll();

	// �Ƴ����ж�ʱ��
	static void removeAll();

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

	// ���� float ���͵�ֵ
	static void saveDouble(
		const String& key,					/* ��ֵ */
		float value,						/* ���� */
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

	// ��ȡ float ���͵�ֵ
	// �����������򷵻� defaultValue ������ֵ��
	static float getDouble(
		const String& key,					/* ��ֵ */
		float defaultValue,				/* Ĭ��ֵ */
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
};


// ·������
class Path
{
	friend class Game;

public:
	// �������·��
	static void add(
		String path
	);

	// �����ļ�·��
	static String searchForFile(
		const String& path
	);

	// ��ȡ��Դ�ļ���������ȡ����ļ�·��
	static String extractResource(
		int resNameId,				/* ��Դ���� */
		const String& resType,		/* ��Դ���� */
		const String& destFileName	/* Ŀ���ļ��� */
	);

	// ��ȡ���ݵ�Ĭ�ϱ���·��
	static String getDataSavePath();

	// ��ȡ��ʱ�ļ�Ŀ¼
	static String getTempPath();

	// ��ȡִ�г���ľ���·��
	static String getExecutableFilePath();

	// �򿪱����ļ��Ի���
	static String getSaveFilePath(
		const String& title = L"���浽",		/* �Ի������ */
		const String& defExt = L""			/* Ĭ����չ�� */
	);

	// �����ļ���
	static bool createFolder(
		const String& dirPath	/* �ļ���·�� */
	);

	// �ж��ļ����ļ����Ƿ����
	static bool exists(
		const String& dirPath	/* �ļ���·�� */
	);

private:
	// ��ʼ��
	static bool __init(
		const String& gameName
	);
};

}