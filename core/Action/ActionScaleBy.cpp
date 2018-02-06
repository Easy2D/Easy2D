#include "..\eactions.h"


e2d::EActionScaleBy::EActionScaleBy(float duration, float scale)
	: EActionGradual(duration)
{
	m_nVariationX = scale;
	m_nVariationY = scale;
}

e2d::EActionScaleBy::EActionScaleBy(float duration, float scaleX, float scaleY)
	: EActionGradual(duration)
{
	m_nVariationX = scaleX;
	m_nVariationY = scaleY;
}

void e2d::EActionScaleBy::_init()
{
	EActionGradual::_init();
	if (m_pTarget)
	{
		m_nBeginScaleX = m_pTarget->getScaleX();
		m_nBeginScaleY = m_pTarget->getScaleY();
	}
}

void e2d::EActionScaleBy::_update()
{
	EActionGradual::_update();

	if (m_pTarget == nullptr)
	{
		this->stop();
		return;
	}

	// ���Žڵ�
	m_pTarget->setScale(
		m_nBeginScaleX + m_nVariationX * m_fRateOfProgress, 
		m_nBeginScaleX + m_nVariationX * m_fRateOfProgress);
}

e2d::EActionScaleBy * e2d::EActionScaleBy::clone() const
{
	return new EActionScaleBy(m_fDuration, m_nVariationX, m_nVariationY);
}

e2d::EActionScaleBy * e2d::EActionScaleBy::reverse() const
{
	return new EActionScaleBy(m_fDuration, -m_nVariationX, -m_nVariationY);
}