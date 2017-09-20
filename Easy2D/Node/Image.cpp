#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"

// �� PNG ͼ���������ת��
static void CrossImage(CImage &img);

Image::Image() :
	m_fScaleX(1),
	m_fScaleY(1)
{
}

Image::Image(LPCTSTR ImageFile) :
	m_fScaleX(1),
	m_fScaleY(1)
{
	setImage(ImageFile);	// ����ͼƬ��Դ
}

Image::Image(LPCTSTR ImageFile, int x, int y, int width, int height) :
	m_fScaleX(1),
	m_fScaleY(1)
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
	m_Image.Draw(GetImageHDC(), m_rDest, m_rSrc);
}

int Image::getWidth() const
{
	return m_rDest.Width();		// Ŀ����εĿ��
}

int Image::getHeight() const
{
	return m_rDest.Height();	// Ŀ����εĸ߶�
}

float Image::getScaleX() const
{
	return m_fScaleX;
}

float Image::getScaleY() const
{
	return m_fScaleY;
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
	// ��ȡ��չ������ PNG ͼƬ�������⴦��
	if (_T(".png") == FileUtils::getFileExtension(ImageFile))
	{
		// ����ת��
		CrossImage(m_Image);
		// Alpha ͨ��
		m_Image.AlphaBlend(GetImageHDC(), 15, 30);
	}
	// ����Ŀ����Σ������Ƶ����ڵ�λ�úʹ�С��
	m_rDest.SetRect(m_nX, m_nY, m_nX + m_Image.GetWidth(), m_nY + m_Image.GetHeight());
	m_rSrc.SetRect(0, 0, m_Image.GetWidth(), m_Image.GetHeight());
	// ������������
	m_fScaleX = 1;
	m_fScaleY = 1;

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
	// ����Ŀ����Σ������Ƶ����ڵ�λ�úʹ�С��
	m_rDest.SetRect(m_nX, m_nY, m_nX + m_Image.GetWidth(), m_nY + m_Image.GetHeight());
	m_rSrc.SetRect(0, 0, m_Image.GetWidth(), m_Image.GetHeight());
	// ������������
	m_fScaleX = 1;
	m_fScaleY = 1;

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
	m_rSrc.SetRect(x, y, x + width, y + height);
	// ����Ŀ����Σ������Ƶ����ڵ�λ�úʹ�С��
	m_rDest.SetRect(m_nX, m_nY, m_nX + int(width * m_fScaleX), m_nY + int(height * m_fScaleY));
}

void Image::stretch(int width, int height)
{
	width = max(width, 0);
	height = max(height, 0);
	// ����Ŀ����ε�λ�úʹ�С�������Ƶ����ڵ�λ�úʹ�С����������ͼƬ��
	m_rDest.SetRect(m_nX, m_nY, m_nX + width, m_nY + height);
	// ���ñ�����������
	m_fScaleX = 1;
	m_fScaleY = 1;
}

void Image::scale(float scaleX, float scaleY)
{
	m_fScaleX = max(scaleX, 0);
	m_fScaleY = max(scaleY, 0);
	m_rDest.SetRect(
		m_nX, m_nY, 
		m_nX + int(m_rSrc.Width() * scaleX),
		m_nY + int(m_rSrc.Height() * scaleY));
}

void Image::setPos(int x, int y)
{
	// �ƶ�Ŀ�����
	m_rDest.MoveToXY(x, y);
	m_nX = x;
	m_nY = y;
}

void Image::move(int x, int y)
{
	// �ƶ�Ŀ�����
	m_rDest.OffsetRect(x, y);
	m_nX += x;
	m_nY += y;
}

void Image::setX(int x)
{
	// �ƶ�Ŀ�����
	m_rDest.MoveToX(x);
	m_nX = x;
}

void Image::setY(int y)
{
	// �ƶ�Ŀ�����
	m_rDest.MoveToY(y);
	m_nY = y;
}

void Image::setTransparentColor(COLORREF value)
{
	// ����͸��ɫ
	m_Image.SetTransparentColor(value);
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