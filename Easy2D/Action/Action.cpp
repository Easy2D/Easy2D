#include "..\easy2d.h"
#include <assert.h>

Action::Action() :
	m_bRunning(true),
	m_bWaiting(false),
	m_bEnding(false),
	m_bInit(false),
	m_pTargetSprite(nullptr),
	m_pParentScene(nullptr)
{
	// Ĭ�϶��� 15ms ����һ��
	setInterval(15);
}

Action::~Action()
{
}

bool Action::isRunning()
{
	return m_bRunning && !m_bWaiting;
}

bool Action::isEnding()
{
	return m_bEnding;
}

void Action::start()
{
	m_bRunning = true;
}

void Action::resume()
{
	m_bRunning = true;
}

void Action::pause()
{
	m_bRunning = false;
}

void Action::stop()
{
	m_bEnding = true;
}
void Action::wait()
{
	m_bWaiting = true;
}

void Action::notify()
{
	m_bWaiting = false;
}

void Action::setInterval(UINT ms)
{
	// ���ö�����ʱ����
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	m_nAnimationInterval.QuadPart = (LONGLONG)(ms / 1000.0 * nFreq.QuadPart);
	// ����ʱ������ʱ��
	this->m_nMilliSeconds = ms;
}

Action * Action::reverse() const
{
	assert(0);
	return nullptr;
}

Sprite * Action::getTarget()
{
	return m_pTargetSprite;
}

void Action::_init()
{
	m_bInit = true;
}

void Action::_reset()
{
	m_bInit = false;
	m_bEnding = false;
}
