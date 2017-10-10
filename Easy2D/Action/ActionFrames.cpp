#include "..\easy2d.h"
#include "..\Win\winbase.h"

ActionFrames::ActionFrames() :
	m_nFrameIndex(0)
{
	// ֡����Ĭ�� .5s ˢ��һ��
	setInterval(500);
}

ActionFrames::ActionFrames(LONGLONG frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

ActionFrames::~ActionFrames()
{
	for (auto frame : m_vFrames)
	{
		frame->autoRelease();
		frame->release();
	}
}

void ActionFrames::_init()
{
	Action::_init();
	// ��¼��ǰʱ��
	m_nLast = steady_clock::now();
}

void ActionFrames::_exec(steady_clock::time_point nNow)
{
	// �ж�ʱ�����Ƿ��㹻
	while (duration_cast<milliseconds>(nNow - m_nLast).count() > m_nAnimationInterval)
	{
		// ���¼�¼ʱ��
		m_nLast += milliseconds(m_nAnimationInterval);
		m_pTargetSprite->setImage(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// �ж϶����Ƿ����
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void ActionFrames::_reset()
{
	Action::_reset();
	m_nFrameIndex = 0;
	// ��¼��ǰʱ��
	m_nLast = steady_clock::now();
}

void ActionFrames::addFrame(Image * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

ActionFrames * ActionFrames::copy() const
{
	auto a = new ActionFrames(this->m_nAnimationInterval);
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

ActionFrames * ActionFrames::reverse() const
{
	auto a = this->copy();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
