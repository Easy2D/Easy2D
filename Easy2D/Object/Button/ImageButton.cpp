#include "..\..\easy2d.h"


ImageButton::ImageButton() :
	m_pNormalImage(nullptr),
	m_pMouseInImage(nullptr),
	m_pSelectedImage(nullptr),
	m_pUnableImage(nullptr)
{
}

ImageButton::ImageButton(LPCTSTR image) :
	ImageButton()
{
	setNormal(new Image(image));	// ���ð�ť������״̬ʱ��ͼƬ
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

void ImageButton::_setStatus(Status status)
{
	if (m_eStatus != status)
	{
		if (status == MOUSEIN)
		{
			if (m_pMouseInImage) setRect(m_pMouseInImage->getRect());
		}
		else if (status == SELECTED)
		{
			if (m_pSelectedImage) setRect(m_pSelectedImage->getRect());
		}
		else
		{
			setRect(m_pNormalImage->getRect());
		}
	}
	MouseNode::_setStatus(status);
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
		// ����ͼƬ����趨��ť��С
		setSize(m_pNormalImage->getSize());
		// ���¼���ͼƬλ��
		_resetPosition();
	}
}

void ImageButton::setMouseIn(Image * image)
{
	if (image)
	{
		SAFE_RELEASE(m_pMouseInImage);
		m_pMouseInImage = image;
		m_pMouseInImage->retain();
		_resetPosition();
	}
}

void ImageButton::setSelected(Image * image)
{
	if (image)
	{
		SAFE_RELEASE(m_pSelectedImage);
		m_pSelectedImage = image;
		m_pSelectedImage->retain();
		_resetPosition();
	}
}

void ImageButton::setUnable(Image * image)
{
	if (image)
	{
		SAFE_RELEASE(m_pUnableImage);
		m_pUnableImage = image;
		m_pUnableImage->retain();
		_resetPosition();
	}
}

void ImageButton::_resetPosition()
{
	if (m_pNormalImage)
	{
		// ���ݰ�ťλ�ú�ͼƬ�������ͼƬλ�þ�����ʾ
		m_pNormalImage->setPos(getX(), getY());
	}
	if (m_pMouseInImage)
	{
		m_pMouseInImage->setPos(
			getX() + (getWidth() - m_pMouseInImage->getWidth()) / 2,
			getY() + (getHeight() - m_pMouseInImage->getHeight()) / 2);
	}
	if (m_pSelectedImage)
	{
		m_pSelectedImage->setPos(
			getX() + (getWidth() - m_pSelectedImage->getWidth()) / 2,
			getY() + (getHeight() - m_pSelectedImage->getHeight()) / 2);
	}
	if (m_pUnableImage)
	{
		m_pUnableImage->setPos(
			getX() + (getWidth() - m_pUnableImage->getWidth()) / 2,
			getY() + (getHeight() - m_pUnableImage->getHeight()) / 2);
	}
}
