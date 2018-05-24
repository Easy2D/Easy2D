#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include "..\e2dcollider.h"


// ������Ϸ��ֹ
static bool s_bEndGame = true;
// ������Ϸ��ͣ
static bool s_bPaused = false;
// �Ƿ���й���ʼ��
static bool s_bInitialized = false;
// ��Ϸ����
static e2d::String s_sGameName;


bool e2d::Game::init(const String& name, const String& mutexName)
{
	if (s_bInitialized)
	{
		WARN("The game has been initialized!");
		return false;
	}

	if (!mutexName.isEmpty())
	{
		// �������̻�����
		HANDLE hMutex = ::CreateMutex(nullptr, TRUE, L"Easy2DApp-" + mutexName);

		if (hMutex == nullptr)
		{
			WARN("CreateMutex Failed!");
		}
		else if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// ��������Ѿ����ڲ����������У�������ʾ
			Window::info(L"��Ϸ�������������д򿪣�", L"��ʾ");
			// �رս��̻�����
			::CloseHandle(hMutex);
			return false;
		}
	}

	// ��ʼ�� COM ���
	CoInitialize(nullptr);

	bool bRendererDevIndResInit = false,
		bWindowInit = false,
		bRendererDevResInit = false,
		bInputInit = false,
		bMusicInit = false;

	auto DestroyResources = [&]()
	{
		if (bRendererDevIndResInit) Renderer::__discardResources();
		if (bWindowInit) Window::__init();
		if (bRendererDevResInit) Renderer::__discardDeviceResources();
		if (bInputInit) Input::__uninit();
		if (bMusicInit) Music::__uninit();
	};

	// �����豸�޹���Դ
	if (Renderer::__createDeviceIndependentResources())
	{
		bRendererDevIndResInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"��Ⱦ���豸�޹���Դ����ʧ��");
	}

	// ��ʼ������
	if (Window::__init())
	{
		bWindowInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"��ʼ������ʧ��");
	}

	// �����豸�����Դ
	if (Renderer::__createDeviceResources())
	{
		bRendererDevResInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"��Ⱦ���豸�����Դ����ʧ��");
	}

	// ��ʼ�� DirectInput
	if (Input::__init())
	{
		bInputInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"��ʼ�� DirectInput ʧ��");
	}

	// ��ʼ��������
	if (Music::__init())
	{
		bMusicInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"��ʼ�� XAudio2 ʧ��");
	}

	// ������Ϸ����
	s_sGameName = name;

	// ��ʼ��·��
	if (!Path::__init())
	{
		WARN("Path::__init failed!");
	}

	// ��ʼ���ɹ�
	s_bInitialized = true;

	return s_bInitialized;
}

void e2d::Game::start(bool autoRelease/* true */)
{
	if (!s_bInitialized)
	{
		throw Exception(L"��ʼ��Ϸǰδ���г�ʼ��");
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
			Timer::__update();				// ��ʱ��������ִ�г���
			ActionManager::__update();		// ����������ִ�г���
			while (Time::__isReady())
			{
				Input::__update();			// ��ȡ�û�����
				SceneManager::__update();	// ���³�������
				Time::__updateLast();		// ˢ��ʱ����Ϣ
			}
			Renderer::__render();			// ��Ⱦ��Ϸ����
			GC::__update();		// ˢ���ڴ��
		}
		else
		{
			Time::__sleep();				// �����߳�
		}
	}

	s_bEndGame = true;

	if (autoRelease)
	{
		Game::destroy();
	}
}

void e2d::Game::pause()
{
	s_bPaused = true;
}

void e2d::Game::resume()
{
	if (s_bInitialized && s_bPaused)
	{
		Game::reset();
	}
	s_bPaused = false;
}

void e2d::Game::reset()
{
	if (s_bInitialized && !s_bEndGame)
	{
		Time::__reset();
		ActionManager::__resetAll();
		Timer::__resetAll();
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

void e2d::Game::destroy()
{
	if (!s_bInitialized)
		return;

	// ɾ�����г���
	SceneManager::__uninit();
	// ɾ�����������
	Input::__clearListeners();
	// ɾ����ײ������
	Collision::__clearListeners();
	// ɾ������
	ActionManager::__uninit();
	// �������ֲ�������Դ
	Player::__uninit();
	// ɾ�����ж���
	GC::__clear();
	// ���ͼƬ����
	Image::clearCache();
	// �������������Դ
	Music::__uninit();
	// ��ն�ʱ��
	Timer::__uninit();
	// �ر�����
	Input::__uninit();
	// ������Ⱦ�����Դ
	Renderer::__discardResources();
	// ���ٴ���
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

e2d::String e2d::Game::getName()
{
	return s_sGameName;
}
