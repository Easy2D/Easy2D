#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// �������ص�����������
static std::vector<MouseMsg*> s_vMouseMsg;

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
		for (auto m : s_vMouseMsg)	// ѭ���������е�������
		{
			m->onMouseMsg();		// ִ�лص�����
		}
	}
}

MouseMsg::MouseMsg() :
	m_callback([]() {}),
	m_pParentScene(nullptr)
{
}

MouseMsg::MouseMsg(TString name, const MOUSE_CALLBACK & callback) :
	m_sName(name),
	m_callback(callback),
	m_pParentScene(nullptr)
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
	auto mouse = new MouseMsg(name, callback);
	mouse->m_pParentScene = App::getCurrentScene();
	// ����µİ����ص�����
	s_vMouseMsg.push_back(mouse);
}

bool MouseMsg::delListener(TString name)
{
	// ����������
	std::vector<MouseMsg*>::iterator iter;
	// ѭ���������м�����
	for (iter = s_vMouseMsg.begin(); iter != s_vMouseMsg.end(); iter++)
	{
		// ������ͬ���Ƶļ�����
		if ((*iter)->m_sName == name)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			s_vMouseMsg.erase(iter);
			return true;
		}
	}
	// ��δ�ҵ�ͬ�����Ƶļ����������� false
	return false;
}

void MouseMsg::bindListenersWithScene(Scene * scene)
{
	for (auto m : s_vMouseMsg)
	{
		if (!m->m_pParentScene)
		{
			m->m_pParentScene = App::getCurrentScene();
		}
	}
}

void MouseMsg::clearAllSceneListeners(Scene * scene)
{
	// ����������
	std::vector<MouseMsg*>::iterator iter;
	// ѭ���������м�����
	for (iter = s_vMouseMsg.begin(); iter != s_vMouseMsg.end(); iter++)
	{
		// ������ͬ���Ƶļ�����
		if ((*iter)->m_pParentScene == scene)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			s_vMouseMsg.erase(iter);
		}
	}
}

void MouseMsg::clearAllListeners()
{
	// ɾ�����м�����
	for (auto m : s_vMouseMsg)
	{
		delete m;
	}
	// �������
	s_vMouseMsg.clear();
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

bool MouseMsg::isOnMouseMoved()
{
	return s_mouseMsg.uMsg == WM_MOUSEMOVE;
}

bool MouseMsg::isOnLButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_LBUTTONDBLCLK;
}

bool MouseMsg::isOnLButtonDown()
{
	return s_mouseMsg.uMsg == WM_LBUTTONDOWN;
}

bool MouseMsg::isOnLButtonUp()
{
	return s_mouseMsg.uMsg == WM_LBUTTONUP;
}

bool MouseMsg::isOnRButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_RBUTTONDBLCLK;
}

bool MouseMsg::isOnRButtonDown()
{
	return s_mouseMsg.uMsg == WM_RBUTTONDOWN;
}

bool MouseMsg::isOnRButtonUp()
{
	return s_mouseMsg.uMsg == WM_LBUTTONUP;
}

bool MouseMsg::isOnMButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_MBUTTONDBLCLK;
}

bool MouseMsg::isOnMButtonDown()
{
	return s_mouseMsg.uMsg == WM_MBUTTONDOWN;
}

bool MouseMsg::isOnMButtonUp()
{
	return s_mouseMsg.uMsg == WM_MBUTTONUP;
}

bool MouseMsg::isOnWheel()
{
	return s_mouseMsg.uMsg == WM_MOUSEWHEEL;
}

void MouseMsg::resetMouseMsg()
{
	s_mouseMsg.uMsg = 0;
}
