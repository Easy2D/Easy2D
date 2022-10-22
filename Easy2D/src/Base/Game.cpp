#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>
#include <easy2d/e2dtool.h>


// ������Ϸ��ֹ
static bool s_bEndGame = true;
// ������Ϸ��ͣ
static bool s_bPaused = false;
// �Ƿ���й���ʼ��
static bool s_bInitialized = false;
// ��Ϸ����
static easy2d::String s_sGameName;


bool easy2d::Game::init(const String& title, int width, int height, const String& uniqueName)
{
	if (s_bInitialized)
	{
		E2D_WARNING("The game has been initialized!");
		return false;
	}

	if (!uniqueName.empty())
	{
		// �������̻�����
		String fullMutexName = "Easy2DApp-" + uniqueName;
		HANDLE hMutex = ::CreateMutexA(nullptr, TRUE, fullMutexName.c_str());

		if (hMutex == nullptr)
		{
			E2D_WARNING("CreateMutex Failed!");
		}
		else if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// ��������Ѿ����ڲ����������У�������ʾ
			Window::error("��Ϸ�������������д򿪣�", "��ʾ");
			// �رս��̻�����
			::CloseHandle(hMutex);
			return false;
		}
	}

	// ��ʼ�� COM ���
	if (FAILED(CoInitialize(nullptr)))
	{
		E2D_ERROR("��ʼ�� COM ���ʧ��");
		return false;
	}

	// �����豸�޹���Դ
	if (!Renderer::__createDeviceIndependentResources())
	{
		E2D_ERROR("��Ⱦ���豸�޹���Դ����ʧ��");
		return false;
	}

	// ��ʼ������
	if (!Window::__init(title, width, height))
	{
		E2D_ERROR("��ʼ������ʧ��");
		return false;
	}

	// �����豸�����Դ
	if (!Renderer::__createDeviceResources())
	{
		E2D_ERROR("��Ⱦ���豸�����Դ����ʧ��");
		return false;
	}

	// ��ʼ�� DirectInput
	if (!Input::__init())
	{
		E2D_ERROR("��ʼ�� DirectInput ʧ��");
		return false;
	}

	// ��ʼ��������
	if (!Music::__init())
	{
		E2D_ERROR("��ʼ�� XAudio2 ʧ��");
		return false;
	}

	// ��ʼ��·��
	if (!Path::__init(title))
	{
		E2D_WARNING("Path::__init failed!");
	}

	// ������Ϸ����
	s_sGameName = title;

	// ��ʼ���ɹ�
	s_bInitialized = true;

	return s_bInitialized;
}

void easy2d::Game::start(int fpsLimit)
{
	if (!s_bInitialized)
	{
		E2D_ERROR("��ʼ��Ϸǰδ���г�ʼ��");
		return;
	}

	// ��ʼ������������
	SceneManager::__init();
	// ��ʾ����
	::ShowWindow(Window::getHWnd(), SW_SHOWNORMAL);
	// ˢ�´�������
	::UpdateWindow(Window::getHWnd());
	// ��ʼ����ʱ
	Time::__init(fpsLimit);

	s_bEndGame = false;

	while (!s_bEndGame)
	{
		// ��������Ϣ
		Window::__poll();
		// ˢ��ʱ��
		Time::__updateNow();

		// �ж��Ƿ�ﵽ��ˢ��״̬
		if (Time::__isReady())
		{
			Input::__update();			// ��ȡ�û�����
			Timer::__update();			// ���¶�ʱ��
			ActionManager::__update();	// ���¶���������
			SceneManager::__update();	// ���³�������
			Renderer::__render();		// ��Ⱦ��Ϸ����
			GC::clear();				// �����ڴ�

			Time::__updateLast();		// ˢ��ʱ����Ϣ
		}
		else
		{
			Time::__sleep();			// �����߳�
		}
	}

	// �������ʱ��������Դ
	{
		// ɾ������
		ActionManager::__uninit();
		// �������ֲ�������Դ
		MusicPlayer::__uninit();
		// ��ն�ʱ��
		Timer::__uninit();
		// ɾ�����г���
		SceneManager::__uninit();
		// �������
		GC::clear();
	}
}

void easy2d::Game::pause()
{
	s_bPaused = true;
}

void easy2d::Game::resume()
{
	if (s_bInitialized && s_bPaused)
	{
		Game::reset();
	}
	s_bPaused = false;
}

void easy2d::Game::reset()
{
	if (s_bInitialized && !s_bEndGame)
	{
		Time::__reset();
		ActionManager::__resetAll();
		Timer::__resetAll();
	}
}

bool easy2d::Game::isPaused()
{
	return s_bPaused;
}

void easy2d::Game::quit()
{
	s_bEndGame = true;	// ���������������Ϸ�Ƿ����
}

void easy2d::Game::destroy()
{
	if (!s_bInitialized)
		return;

	// ���ͼƬ����
	Image::clearCache();
	// �������������Դ
	Music::__uninit();
	// �ر�����
	Input::__uninit();
	// ������Ⱦ�����Դ
	Renderer::__discardResources();
	// ���ٴ���
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

easy2d::String easy2d::Game::getName()
{
	return s_sGameName;
}
