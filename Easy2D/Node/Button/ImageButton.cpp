#include "..\..\Easy2d.h"


ImageButton::ImageButton() :
	m_pNormalImage(nullptr),
	m_pMouseInImage(nullptr),
	m_pSelectedImage(nullptr),
	m_pUnableImage(nullptr)
{
	m_nWidth = 0;
	m_nHeight = 0;
}

ImageButton::ImageButton(Image * image) :
	ImageButton()
{
	setNormal(image);	// ���ð�ť������״̬ʱ��ͼƬ
}

ImageButton::~ImageButton()
{
	// ����ͼƬ�����ü�����һ
	SAFE_RELEASE(m_pNormalImage);
	SAFE_RELEASE(m_pMouseInImage);
	SAFE_RELEASE(m_pSelectedImage);
	SAFE_RELEASE(m_pUnableImage);
}

void ImageButton::_onNormal()
{
	if (m_pNormalImage)
	{
		m_pNormalImage->_onDraw();
	}
}

void ImageButton::_onMouseIn()
{
	if (m_pMouseInImage)
	{
		m_pMouseInImage->_onDraw();
	}
	else
	{
		_onNormal();
	}
}

void ImageButton::_onSelected()
{
	if (m_pSelectedImage)
	{
		m_pSelectedImage->_onDraw();
	}
	else
	{
		_onNormal();
	}
}

void ImageButton::_onDisable()
{
	if (m_pUnableImage)
	{
		m_pUnableImage->_onDraw();
	}
	else
	{
		_onNormal();
	}
}

void ImageButton::setNormal(Image * image)
{
	if (image)
	{
		// ԭͼƬ���ü�����һ
		SAFE_RELEASE(m_pNormalImage);
		// �޸�ͼƬ
		m_pNormalImage = image;
		// ��ͼƬ���ü�����һ
		m_pNormalImage->retain();
		// ���¼���ͼƬλ��
		resetImagePosition();
	}
}

void ImageButton::setMouseIn(Image * image)
{
	if (image)
	{
		SAFE_RELEASE(m_pMouseInImage);
		m_pMouseInImage = image;
		m_pMouseInImage->retain();
		resetImagePosition();
	}
}

void ImageButton::setSelected(Image * image)
{
	if (image)
	{
		SAFE_RELEASE(m_pSelectedImage);
		m_pSelectedImage = image;
		m_pSelectedImage->retain();
		resetImagePosition();
	}
}

void ImageButton::setUnable(Image * image)
{
	if (image)
	{
		SAFE_RELEASE(m_pUnableImage);
		m_pUnableImage = image;
		m_pUnableImage->retain();
		resetImagePosition();
	}
}

void ImageButton::setX(int x)
{
	Node::setX(x);
	resetImagePosition();
}

void ImageButton::setY(int y)
{
	Node::setY(y);
	resetImagePosition();
}

void ImageButton::setPos(int x, int y)
{
	Node::setPos(x, y);
	resetImagePosition();
}

void ImageButton::resetImagePosition()
{
	if (m_pNormalImage)
	{
		// ����ͼƬ����趨��ť��С
		m_nWidth = m_pNormalImage->getWidth();
		m_nHeight = m_pNormalImage->getHeight();
		// ���ݰ�ťλ�ú�ͼƬ�������ͼƬλ�þ�����ʾ
		m_pNormalImage->setPos(m_nX, m_nY);
	}
	if (m_pMouseInImage)
	{
		m_pMouseInImage->setPos(
			m_nX + (m_nWidth - m_pMouseInImage->getWidth()) / 2,
			m_nY + (m_nHeight - m_pMouseInImage->getHeight()) / 2);
	}
	if (m_pSelectedImage)
	{
		m_pSelectedImage->setPos(
			m_nX + (m_nWidth - m_pSelectedImage->getWidth()) / 2,
			m_nY + (m_nHeight - m_pSelectedImage->getHeight()) / 2);
	}
	if (m_pUnableImage)
	{
		m_pUnableImage->setPos(
			m_nX + (m_nWidth - m_pUnableImage->getWidth()) / 2,
			m_nY + (m_nHeight - m_pUnableImage->getHeight()) / 2);
	}
}
