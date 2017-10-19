#include "..\eactions.h"


e2d::EActionMoveBy::EActionMoveBy(float duration, EVec vector) :
	EAnimation(duration)
{
	m_MoveVector = vector;
}

e2d::EActionMoveBy::~EActionMoveBy()
{
}

void e2d::EActionMoveBy::_init()
{
	EAnimation::_init();
	m_BeginPos = m_pTarget->getPos();
}

void e2d::EActionMoveBy::_exec()
{
	while (EAnimation::_isDelayEnough())
	{
		// �����ƶ�λ��
		float scale = static_cast<float>(m_nDuration) / m_nTotalDuration;
		// �ƶ� Sprite
		m_pTarget->setPos(
			m_BeginPos.x + m_MoveVector.x * scale, 
			m_BeginPos.y + m_MoveVector.y * scale
		);
		// �ж϶����Ƿ����
		if (_isEnd())
		{
			this->stop();
			break;
		}
	}
}

void e2d::EActionMoveBy::_reset()
{
	EAnimation::_reset();
}

e2d::EActionMoveBy * e2d::EActionMoveBy::copy() const
{
	return new EActionMoveBy(m_nAnimationInterval / 1000.0f, m_MoveVector);
}

e2d::EActionMoveBy * e2d::EActionMoveBy::reverse() const
{
	return new EActionMoveBy(m_nTotalDuration / 1000.0f, EVec(-m_MoveVector.x, -m_MoveVector.y));
}