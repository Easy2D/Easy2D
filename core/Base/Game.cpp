#include "..\ebase.h"
#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\etransitions.h"
#include "..\etools.h"


// ������Ϸ��ֹ
static bool s_bEndGame = false;
// ������Ϸ��ͣ
static bool s_bPaused = false;
// �Ƿ���й���ʼ��
static bool s_bInitialized = false;
// AppName
static e2d::EString s_sAppName;


bool e2d::EGame::init(LPCTSTR sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID, bool bNoClose, bool bNoMiniSize, bool bTopMost)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	// ��ʼ�� COM ���
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		// �����豸�޹���Դ
		if (ERenderer::__createDeviceIndependentResources())
		{
			// ��ʼ������
			if (EWindow::__init(sTitle, nWidth, nHeight, pIconID, bNoClose, bNoMiniSize, bTopMost))
			{
				// �����豸�����Դ
				if (ERenderer::__createDeviceResources())
				{
					// ���� Client ��С
					EWindow::setSize(nWidth, nHeight);
					// ����Ĭ�� AppName
					if (s_sAppName.isEmpty())
						s_sAppName = EWindow::getTitle();
					// ��־��ʼ���ɹ�
					s_bInitialized = true;
				}
			}
		}
	}
	
	return s_bInitialized;
}

int e2d::EGame::run()
{
	if (!s_bInitialized)
	{
		ASSERT(false, "You must initialize EGame first!");
		return -1;
	}

	// ��ʼ�� DirectInput
	EInput::__init();
	// ��ʼ����ʱ����
	ETime::__init();
	// �����һ������
	ESceneManager::__enterNextScene();
	// ��ʾ����
	EWindow::showWindow();
	// ˢ�´�������
	::UpdateWindow(EWindow::getHWnd());
	// ��������Ϣ
	EWindow::__poll();
	// ˢ��ʱ����Ϣ
	ETime::__updateNow();
	ETime::__updateLast();

	// ����ʱ��
	int nWaitMS = 0;

	while (!s_bEndGame)
	{
		// ��������Ϣ
		EWindow::__poll();
		// ˢ��ʱ����Ϣ
		ETime::__updateNow();

		// �ж��Ƿ�ﵽ��ˢ��״̬
		if (ETime::getDeltaTime() >= 17)
		{
			ETime::__updateLast();
			EGame::__update();
		}
		else
		{
			// �����߳�
			ETime::__sleep();
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
	// ���ô�������
	EWindow::__uninit();
	// �ر�����
	EInput::__uninit();
	// �ָ���ʱ����
	ETime::__uninit();
	// ɾ����Ⱦ�����Դ
	ERenderer::__discardResources();

	CoUninitialize();
}

void e2d::EGame::__update()
{
	EInput::__updateDeviceState();		// ��ȡ�û�����

	if (!s_bPaused)
	{
		ETimerManager::__update();		// ��ʱ��������ִ�г���
		EActionManager::__update();		// ����������ִ�г���
		ESceneManager::__update();		// ������Ϸ����
		ERenderer::__render();			// ��Ⱦ��Ϸ����
	}
}

void e2d::EGame::setAppName(const EString &appname)
{
	s_sAppName = appname;
}

e2d::EString e2d::EGame::getAppName()
{
	return s_sAppName;
}
