#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <functional>

namespace e2d
{

struct EPoint
{
	float x;
	float y;

	EPoint()
	{
		x = 0;
		y = 0;
	}

	EPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	EPoint operator + (EPoint const & p)
	{
		return EPoint(x + p.x, y + p.y);
	}

	EPoint operator - (EPoint const & p)
	{
		return EPoint(x - p.x, y - p.y);
	}
};

struct ESize
{
	float width;
	float height;

	ESize()
	{
		width = 0;
		height = 0;
	}

	ESize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	ESize operator + (ESize const & size)
	{
		return ESize(width + size.width, height + size.height);
	}

	ESize operator - (ESize const & size)
	{
		return ESize(width - size.width, height - size.height);
	}
};


struct EWindowStyle
{
	LPCTSTR m_pIconID;	/* ����ͼ�� ID */
	bool m_bNoClose;	/* ���ùرհ�ť */
	bool m_bNoMiniSize;	/* ������С����ť */
	bool m_bTopMost;	/* �����ö� */

	EWindowStyle()
	{
		m_pIconID = 0;
		m_bNoClose = false;
		m_bNoMiniSize = false;
		m_bTopMost = false;
	}

	EWindowStyle(
		LPCTSTR pIconID
	)
	{
		m_pIconID = pIconID;
		m_bNoClose = false;
		m_bNoMiniSize = false;
		m_bTopMost = false;
	}

	EWindowStyle(
		LPCTSTR pIconID,
		bool bNoClose,
		bool bNoMiniSize,
		bool bTopMost
	)
	{
		m_pIconID = pIconID;
		m_bNoClose = bNoClose;
		m_bNoMiniSize = bNoMiniSize;
		m_bTopMost = bTopMost;
	}
};

typedef EPoint EVec;

typedef std::wstring EString;

template<typename T>
using EVector = std::vector<T>;

// ��ʱ���ص�����������Ϊ�ö�ʱ�������õĴ������� 0 ��ʼ��
typedef std::function<void(int)> TIMER_CALLBACK;

// ��ť����ص�����
typedef std::function<void()> BUTTON_CLICK_CALLBACK;

// ������Ϣ�����ص�����
typedef std::function<void()> KEY_LISTENER_CALLBACK;

// �����Ϣ�����ص�����
typedef std::function<void()> MOUSE_LISTENER_CALLBACK;

// �������Ϣ�����ص�����������Ϊ���λ�ã�
typedef std::function<void(EPoint mousePos)> MOUSE_CLICK_LISTENER_CALLBACK;

// ��갴����Ϣ�����ص�����������Ϊ����λ�ã�
typedef MOUSE_CLICK_LISTENER_CALLBACK MOUSE_PRESS_LISTENER_CALLBACK;

// ���˫����Ϣ�����ص�����������Ϊ˫��λ�ã�
typedef MOUSE_CLICK_LISTENER_CALLBACK MOUSE_DBLCLK_LISTENER_CALLBACK;

// ����϶���Ϣ�����ص�����������Ϊ�϶�ǰλ�ú��϶���λ�ã�
typedef std::function<void(EPoint begin, EPoint end)> MOUSE_DRAG_LISTENER_CALLBACK;

}