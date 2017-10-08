#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// �������ص�����������
static std::vector<MouseMsg*> s_vListeners;

// �����Ϣ
static MOUSEMSG s_mouseMsg;

void MouseMsg::__exec()
{
	// ��ȡ�����Ϣ
	while (MouseHit())
	{
		// ��ȡ�����Ϣ
		s_mouseMsg = GetMouseMsg();
		// ִ�г�������
		App::get()->getCurrentScene()->_exec();
		// ִ���������ص�����
		for (auto l : s_vListeners)	// ѭ���������е�������
		{
			if (!l->m_bWaiting && l->m_bRunning)
			{
				l->onMouseMsg();	// ִ�лص�����
			}
		}
	}
}

MouseMsg::MouseMsg() :
	m_callback([]() {}),
	m_pParentScene(nullptr),
	m_bRunning(true),
	m_bWaiting(false)
{
}

MouseMsg::MouseMsg(TString name, const MOUSE_CALLBACK & callback) :
	m_sName(name),
	m_callback(callback),
	m_pParentScene(nullptr),
	m_bRunning(true),
	m_bWaiting(false)
{
}

MouseMsg::~MouseMsg()
{
}

void MouseMsg::onMouseMsg()
{
	m_callback();
}

void MouseMsg::addListener(TString name, const MOUSE_CALLBACK & callback)
{
	// �����µļ�������
	auto listener = new MouseMsg(name, callback);
	// ���ڳ�����
	listener->m_pParentScene = App::getLoadingScene();
	// ����µİ����ص�����
	s_vListeners.push_back(listener);
}

void MouseMsg::startListener(TString name)
{
	// ����������ͬ�ļ�����
	for (auto l : s_vListeners)
	{
		if (l->m_sName == name)
		{
			l->start();
		}
	}
}

void MouseMsg::stopListener(TString name)
{
	// ����������ͬ�ļ�����
	for (auto l : s_vListeners)
	{
		if (l->m_sName == name)
		{
			l->stop();
		}
	}
}

void MouseMsg::delListener(TString name)
{
	// ����������
	std::vector<MouseMsg*>::iterator iter;
	// ѭ���������м�����
	for (iter = s_vListeners.begin(); iter != s_vListeners.end();)
	{
		// ������ͬ���Ƶļ�����
		if ((*iter)->m_sName == name)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			iter = s_vListeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void MouseMsg::start()
{
	m_bRunning = true;
}

void MouseMsg::stop()
{
	m_bRunning = false;
}

void MouseMsg::wait()
{
	m_bWaiting = true;
}

void MouseMsg::notify()
{
	m_bWaiting = false;
}

void MouseMsg::clearAllListeners()
{
	// ɾ�����м�����
	for (auto l : s_vListeners)
	{
		delete l;
	}
	// �������
	s_vListeners.clear();
}

void MouseMsg::notifyAllSceneListeners(Scene * scene)
{
	for (auto l : s_vListeners)
	{
		if (l->m_pParentScene == scene)
		{
			l->notify();
		}
	}
}

void MouseMsg::waitAllSceneListeners(Scene * scene)
{
	for (auto l : s_vListeners)
	{
		if (l->m_pParentScene == scene)
		{
			l->wait();
		}
	}
}

void MouseMsg::clearAllSceneListeners(Scene * scene)
{
	// ����������
	std::vector<MouseMsg*>::iterator iter;
	// ѭ���������м�����
	for (iter = s_vListeners.begin(); iter != s_vListeners.end(); iter++)
	{
		// ������ͬ���Ƶļ�����
		if ((*iter)->m_pParentScene == scene)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			s_vListeners.erase(iter);
		}
	}
}

bool MouseMsg::isLButtonDown()
{
	return s_mouseMsg.mkLButton;
}

bool MouseMsg::isRButtonDown()
{
	return s_mouseMsg.mkRButton;
}

bool MouseMsg::isMButtonDown()
{
	return s_mouseMsg.mkMButton;
}

int MouseMsg::getX()
{
	return s_mouseMsg.x;
}

int MouseMsg::getY()
{
	return s_mouseMsg.y;
}

CPoint MouseMsg::getPos()
{
	return CPoint(s_mouseMsg.x, s_mouseMsg.y);
}

int MouseMsg::getWheel()
{
	return s_mouseMsg.wheel;
}

MouseMsg::MESSAGE MouseMsg::getMsg()
{
	return MESSAGE(s_mouseMsg.uMsg);
}
