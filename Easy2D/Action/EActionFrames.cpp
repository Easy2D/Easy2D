#include "..\eactions.h"
#include "..\Win\winbase.h"

e2d::EActionFrames::EActionFrames() :
	m_nFrameIndex(0)
{
	// ֡����Ĭ�� .5s ˢ��һ��
	setInterval(500);
}

e2d::EActionFrames::EActionFrames(LONGLONG frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

e2d::EActionFrames::~EActionFrames()
{
	for (auto frame : m_vFrames)
	{
		frame->autoRelease();
		frame->release();
	}
}

void e2d::EActionFrames::_init()
{
	EAction::_init();
	// ��¼��ǰʱ��
	m_nLast = GetNow();
}

void e2d::EActionFrames::_callOn()
{
	// �ж�ʱ�����Ƿ��㹻
	while (GetInterval(m_nLast) > m_nAnimationInterval)
	{
		// ���¼�¼ʱ��
		m_nLast += milliseconds(m_nAnimationInterval);
		//m_pTarget->setImage(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// �ж϶����Ƿ����
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionFrames::_reset()
{
	EAction::_reset();
	m_nFrameIndex = 0;
	// ��¼��ǰʱ��
	m_nLast = steady_clock::now();
}

void e2d::EActionFrames::addFrame(ESpriteFrame * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

e2d::EActionFrames * e2d::EActionFrames::clone() const
{
	auto a = new EActionFrames(this->m_nAnimationInterval);
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

e2d::EActionFrames * e2d::EActionFrames::reverse() const
{
	auto a = this->clone();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
