#include "..\emsg.h"
#include "..\Win\winbase.h"
#include <vector>


// �����Ϣ
e2d::EMouseMsg mouseMsg;
// ������Ϣ
e2d::EKeyMsg keyMsg;
// �����Ϣ������
std::vector<e2d::EMouseListener*> m_vMouseListeners;
// ������Ϣ������
std::vector<e2d::EKeyListener*> m_vKeyListeners;


DWORD e2d::EMouseMsg::getX()
{
	return LOWORD(mouseMsg.m_lParam);
}

DWORD e2d::EMouseMsg::getY()
{
	return HIWORD(mouseMsg.m_lParam);
}

e2d::EPoint e2d::EMouseMsg::getPos()
{
	return EPoint(LOWORD(mouseMsg.m_lParam), HIWORD(mouseMsg.m_lParam));
}

bool e2d::EMouseMsg::isLButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_LBUTTON;
}

bool e2d::EMouseMsg::isMButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_MBUTTON;
}

bool e2d::EMouseMsg::isRButtonDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_RBUTTON;
}

bool e2d::EMouseMsg::isShiftDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_SHIFT;
}

bool e2d::EMouseMsg::isCtrlDown()
{
	return GET_KEYSTATE_WPARAM(mouseMsg.m_wParam) == MK_CONTROL;
}

DWORD e2d::EMouseMsg::getWheelDelta()
{
	return GET_WHEEL_DELTA_WPARAM(mouseMsg.m_wParam);
}

e2d::EMouseMsg::MOUSE_MSG e2d::EMouseMsg::getMsg()
{
	return MOUSE_MSG(mouseMsg.m_nMsg);
}



e2d::EKeyMsg::KEYBOARD_MSG e2d::EKeyMsg::getMsg()
{
	return KEYBOARD_MSG(keyMsg.m_nMsg);
}

e2d::EKeyMsg::KEY e2d::EKeyMsg::getVal()
{
	return KEY(keyMsg.m_wParam);
}

DWORD e2d::EKeyMsg::getCount()
{
	return (((DWORD)keyMsg.m_lParam) & 0x0000FFFF);
}

bool e2d::EKeyMsg::isKeyDown(KEY key)
{
	if (::GetAsyncKeyState((int)key) & 0x8000)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyMsg::isCapitalLockOn()
{
	if (::GetKeyState(VK_CAPITAL) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyMsg::isNumpadLockOn()
{
	if (::GetKeyState(VK_NUMLOCK) & 0x0001)
	{
		return true;
	}
	return false;
}

bool e2d::EKeyMsg::isScrollLockOn()
{
	if (::GetKeyState(VK_SCROLL) & 0x0001)
	{
		return true;
	}
	return false;
}



void e2d::EMsgManager::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���������Ϣ
	mouseMsg.m_nMsg = message;
	mouseMsg.m_wParam = wParam;
	mouseMsg.m_lParam = lParam;
	// ִ�������Ϣ��������
	for (auto mlistener : m_vMouseListeners)
	{
		if (mlistener->isRunning())
		{
			mlistener->runCallback();
		}
	}
}

void e2d::EMsgManager::KeyboardProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���水����Ϣ
	keyMsg.m_nMsg = message;
	keyMsg.m_wParam = wParam;
	keyMsg.m_lParam = lParam;
	// ִ�а�����Ϣ��������
	for (auto klistener : m_vKeyListeners)
	{
		if (klistener->isRunning())
		{
			klistener->runCallback();
		}
	}
}

void e2d::EMsgManager::addListener(e2d::EMouseListener * listener, EScene * pParentScene)
{
	WARN_IF(listener == nullptr, "Add NULL EMouseListener!");
	WARN_IF(pParentScene == nullptr, "Bind EMouseListener on a NULL Scene!");

	if (listener && pParentScene)
	{
		listener->start();
		listener->retain();
		listener->setParentScene(pParentScene);
		m_vMouseListeners.push_back(listener);
	}
}

void e2d::EMsgManager::addListener(e2d::EKeyListener * listener, EScene * pParentScene)
{
	WARN_IF(listener == nullptr, "Add NULL EKeyListener!");
	WARN_IF(pParentScene == nullptr, "Bind EKeyListener on a NULL Scene!");

	if (listener && pParentScene)
	{
		listener->start();
		listener->retain();
		listener->setParentScene(pParentScene);
		m_vKeyListeners.push_back(listener);
	}
}

void e2d::EMsgManager::startListener(EString name)
{
	// ���������Ϣ������
	for (auto l : m_vMouseListeners)
	{
		if (l->getName() == name)
		{
			l->start();
		}
	}
	// ����������Ϣ������
	for (auto l : m_vKeyListeners)
	{
		if (l->getName() == name)
		{
			l->start();
		}
	}
}

void e2d::EMsgManager::stopListener(EString name)
{
	// ֹͣ�����Ϣ������
	for (auto l : m_vMouseListeners)
	{
		if (l->getName() == name)
		{
			l->stop();
		}
	}
	// ֹͣ������Ϣ������
	for (auto l : m_vKeyListeners)
	{
		if (l->getName() == name)
		{
			l->stop();
		}
	}
}

void e2d::EMsgManager::delListener(EString name)
{
	// ɾ�������Ϣ������
	for (std::vector<EMouseListener*>::iterator iter = m_vMouseListeners.begin(); iter != m_vMouseListeners.end();)
	{
		if ((*iter)->getName() == name)
		{
			(*iter)->autoRelease();
			(*iter)->release();
			delete (*iter);
			iter = m_vMouseListeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	// ɾ��������Ϣ������
	for (std::vector<EKeyListener*>::iterator iter = m_vKeyListeners.begin(); iter != m_vKeyListeners.end();)
	{
		if ((*iter)->getName() == name)
		{
			(*iter)->autoRelease();
			(*iter)->release();
			delete (*iter);
			iter = m_vKeyListeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void e2d::EMsgManager::waitAllListenersOnScene(EScene * scene)
{
	// ���������Ϣ������
	for (auto l : m_vMouseListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->wait();
		}
	}
	// ���𰴼���Ϣ������
	for (auto l : m_vKeyListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->wait();
		}
	}
}

void e2d::EMsgManager::notifyAllListenersOnScene(EScene * scene)
{
	// ���������Ϣ������
	for (auto l : m_vMouseListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->notify();
		}
	}
	// ����������Ϣ������
	for (auto l : m_vKeyListeners)
	{
		if (l->getParentScene() == scene)
		{
			l->notify();
		}
	}
}

