#pragma once
#include <e2dbase.h>

namespace easy2d 
{
	class MouseMsg
	{
		friend class App;

	public:
		MouseMsg();
		MouseMsg(tstring name, const MOUSE_CALLBACK& callback);
		~MouseMsg();

		// ��Ӽ��̼���
		static void addListener(tstring name, const MOUSE_CALLBACK& callback);
		// ɾ�����̼���
		static bool delListener(tstring name);
		// ɾ�����м��̼���
		static void clearAllListeners();
		// ����Ƿ���
		static bool isLButtonDown();
		// �Ҽ��Ƿ���
		static bool isRButtonDown();
		// �м��Ƿ���
		static bool isMButtonDown();
		// ��ȡ���X����
		static int getX();
		// ��ȡ���Y����
		static int getY();
		// ��ȡ�������
		static CPoint getPos();
		// ��ȡ������ֵ
		static int getWheel();
		// ����ƶ���Ϣ
		static bool isOnMouseMoved();
		// ���˫����Ϣ
		static bool isOnLButtonDBClicked();
		// �Ҽ�������Ϣ
		static bool isOnLButtonDown();
		// ���������Ϣ
		static bool isOnLButtonUp();
		// �Ҽ�˫����Ϣ
		static bool isOnRButtonDBClicked();
		// �Ҽ�������Ϣ
		static bool isOnRButtonDown();
		// �Ҽ�������Ϣ
		static bool isOnRButtonUp();
		// �м�˫����Ϣ
		static bool isOnMButtonDBClicked();
		// �м�������Ϣ
		static bool isOnMButtonDown();
		// �м�������Ϣ
		static bool isOnMButtonUp();
		// �����ֲ�����Ϣ
		static bool isOnWheel();
		// ��������Ϣ
		static void resetMouseMsg();

	private:
		static void __exec();

	protected:
		tstring			m_sName;
		MOUSE_CALLBACK	m_callback;

	protected:
		// ִ�лص�����
		void onMouseMsg();
	};


	class KeyMsg
	{
		friend class App;

	public:
		KeyMsg(tstring name, const KEY_CALLBACK& callback);
		~KeyMsg();

		// ִ�лص�����
		void onKbHit(VK_KEY key);

		// ��Ӽ��̼���
		static void addListener(tstring name, const KEY_CALLBACK& callback);
		// ɾ�����̼���
		static bool delListener(tstring name);
		// ɾ�����м��̼���
		static void clearAllListeners();
		// �жϼ��Ƿ񱻰��£����·���true
		static bool isKeyDown(VK_KEY key);

	public:
		// ��ĸ��ֵ
		static const VK_KEY A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
		// ���ּ�ֵ
		static const VK_KEY NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0;
		// С���ּ���ֵ
		static const VK_KEY NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, NUMPAD_0;
		// ���Ƽ�ֵ
		static const VK_KEY Enter, Space, Up, Down, Left, Right, Esc, Shift, LShift, RShift, Ctrl, LCtrl, RCtrl;
		// F ��ֵ
		static const VK_KEY F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

	private:
		static void __exec();

	protected:
		tstring			m_sName;
		KEY_CALLBACK	m_callback;
	};
}	// End of easy2d namespace