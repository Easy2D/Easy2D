#include "..\ebase.h"
#include "..\emanagers.h"


// ������Ϸ��ֹ
static bool s_bEndGame = false;
// ������Ϸ��ͣ
static bool s_bPaused = false;
// �Ƿ���й���ʼ��
static bool s_bInitialized = false;
// AppName
static e2d::EString s_sAppName;


bool e2d::EGame::init(LPCTSTR sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID, LPCTSTR sAppname)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	do
	{
		// ��ʼ�� COM ���
		if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		{
			WARN_IF(true, "CoInitializeEx Failed!");
			break;
		}

		// �����豸�޹���Դ
		if (!ERenderer::__createDeviceIndependentResources())
		{
			WARN_IF(true, "ERenderer::__createDeviceIndependentResources Failed!");
			break;
		}

		// ��ʼ������
		if (!EWindow::__init(sTitle, nWidth, nHeight, pIconID))
		{
			WARN_IF(true, "EWindow::__init Failed!");
			break;
		}

		// �����豸�����Դ
		if (!ERenderer::__createDeviceResources())
		{
			WARN_IF(true, "ERenderer::__createDeviceResources Failed!");
			break;
		}

		// ��ʼ����ʱ
		if (!ETime::__init())
		{
			WARN_IF(true, "ETime::__init Failed!");
			break;
		}

		// ��ʼ�� DirectInput
		if (!EInput::__init())
		{
			WARN_IF(true, "EInput::__init Failed!");
			break;
		}

		// ��ʼ��������
		if (!EMusicManager::__init())
		{
			WARN_IF(true, "EMusicManager::__init Failed!");
			break;
		}

		// ���� Client ��С
		EWindow::setSize(nWidth, nHeight);
		// ���� AppName
		s_sAppName = (sAppname != nullptr) ? sAppname : EWindow::getTitle();
		// ��־��ʼ���ɹ�
		s_bInitialized = true;

	} while (0);

	return s_bInitialized;
}

int e2d::EGame::run()
{
	if (!s_bInitialized)
	{
		ASSERT(false, "You must initialize EGame first!");
		return -1;
	}

	// ��ʼ������������
	ESceneManager::__init();
	// ��ʾ����
	::ShowWindow(EWindow::getHWnd(), SW_SHOWNORMAL);
	// ˢ�´�������
	::UpdateWindow(EWindow::getHWnd());
	// ��������Ϣ
	EWindow::__poll();
	// ˢ��ʱ����Ϣ
	ETime::__updateNow();
	ETime::__updateLast();

	while (!s_bEndGame)
	{
		// ��������Ϣ
		EWindow::__poll();
		// ˢ��ʱ����Ϣ
		ETime::__updateNow();

		// �ж��Ƿ�ﵽ��ˢ��״̬
		if (ETime::getDeltaTime() >= 17)
		{
			ETime::__updateLast();		// ˢ��ʱ����Ϣ
			EGame::__update();			// ������Ϸ����
			ERenderer::__render();		// ��Ⱦ��Ϸ����
		}
		else
		{
			EObjectManager::__flush();	// ˢ���ڴ��
			ETime::__sleep();			// �����߳�
		}
	}

	return 0;
}

void e2d::EGame::pause()
{
	s_bPaused = true;
}

void e2d::EGame::resume()
{
	if (isPaused())
	{
		s_bPaused = false;
		// ˢ�µ�ǰʱ��
		ETime::__updateLast();
		// ���ö����Ͷ�ʱ��
		EActionManager::__resetAllActions();
		ETimerManager::__resetAllTimers();
	}
}

bool e2d::EGame::isPaused()
{
	return s_bPaused;
}

void e2d::EGame::quit()
{
	s_bEndGame = true;	// ���������������Ϸ�Ƿ����
}

void e2d::EGame::uninit()
{
	// ɾ�����г���
	ESceneManager::__uninit();
	// ���ô�������
	EWindow::__uninit();
	// �ر�����
	EInput::__uninit();
	// �رղ�����
	EMusicManager::__uninit();
	// �ָ���ʱ����
	ETime::__uninit();
	// ���ͼƬ����
	EImage::clearCache();
	// ɾ����Ⱦ�����Դ
	ERenderer::__discardResources();
	// ˢ���ڴ��
	EObjectManager::__flush();

	CoUninitialize();

	s_bInitialized = false;
}

void e2d::EGame::__update()
{
	if (s_bPaused)
		return;

	EInput::__updateDeviceState();	// ��ȡ�û�����
	ETimerManager::__update();		// ��ʱ��������ִ�г���
	EActionManager::__update();		// ����������ִ�г���
	ESceneManager::__update();		// ������Ϸ����
}

e2d::EString e2d::EGame::getAppName()
{
	return s_sAppName;
}
