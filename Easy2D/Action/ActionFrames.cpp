#include "..\easy2d.h"

ActionFrames::ActionFrames() :
	m_nFrameIndex(0)
{
	// ֡����Ĭ�� .5s ˢ��һ��
	setInterval(500);
}

ActionFrames::~ActionFrames()
{
	for (auto frame : m_vFrames)
	{
		frame->release();
	}
}

void ActionFrames::_init()
{
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}

bool ActionFrames::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	// �ж�ʱ�����Ƿ��㹻
	while (nNow.QuadPart - m_nLast.QuadPart > m_nAnimationInterval.QuadPart)
	{
		// ������ķ������¼�¼ʱ��
		m_nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % m_nAnimationInterval.QuadPart);
		m_pParent->setImage(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		if (m_nFrameIndex == m_vFrames.size())
		{
			return true;
		}
	}
	return false;
}

void ActionFrames::_reset()
{
	m_nFrameIndex = 0;
}

void ActionFrames::addFrame(Image * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

ActionFrames * ActionFrames::copy()
{
	auto a = new ActionFrames();
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

ActionFrames * ActionFrames::reverse() const
{
	auto a = new ActionFrames();
	a->m_vFrames = this->m_vFrames;
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
