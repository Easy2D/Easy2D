#include "..\eactions.h"


e2d::EActionMoveBy::EActionMoveBy(float duration, EVec vector) :
	EActionGradual(duration)
{
	m_MoveVector = vector;
}

void e2d::EActionMoveBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_BeginPos = m_pTarget->getPos();
	}
}

void e2d::EActionMoveBy::_callOn()
{
	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}
	while (EActionGradual::_isDelayEnough())
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
	EActionGradual::_reset();
}

e2d::EActionMoveBy * e2d::EActionMoveBy::clone() const
{
	return new EActionMoveBy(m_nAnimationInterval / 1000.0f, m_MoveVector);
}

e2d::EActionMoveBy * e2d::EActionMoveBy::reverse() const
{
	return new EActionMoveBy(m_nTotalDuration / 1000.0f, EVec(-m_MoveVector.x, -m_MoveVector.y));
}