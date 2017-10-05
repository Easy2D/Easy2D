#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

Sprite::Sprite() :
	m_nAlpha(255),
	m_fScaleX(1),
	m_fScaleY(1),
	m_pImage(nullptr)
{

}

Sprite::Sprite(Image * image) :
	Sprite()
{
	setImage(image);
}

Sprite::Sprite(LPCTSTR imageFileName) :
	Sprite()
{
	setImage(new Image(imageFileName));
}

Sprite::~Sprite()
{
	SafeRelease(m_pImage);
}

bool Sprite::_exec(bool active)
{
	return false;
}

void Sprite::_onDraw()
{
	// display ����Ϊ false����δ����ͼƬ��Դʱ�������Ƹ�ͼƬ
	if (!m_bDisplay || !m_pImage->m_pCImage)
	{
		return;
	}
	// ����ͼƬ
	if (m_pImage->m_pCImage->GetBPP() == 32)
	{
		m_pImage->m_pCImage->AlphaBlend(GetImageHDC(), getRect(), m_pImage->m_SrcRect, m_nAlpha, AC_SRC_OVER);
	}
	else
	{
		m_pImage->m_pCImage->Draw(GetImageHDC(), getRect(), m_pImage->m_SrcRect);
	}
}

void Sprite::setImage(Image * image)
{
	SafeRelease(m_pImage);
	m_pImage = image;
	setSize(int(m_pImage->getWidth() * m_fScaleX), int(m_pImage->getHeight() * m_fScaleY));
	m_pImage->retain();
}

bool Sprite::isCollisionWith(Sprite * sprite)
{
	return IntersectRect(NULL, &getRect(), &sprite->getRect());
}

void Sprite::addAction(Action * action)
{
	if (action)
	{
		// �������� Sprite ��
		action->m_pTargetSprite = this;
		// ���������붯�����������������ᴦ���������
		ActionManager::addAction(action);
	}
}

void Sprite::pauseAllActions()
{
	ActionManager::pauseSpriteAllActions(this);
}

void Sprite::resumeAllActions()
{
	ActionManager::resumeSpriteAllActions(this);
}

void Sprite::stopAllActions()
{
	ActionManager::stopSpriteAllActions(this);
}

float Sprite::getScaleX() const
{
	return m_fScaleX;
}

float Sprite::getScaleY() const
{
	return m_fScaleY;
}

float Sprite::getOpacity() const
{
	return m_nAlpha / 255.0f;
}

void Sprite::setScale(float scaleX, float scaleY)
{
	m_fScaleX = max(scaleX, 0);
	m_fScaleY = max(scaleY, 0);
	setSize(int(m_pImage->getWidth() * scaleX), int(m_pImage->getHeight() * scaleY));
}

void Sprite::setOpacity(float opacity)
{
	m_nAlpha = BYTE(min(max(opacity, 0), 1) * 255);
}
