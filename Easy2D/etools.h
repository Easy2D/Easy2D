#pragma once
#include "ebase.h"

namespace e2d
{

class EMsgManager;


class EObjectManager
{
	friend EApp;

public:
	// ��һ���ڵ�����ͷų�
	static void add(
		e2d::EObject * nptr
	);

	// ɾ�����нڵ�
	static void clearAllObjects();

private:
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
	friend EMsgManager;

public:
	EMouseListener();

	EMouseListener(
		EString name
	);

	EMouseListener(
		const MOUSE_CALLBACK &callback
	);

	EMouseListener(
		EString name,
		const MOUSE_CALLBACK &callback
	);

	// ��ȡ������״̬
	bool isRunnint() const;

	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ����ȴ�״̬
	void wait();

	// ����
	void notify();

	// ִ�м������ص�����
	void runCallback();

	// ��ȡ����������
	EString getName() const;

	// ��ȡ���������ڳ���
	EScene * getParentScene();
	
	// ���ü���������
	void setName(EString name);

	// ���ü������ص�����
	void setCallback(const MOUSE_CALLBACK &callback);

protected:
	EString			m_sName;
	bool			m_bRunning;
	bool			m_bWaiting;
	MOUSE_CALLBACK	m_callback;
	EScene *		m_pParentScene;
};


class EMsgManager
{
	friend EApp;

public:
	static void setMouseMsg(
		UINT message
	);

	static e2d::EMouseMsg::MESSAGE getMouseMsg();

	static void addListener(
		EMouseListener * listener
	);

private:
	
	static void __exec();
};

}