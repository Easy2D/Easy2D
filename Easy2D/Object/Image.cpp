#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

// �� PNG ͼ���������ת��
static void CrossImage(CImage &img);

Image::Image() :
	m_nAlpha(255),
	m_fScaleX(1),
	m_fScaleY(1)
{
}

Image::Image(LPCTSTR ImageFile) :
	Image()
{
	setImage(ImageFile);	// ����ͼƬ��Դ
}

Image::Image(LPCTSTR ImageFile, int x, int y, int width, int height) :
	Image()
{
	setImage(ImageFile, x, y, width, height);	// ����ͼƬ��Դ�Ͳü���С
}

Image::~Image()
{
}

void Image::_onDraw()
{
	// display ����Ϊ false����δ����ͼƬ��Դʱ�������Ƹ�ͼƬ
	if (!m_bDisplay || m_Image.IsNull())
	{
		return;
	}
	// ����ͼƬ
	if (m_Image.GetBPP() == 32)
	{
		m_Image.AlphaBlend(GetImageHDC(), m_Rect, m_SrcRect, m_nAlpha, AC_SRC_OVER);
	}
	else
	{
		m_Image.Draw(GetImageHDC(), m_Rect, m_SrcRect);
	}
}

float Image::getScaleX() const
{
	return m_fScaleX;
}

float Image::getScaleY() const
{
	return m_fScaleY;
}

float Image::getOpacity() const
{
	return m_nAlpha / 255.0f;
}

bool Image::setImage(LPCTSTR ImageFile)
{
	//�ж�ͼƬ·���Ƿ����
	if (!PathFileExists(ImageFile))
	{
		return false;
	}
	// ���ԭ��Դ
	if (!m_Image.IsNull())
	{
		m_Image.Destroy();
	}
	// ����ͼƬ
	m_Image.Load(ImageFile);
	// ����ʧ��
	if (m_Image.IsNull())
	{
		return false;
	}
	// ȷ�ϸ�ͼ����� Alpha ͨ��
	if (m_Image.GetBPP() == 32)
	{
		// ͸��ͼƬ����
		CrossImage(m_Image);
	}
	reset();

	return true;
}

bool Image::setImage(LPCTSTR ImageFile, int x, int y, int width, int height)
{
	if (!setImage(ImageFile))
	{
		return false;
	}
	// �ü�ͼƬ��С
	crop(x, y, width, height);

	return true;
}

bool Image::setImageFromRes(LPCTSTR pResName)
{
	// ����Դ����ͼƬ����֧�� PNG��
	m_Image.LoadFromResource(GetModuleHandle(NULL), pResName);
	// ����ʧ��
	if (m_Image.IsNull())
	{
		return false;
	}
	reset();

	return true;
}

bool Image::setImageFromRes(LPCTSTR pResName, int x, int y, int width, int height)
{
	if (!setImageFromRes(pResName))
	{
		return false;
	}
	// �ü�ͼƬ��С
	crop(x, y, width, height);

	return true;
}

void Image::crop(int x, int y, int width, int height)
{
	width = min(max(width, 0), m_Image.GetWidth());
	height = min(max(height, 0), m_Image.GetHeight());
	// ����Դ���ε�λ�úʹ�С�����ڲü���
	m_SrcRect.SetRect(x, y, x + width, y + height);
	// ����Ŀ����Σ������Ƶ����ڵ�λ�úʹ�С��
	setSize(int(width * m_fScaleX), int(height * m_fScaleY));
}

void Image::stretch(int width, int height)
{
	// ����Ŀ����ε�λ�úʹ�С�������Ƶ����ڵ�λ�úʹ�С����������ͼƬ��
	setSize(max(width, 0), max(height, 0));
	// ���ñ�����������
	m_fScaleX = 1;
	m_fScaleY = 1;
}

void Image::setScale(float scaleX, float scaleY)
{
	m_fScaleX = max(scaleX, 0);
	m_fScaleY = max(scaleY, 0);
	setSize(int(m_SrcRect.Width() * scaleX), int(m_SrcRect.Height() * scaleY));
}

void Image::setOpacity(float value)
{
	if (m_Image.GetBPP() == 32)
	{
		m_nAlpha = BYTE(min(max(value, 0), 1) * 255);
	}
}

void Image::setTransparentColor(COLORREF value)
{
	// ����͸��ɫ
	m_Image.SetTransparentColor(value);
}

void Image::reset()
{
	// ����Ŀ����Σ������Ƶ����ڵ�λ�úʹ�С��
	setSize(m_Image.GetWidth(), m_Image.GetHeight());
	// ����Դ���Σ�����ȡͼƬ�Ĵ�С��
	m_SrcRect.SetRect(0, 0, m_Image.GetWidth(), m_Image.GetHeight());
	// ������������
	m_fScaleX = 1;
	m_fScaleY = 1;
	// ����͸����
	m_nAlpha = 255;
}

void Image::saveScreenshot()
{
	tstring savePath;
	// ��ȡ����λ��
	if (FileUtils::getSaveFilePath(savePath, _T("��ͼ���浽"), _T("jpg")))
	{
		// ���洰�ڽ�ͼ
		IMAGE image;
		getimage(&image, 0, 0, App::get()->getWidth(), App::get()->getHeight());
		saveimage(savePath.c_str(), &image);
	}
}


// �� PNG ͼ���������ת��
void CrossImage(CImage &img)
{
	// ��������ת��
	for (int i = 0; i < img.GetWidth(); i++)
	{
		for (int j = 0; j < img.GetHeight(); j++)
		{
			UCHAR *cr = (UCHAR*)img.GetPixelAddress(i, j);
			cr[0] = cr[0] * cr[3] / 255;
			cr[1] = cr[1] * cr[3] / 255;
			cr[2] = cr[2] * cr[3] / 255;
		}
	}
}