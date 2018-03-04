#include "..\ebase.h"
#include "..\emanagers.h"


// ������Ϸ��ֹ
static bool s_bEndGame = false;
// ������Ϸ��ͣ
static bool s_bPaused = false;
// �Ƿ���й���ʼ��
static bool s_bInitialized = false;
// AppName
static e2d::String s_sAppName;


bool e2d::Game::init(const String & sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID, const String & sAppname)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	do
	{
		// ��ʼ�� COM ���
		CoInitializeEx(NULL, COINIT_MULTITHREADED);

		// ���� AppName
		s_sAppName = sAppname;

		// �����豸�޹���Դ
		if (!Renderer::__createDeviceIndependentResources())
		{
			WARN_IF(true, "Renderer::__createDeviceIndependentResources Failed!");
			break;
		}

		// ��ʼ������
		if (!Window::__init(sTitle, nWidth, nHeight, pIconID))
		{
			WARN_IF(true, "Window::__init Failed!");
			break;
		}

		// �����豸�����Դ
		if (!Renderer::__createDeviceResources())
		{
			WARN_IF(true, "Renderer::__createDeviceResources Failed!");
			break;
		}

		// ��ʼ�� DirectInput
		if (!Input::__init())
		{
			WARN_IF(true, "Input::__init Failed!");
			break;
		}

		// ��ʼ��������
		if (!MusicManager::__init())
		{
			WARN_IF(true, "MusicManager::__init Failed!");
			break;
		}

		// ���� Client ��С
		Window::setSize(nWidth, nHeight);
		// ��־��ʼ���ɹ�
		s_bInitialized = true;

	} while (0);

	return s_bInitialized;
}

int e2d::Game::run()
{
	if (!s_bInitialized)
	{
		ASSERT(false, "You must initialize Game first!");
		return -1;
	}

	// ��ʼ������������
	SceneManager::__init();
	// ��ʾ����
	::ShowWindow(Window::getHWnd(), SW_SHOWNORMAL);
	// ˢ�´�������
	::UpdateWindow(Window::getHWnd());
	// ��������Ϣ
	Window::__poll();
	// ��ʼ����ʱ
	Time::__init();

	while (!s_bEndGame)
	{
		// ��������Ϣ
		Window::__poll();
		// ˢ��ʱ��
		Time::__updateNow();

		// �ж��Ƿ�ﵽ��ˢ��״̬
		if (Time::__isReady())
		{
			TimerManager::__update();	// ��ʱ��������ִ�г���
			ActionManager::__update();	// ����������ִ�г���
			while (Time::__isReady())
			{
				Input::__updateDeviceState();	// ��ȡ�û�����
				SceneManager::__update();		// ���³�������
				Time::__updateLast();			// ˢ��ʱ����Ϣ
			}
			Renderer::__render();		// ��Ⱦ��Ϸ����
		}
		else
		{
			ObjectManager::__update();	// ˢ���ڴ��
			Time::__sleep();			// �����߳�
		}
	}

	return 0;
}

void e2d::Game::pause()
{
	s_bPaused = true;
}

void e2d::Game::resume()
{
	if (isPaused())
	{
		s_bPaused = false;
		// ˢ�µ�ǰʱ��
		Time::__updateLast();
		// ���ö����Ͷ�ʱ��
		ActionManager::__resetAllActions();
		TimerManager::__resetAllTimers();
	}
}

bool e2d::Game::isPaused()
{
	return s_bPaused;
}

void e2d::Game::quit()
{
	s_bEndGame = true;	// ���������������Ϸ�Ƿ����
}

void e2d::Game::uninit()
{
	// ɾ�����г���
	SceneManager::__uninit();
	// �ر�����
	Input::__uninit();
	// �رղ�����
	MusicManager::__uninit();
	// ��ն�ʱ��
	TimerManager::__uninit();
	// �ָ���ʱ����
	Time::__uninit();
	// ���ͼƬ����
	Image::clearCache();
	// ˢ���ڴ��
	ObjectManager::__clearAllObjects();
	// ɾ����Ⱦ�����Դ
	Renderer::__discardResources();
	// ���ٴ���
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

e2d::String e2d::Game::getAppName()
{
	return s_sAppName;
}
