#include "..\easy2d.h"

ActionOpacityBy::ActionOpacityBy(float duration, float opacity) :
	Animation(duration)
{
	m_nVariation = opacity;
}

ActionOpacityBy::~ActionOpacityBy()
{
}

void ActionOpacityBy::_init()
{
	Animation::_init();
	m_nBeginVal = m_pParent->getOpacity();
}

bool ActionOpacityBy::_exec(LARGE_INTEGER nNow)
{
	if (m_bStop) return true;
	if (!m_bRunning) return false;

	while (Animation::_exec(nNow))
	{
		// �����ƶ�λ��
		float scale = float(m_nDuration) / m_nTotalDuration;
		// �ƶ� Sprite
		m_pParent->setOpacity(m_nBeginVal + m_nVariation * scale);
		// �ж϶����Ƿ����
		if (m_nDuration >= m_nTotalDuration)
		{
			return true;
		}
	}
	return false;
}

void ActionOpacityBy::_reset()
{
	Animation::_reset();
}

ActionOpacityBy * ActionOpacityBy::copy()
{
	auto a = new ActionOpacityBy(*this);
	a->_reset();
	return a;
}

ActionOpacityBy * ActionOpacityBy::reverse() const
{
	return new ActionOpacityBy(m_nTotalDuration / 1000.0f, -m_nVariation);
}