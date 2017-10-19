#include "..\eactions.h"
#include "..\Win\winbase.h"
#include <typeinfo>

e2d::EAnimation::EAnimation() :
	m_nFrameIndex(0)
{
	// ֡����Ĭ�� 0.1s ˢ��һ��
	setInterval(100);
}

e2d::EAnimation::EAnimation(LONGLONG frameDelay) :
	m_nFrameIndex(0)
{
	setInterval(frameDelay);
}

e2d::EAnimation::~EAnimation()
{
	for (auto frame : m_vFrames)
	{
		SafeRelease(&frame);
	}
}

void e2d::EAnimation::_init()
{
	// �ж�ִ��֡������Ŀ�������Ƿ��� ESprite
	ASSERT(
		typeid(ESprite) == typeid(*m_pTarget),
		"Only ESprite can use EAnimation!"
	);
	EAction::_init();
	// ��¼��ǰʱ��
	m_tLast = GetNow();
}

void e2d::EAnimation::_callOn()
{
	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}
	// �ж�ʱ�����Ƿ��㹻
	while (GetInterval(m_tLast) > m_nAnimationInterval)
	{
		// ���¼�¼ʱ��
		m_tLast += milliseconds(m_nAnimationInterval);
		reinterpret_cast<ESprite*>(m_pTarget)->loadFromSpriteFrame(m_vFrames[m_nFrameIndex]);
		m_nFrameIndex++;
		// �ж϶����Ƿ����
		if (m_nFrameIndex == m_vFrames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EAnimation::_reset()
{
	EAction::_reset();
	m_nFrameIndex = 0;
	// ��¼��ǰʱ��
	m_tLast = steady_clock::now();
}

void e2d::EAnimation::addFrame(ESpriteFrame * frame)
{
	if (frame)
	{
		m_vFrames.push_back(frame);
		frame->retain();
	}
}

e2d::EAnimation * e2d::EAnimation::clone() const
{
	auto a = new EAnimation(this->m_nAnimationInterval);
	for (auto f : m_vFrames)
	{
		a->addFrame(f);
	}
	return a;
}

e2d::EAnimation * e2d::EAnimation::reverse() const
{
	auto a = this->clone();
	a->m_vFrames.reserve(m_vFrames.size());
	return a;
}
