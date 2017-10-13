#pragma once
#include "ebase.h"

namespace e2d
{

class EObjectManager
{
public:
	// ��һ���ڵ�����ͷų�
	static void add(
		e2d::EObject * nptr
	);

	// ɾ�����нڵ�
	static void clearAllObjects();

private:
	friend EApp;

	// ˢ���ڴ��
	static void __flush();
};


class EMouseMsg
{
public:
	// �����Ϣ����
	enum MESSAGE
	{
		MOVE = 0x0200,	// ����ƶ�
		LBUTTON_DOWN,	// ����������
		LBUTTON_UP,		// ������̧��
		LBUTTON_DBLCLK,	// ������˫��
		RBUTTON_DOWN,	// ����Ҽ�����
		RBUTTON_UP,		// ����Ҽ�̧��
		RBUTTON_DBLCLK,	// ����Ҽ�˫��
		MBUTTON_DOWN,	// ����м�����
		MBUTTON_UP,		// ����м�̧��
		MBUTTON_DBLCLK,	// ����м�˫��
		WHEEL			// ��������
	};

public:
	EPoint m_Pos;
	UINT m_nMsg = 0;
	bool m_bLButtonDown = false;
	bool m_bRButtonDown = false;
	bool m_bMButtonDown = false;
};


class EMouseListener :
	public EObject
{
protected:
	friend EMsgManager;
	e2d::EString name;

};


class EMsgManager
{
public:
	static void setMouseMsg(
		UINT message
	);

	static e2d::EMouseMsg::MESSAGE getMouseMsg();

	static void addListener(
		EMouseListener * listener
	);

private:
	friend EApp;

	static void __exec();
};

}