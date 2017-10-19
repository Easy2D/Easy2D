#include "..\eactions.h"


e2d::EActionScaleBy::EActionScaleBy(float duration, float scaleX, float scaleY) :
	EAnimation(duration)
{
	m_nVariationX = scaleX;
	m_nVariationY = scaleY;
}

e2d::EActionScaleBy::~EActionScaleBy()
{
}

void e2d::EActionScaleBy::_init()
{
	EAnimation::_init();
	m_nBeginScaleX = m_pTarget->getScaleX();
	m_nBeginScaleY = m_pTarget->getScaleY();
}

void e2d::EActionScaleBy::_exec()
{
	while (EAnimation::_isDelayEnough())
	{
		// �����ƶ�λ��
		float scale = static_cast<float>(m_nDuration) / m_nTotalDuration;
		// �ƶ� Sprite
		m_pTarget->setScale(m_nBeginScaleX + m_nVariationX * scale, m_nBeginScaleX + m_nVariationX * scale);
		// �ж϶����Ƿ����
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionScaleBy::_reset()
{
	EAnimation::_reset();
}

e2d::EActionScaleBy * e2d::EActionScaleBy::copy() const
{
	return new EActionScaleBy(m_nAnimationInterval / 1000.0f, m_nVariationX, m_nVariationY);
}

e2d::EActionScaleBy * e2d::EActionScaleBy::reverse() const
{
	return new EActionScaleBy(m_nTotalDuration / 1000.0f, -m_nVariationX, -m_nVariationY);
}