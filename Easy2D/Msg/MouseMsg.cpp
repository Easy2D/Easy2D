#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// �����Ϣ
static MouseMsg s_mouseMsg = MouseMsg();

// �� EasyX �� MOUSEMSG ת��Ϊ MouseMsg
static void ConvertMsg(MOUSEMSG msg);

void easy2d::MouseMsg::__exec()
{
	// ��ȡ�����Ϣ
	while (MouseHit())
	{
		// ת�������Ϣ
		ConvertMsg(GetMouseMsg());
		// ִ�г�������
		Application::get()->getCurrentScene()->_exec();
	}
}

MouseMsg MouseMsg::getMsg()
{
	return s_mouseMsg;	// ��ȡ��ǰ�����Ϣ
}

bool MouseMsg::getLButtonDown()
{
	return s_mouseMsg.mkLButton;
}

bool MouseMsg::getRButtonDown()
{
	return s_mouseMsg.mkRButton;
}

bool MouseMsg::getMButtonDown()
{
	return s_mouseMsg.mkMButton;
}

int MouseMsg::getMouseX()
{
	return s_mouseMsg.x;
}

int MouseMsg::getMouseY()
{
	return s_mouseMsg.y;
}

int MouseMsg::getMouseWheel()
{
	return s_mouseMsg.wheel;
}

bool MouseMsg::getMouseMovedMsg()
{
	return s_mouseMsg.uMsg == WM_MOUSEMOVE;
}

bool MouseMsg::getLButtonDBClickedMsg()
{
	return s_mouseMsg.uMsg == WM_LBUTTONDBLCLK;
}

bool MouseMsg::getLButtonDownMsg()
{
	return s_mouseMsg.uMsg == WM_LBUTTONDOWN;
}

bool MouseMsg::getLButtonUpMsg()
{
	return s_mouseMsg.uMsg == WM_LBUTTONUP;
}

bool MouseMsg::getRButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_RBUTTONDBLCLK;
}

bool MouseMsg::getRButtonDownMsg()
{
	return s_mouseMsg.uMsg == WM_RBUTTONDOWN;
}

bool MouseMsg::getRButtonUpMsg()
{
	return s_mouseMsg.uMsg == WM_LBUTTONUP;
}

bool MouseMsg::getMButtonDBClicked()
{
	return s_mouseMsg.uMsg == WM_MBUTTONDBLCLK;
}

bool MouseMsg::getMButtonDownMsg()
{
	return s_mouseMsg.uMsg == WM_MBUTTONDOWN;
}

bool MouseMsg::getMButtonUpMsg()
{
	return s_mouseMsg.uMsg == WM_MBUTTONUP;
}

bool MouseMsg::getWheelMsg()
{
	return s_mouseMsg.uMsg == WM_MOUSEWHEEL;
}

void MouseMsg::resetMouseMsg()
{
	s_mouseMsg.uMsg = 0;
}

void ConvertMsg(MOUSEMSG msg)
{
	// �� MOUSEMSG ת��Ϊ MouseMsg
	/// ��Ȼ MOUSEMSG �� MouseMsg ��������һ����
	/// ����Ϊ��ʵ�� Easy2D �� EasyX ����Ը��룬���Զ������µ� MouseMsg
	/// ���� Msg ����Ϣ�Ĵ����ͳһ�� WinAPIʵ�֣��������� EasyX �ĺ���
	s_mouseMsg.uMsg = msg.uMsg;
	s_mouseMsg.mkLButton = msg.mkLButton;
	s_mouseMsg.mkMButton = msg.mkMButton;
	s_mouseMsg.mkRButton = msg.mkRButton;
	s_mouseMsg.wheel = msg.wheel;
	s_mouseMsg.x = msg.x;
	s_mouseMsg.y = msg.y;
}
