#pragma once
#include <e2dbase.h>

namespace easy2d 
{
	class Action;

	class FileUtils
	{
	public:
		// ��ȡϵͳ�� AppData\Local ·��
		static tstring getLocalAppDataPath();
		// ��ȡĬ�ϵı���·��
		static tstring getDefaultSavePath();
		// ���� int �͵�ֵ
		static void saveInt(LPCTSTR key, int value);
		// ���� double �͵�ֵ
		static void saveDouble(LPCTSTR key, double value);
		// ���� �ַ��� �͵�ֵ����Ҫ�� Unicode �ַ����±��������ַ���
		static void saveString(LPCTSTR key, tstring value);
		// ��ȡ int �͵�ֵ�����������򷵻� default ������ֵ��
		static int getInt(LPCTSTR key, int default);
		// ��ȡ double �͵�ֵ�����������򷵻� default ������ֵ��
		static double getDouble(LPCTSTR key, double default);
		// ��ȡ �ַ��� �͵�ֵ�����������򷵻� default ������ֵ��
		static tstring getString(LPCTSTR key, tstring default);
		// �õ��ļ���չ����Сд��
		static tstring getFileExtension(const tstring& filePath);
		/**
		*  �򿪱����ļ��Ի��򣬵õ���Ч����·������ true
		*  �����������ļ�·�����ַ��������ڱ��⣬������չ�����ˣ�����Ĭ����չ��
		*/
		static bool getSaveFilePath(tstring& path, LPCTSTR title = _T("���浽"), LPCTSTR defExt = NULL);
	};


	class MusicUtils
	{
	public:
		// ���ű�������
		static void playBackgroundMusic(tstring pszFilePath, bool bLoop = true);
		// ֹͣ��������
		static void stopBackgroundMusic(bool bReleaseData = false);
		// ��ͣ��������
		static void pauseBackgroundMusic();
		// �������ű�������
		static void resumeBackgroundMusic();
		// ��ͷ���ű�������
		static void rewindBackgroundMusic();
		// ���������Ƿ����ڲ���
		static bool isBackgroundMusicPlaying();
		// ���ñ�������������0 ~ 1.0f
		static void setBackgroundMusicVolume(float volume);

		// ������Ч
		static unsigned int playMusic(tstring pszFilePath, bool loop = false);
		// ֹͣ��Ч
		static void stopMusic(unsigned int nSoundId);
		// Ԥ������Ч
		static void preloadMusic(tstring pszFilePath);
		// ��ͣ��Ч
		static void pauseMusic(unsigned int nSoundId);
		// ����������Ч
		static void resumeMusic(unsigned int nSoundId);
		// ж����Ч
		static void unloadMusic(LPCTSTR pszFilePath);
		// �����ض����ֵ�������0 ~ 1.0f
		static void setVolume(tstring pszFilePath, float volume);

		// ��ͣ��������
		static void pauseAllMusics();
		// ����������������
		static void resumeAllMusics();
		// ֹͣ��������
		static void stopAllMusics();
		// ֹͣ�������֣����ͷ��ڴ�
		static void end();
		// ������������0 ~ 1.0f
		static void setVolume(float volume);
	};


	class Timer
	{
		friend class App;

	public:
		Timer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
		~Timer();

		// ������ʱ��
		void start();
		// ֹͣ��ʱ��
		void stop();
		// ��ʱ���Ƿ���������
		bool isRunning();
		// ���ü��ʱ��
		void setInterval(UINT ms);
		// ���ûص�����
		void setCallback(const TIMER_CALLBACK& callback);
		// ���ö�ʱ������
		void setName(tstring name);
		// ��ȡ��ʱ�����ʱ��
		UINT getInterval() const;
		// ��ȡ��ʱ������
		tstring getName() const;

		// ��Ӷ�ʱ��
		static void addTimer(Timer * timer);
		// ��Ӷ�ʱ��
		static void addTimer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
		// �������ƻ�ȡ��ʱ��
		static Timer * getTimer(tstring name);
		// �����ض���ʱ��
		static bool startTimer(tstring name);
		// ֹͣ�ض���ʱ��
		static bool stopTimer(tstring name);
		// ɾ���ض���ʱ��
		static bool delTimer(tstring name);
		// ɾ�����ж�ʱ��
		static void clearAllTimers();

	protected:
		bool			m_bRunning;
		tstring			m_sName;
		TIMER_CALLBACK	m_callback;
		LARGE_INTEGER	m_nLast;
		LARGE_INTEGER	m_nAnimationInterval;
		UINT			m_nMilliSeconds;

	private:
		static void __exec();
	};


	class ActionManager
	{
		friend class App;
		friend class Sprite;
	public:
		// ����һ���ض��Ķ���
		static void startAction(Action * action);
		// ����һ���ض��Ķ���
		static void resumeAction(Action * action);
		// ��ͣһ���ض��Ķ���
		static void pauseAction(Action * action);
		// ֹͣһ���ض��Ķ���
		static void stopAction(Action * action);

		// ����һ�� Sprite �����ж���
		static void startSpriteAllActions(Sprite * sprite);
		// ����һ�� Sprite �����ж���
		static void resumeSpriteAllActions(Sprite * sprite);
		// ��ͣһ�� Sprite �����ж���
		static void pauseSpriteAllActions(Sprite * sprite);
		// ֹͣһ�� Sprite �����ж���
		static void stopSpriteAllActions(Sprite * sprite);

		// ������ǰ���ڵ����ж���
		static void startAllActions();
		// ������ǰ���ڵ����ж���
		static void resumeAllActions();
		// ��ͣ��ǰ���ڵ����ж���
		static void pauseAllActions();
		// ֹͣ��ǰ���ڵ����ж���
		static void stopAllActions();

	private:
		static void __exec();
		// ��һ��������ӽ�����������
		static void addAction(Action * action);
	};

}	// End of easy2d namespace