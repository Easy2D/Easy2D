#include "..\eactions.h"


e2d::ActionOpacityBy::ActionOpacityBy(float duration, float opacity) :
	Animation(duration)
{
	m_nVariation = opacity;
}

e2d::ActionOpacityBy::~ActionOpacityBy()
{
}

void e2d::ActionOpacityBy::_init()
{
	Animation::_init();
	m_nBeginVal = m_pTarget->getOpacity();
}

void e2d::ActionOpacityBy::_exec()
{
	while (Animation::_isDelayEnough())
	{
		// �����ƶ�λ��
		float scale = static_cast<float>(m_nDuration) / m_nTotalDuration;
		// �ƶ� Sprite
		m_pTarget->setOpacity(m_nBeginVal + m_nVariation * scale);
		// �ж϶����Ƿ����
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::ActionOpacityBy::_reset()
{
	Animation::_reset();
}

e2d::ActionOpacityBy * e2d::ActionOpacityBy::copy() const
{
	return new ActionOpacityBy(m_nAnimationInterval / 1000.0f, m_nVariation);
}

e2d::ActionOpacityBy * e2d::ActionOpacityBy::reverse() const
{
	return new ActionOpacityBy(m_nTotalDuration / 1000.0f, -m_nVariation);
}