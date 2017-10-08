#include "..\easy2d.h"

BatchSprite::BatchSprite()
{
}

BatchSprite::~BatchSprite()
{
}

void BatchSprite::addSprite(Sprite * sprite, int z_Order)
{
	if (sprite == nullptr) return;

	// ���ýڵ�ĸ�����
	sprite->setParentScene(this->getParentScene());
	// ���ýڵ��������ڵ��е� z ��˳��
	sprite->setZOrder(z_Order);
	// ��������ü�����һ
	sprite->retain();

	// �� z ��˳�����ڵ�
	size_t size = m_vSprites.size();
	for (unsigned i = 0; i <= size; i++)
	{
		if (i != size)
		{
			if (z_Order < m_vSprites.at(i)->getZOrder())
			{
				m_vSprites.insert(m_vSprites.begin() + i, sprite);
				break;
			}
		}
		else
		{
			m_vSprites.push_back(sprite);
			break;
		}
	}
}

bool BatchSprite::delSprite(Sprite * sprite)
{
	if (sprite == nullptr) return false;

	// Ѱ���Ƿ�����ͬ�ڵ�
	std::vector<Sprite*>::iterator iter;
	for (iter = m_vSprites.begin(); iter != m_vSprites.end(); iter++)
	{
		// �ҵ���ͬ�ڵ�
		if ((*iter) == sprite)
		{
			// ��������ü�����һ
			(*iter)->release();
			// ȥ���ýڵ�
			m_vSprites.erase(iter);
			return true;
		}
	}
	return false;
}

void BatchSprite::clearAllSprites()
{
	// ���нڵ�����ü�����һ
	for (auto s : m_vSprites)
	{
		s->release();
	}
	// ��մ���ڵ������
	m_vSprites.clear();
}

bool BatchSprite::_exec(bool active)
{
	// �����ڵ��Ƿ���ʾ
	if (!m_bDisplay)
	{
		return false;
	}
	// ������������ӽڵ�
	for (int i = int(m_vSprites.size() - 1); i >= 0; i--)
	{
		if (m_vSprites[i]->_exec(active))
		{
			active = false;
		}
	}
	// ���ӽڵ�ȡ���˻��潹�㣬��ýڵ�Ҳȡ���˽���
	return !active;
}

void BatchSprite::_onDraw()
{
	// �ڵ��Ƿ���ʾ
	if (!m_bDisplay)
	{
		return;
	}

	// �����λ�û����ӽڵ�
	App::setOrigin(App::getOriginX() + getX(), App::getOriginY() + getY());
	for (auto sprite : m_vSprites)
	{
		sprite->_onDraw();
	}
	App::setOrigin(App::getOriginX() - getX(), App::getOriginY() - getY());
}

Sprite * BatchSprite::isCollisionWith(Sprite * sprite)
{
	for (int i = int(m_vSprites.size() - 1); i >= 0; i--)
	{
		if (m_vSprites[i]->isCollisionWith(sprite))
		{
			return m_vSprites[i];
		}
	}
	return nullptr;
}

Sprite * BatchSprite::isPointIn(CPoint point)
{
	for (int i = int(m_vSprites.size() - 1); i >= 0; i--)
	{
		if (m_vSprites[i]->isPointIn(point))
		{
			return m_vSprites[i];
		}
	}
	return nullptr;
}

void BatchSprite::addAction(Action * action)
{
	Sprite::addAction(action);
}

float BatchSprite::getScaleX() const
{
	return m_fScaleX;
}

float BatchSprite::getScaleY() const
{
	return m_fScaleY;
}

float BatchSprite::getOpacity() const
{
	return m_nAlpha / 255.0f;
}

void BatchSprite::setScale(float scaleX, float scaleY)
{
	m_fScaleX = scaleX;
	m_fScaleY = scaleY;
	for (auto s : m_vSprites)
	{
		s->setScale(scaleX, scaleY);
	}
}

void BatchSprite::setOpacity(float opacity)
{
	m_nAlpha = BYTE(min(max(opacity, 0), 1) * 255);
	for (auto s : m_vSprites)
	{
		s->setOpacity(opacity);
	}
}

void BatchSprite::setImage(Image * image)
{
	for (auto s : m_vSprites)
	{
		s->setImage(image);
	}
}
