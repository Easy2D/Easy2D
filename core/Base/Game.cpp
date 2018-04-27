#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"


// ������Ϸ��ֹ
static bool s_bEndGame = false;
// ������Ϸ��ͣ
static bool s_bPaused = false;
// �Ƿ���й���ʼ��
static bool s_bInitialized = false;
// ��Ϸ����
static e2d::String s_sGameName;


bool e2d::Game::init(String sGameName)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	// ��ʼ�� COM ���
	CoInitialize(NULL);

	// �����豸�޹���Դ
	if (!Renderer::__createDeviceIndependentResources())
	{
		WARN_IF(true, "Renderer::__createDeviceIndependentResources Failed!");
		goto dev_ind_res_fail;
	}

	// ��ʼ������
	if (!Window::__init())
	{
		WARN_IF(true, "Window::__init Failed!");
		goto window_fail;
	}

	// �����豸�����Դ
	if (!Renderer::__createDeviceResources())
	{
		WARN_IF(true, "Renderer::__createDeviceResources Failed!");
		goto dev_res_fail;
	}

	// ��ʼ�� DirectInput
	if (!Input::__init())
	{
		WARN_IF(true, "Input::__init Failed!");
		goto input_fail;
	}

	// ��ʼ��������
	if (!Music::__init())
	{
		WARN_IF(true, "Music::__init Failed!");
		Music::__uninit();
	}

	// ������Ϸ����
	s_sGameName = sGameName;

	// ��ʼ��·��
	if (!Path::__init())
	{
		WARN_IF(true, "Path::__init Failed!");
	}

	// ��ʼ���ɹ�
	s_bInitialized = true;
	goto succeeded;

input_fail:
	Input::__uninit();

dev_res_fail:
	Renderer::__discardDeviceResources();

window_fail:
	Window::__init();

dev_ind_res_fail:
	Renderer::__discardResources();

succeeded:
	return s_bInitialized;
}

int e2d::Game::start(bool bAutoRelease/* true */)
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
			Timer::__update();				// ��ʱ��������ִ�г���
			ActionManager::__update();		// ����������ִ�г���
			while (Time::__isReady())
			{
				Input::__update();			// ��ȡ�û�����
				SceneManager::__update();	// ���³�������
				Time::__updateLast();		// ˢ��ʱ����Ϣ
			}
			Renderer::__render();			// ��Ⱦ��Ϸ����
		}
		else
		{
			ObjectManager::__update();		// ˢ���ڴ��
			Time::__sleep();				// �����߳�
		}
	}

	if (bAutoRelease)
	{
		Game::destroy();
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
	// ɾ�����г���
	SceneManager::__uninit();
	// ɾ��������
	InputManager::__uninit();
	ColliderManager::__uninit();
	// ɾ������
	ActionManager::__uninit();
	// ɾ�����ж���
	ObjectManager::__clear();
	// ���ͼƬ����
	Image::clearCache();
	// �������������Դ
	Music::__uninit();
	// ��ն�ʱ��
	Timer::__uninit();
	// �ر�����
	Input::__uninit();
	// �ָ���ʱ����
	Time::__uninit();
	// ɾ����Ⱦ�����Դ
	Renderer::__discardResources();
	// ���ٴ���
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

bool e2d::Game::createMutex(String sMutexName, String sWindowTitle)
{
	// �������̻�����
	HANDLE m_hMutex = ::CreateMutex(NULL, TRUE, L"Easy2DApp-" + sMutexName);

	if (m_hMutex == nullptr)
	{
		WARN_IF(true, "CreateMutex Failed!");
		return true;
	}

	// ��������Ѿ����ڲ�����������
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// �رս��̻�����
		::CloseHandle(m_hMutex);
		// ��ָ������
		if (!sWindowTitle.isEmpty())
		{
			// ��ȡ���ھ��
			HWND hProgramWnd = ::FindWindow(L"Easy2DApp", sWindowTitle);
			if (hProgramWnd)
			{
				// ��ȡ������ʾ״̬
				WINDOWPLACEMENT wpm;
				::GetWindowPlacement(hProgramWnd, &wpm);
				// �����еĳ��򴰿ڻ�ԭ������״̬
				wpm.showCmd = SW_SHOW;
				::SetWindowPlacement(hProgramWnd, &wpm);
				::SetWindowPos(hProgramWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		return false;
	}
	return true;
}

e2d::String e2d::Game::getName()
{
	return s_sGameName;
}
