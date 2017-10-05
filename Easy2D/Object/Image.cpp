#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <map>
using namespace std;

// ͼƬ����
static map<TString, CImage*> s_mCImages;
// ��ͼƬ�����ж�ȡͼƬ
static CImage* GetCImage(TString name, bool fromRes = false);
// �� PNG ͼ���������ת��
static void CrossImage(CImage &img);


Image::Image() :
	m_pCImage(nullptr),
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
	if (!m_bDisplay || !m_pCImage)
	{
		return;
	}
	// ����ͼƬ
	if (m_pCImage->GetBPP() == 32)
	{
		m_pCImage->AlphaBlend(GetImageHDC(), m_Rect, m_SrcRect, m_nAlpha, AC_SRC_OVER);
	}
	else
	{
		m_pCImage->Draw(GetImageHDC(), m_Rect, m_SrcRect);
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
	m_pCImage = GetCImage(ImageFile);
	if (m_pCImage)
	{
		reset();
		return true;
	}
	return false;
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
	m_pCImage = GetCImage(pResName, true);
	if (m_pCImage)
	{
		reset();
		return true;
	}
	return false;
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
	width = min(max(width, 0), m_pCImage->GetWidth() - x);
	height = min(max(height, 0), m_pCImage->GetHeight() - y);
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
	if (m_pCImage->GetBPP() == 32)
	{
		m_nAlpha = BYTE(min(max(value, 0), 1) * 255);
	}
}

void Image::setTransparentColor(COLORREF value)
{
	// ����͸��ɫ
	m_pCImage->SetTransparentColor(value);
}

void Image::reset()
{
	// ����Ŀ����Σ������Ƶ����ڵ�λ�úʹ�С��
	setSize(m_pCImage->GetWidth(), m_pCImage->GetHeight());
	// ����Դ���Σ�����ȡͼƬ�Ĵ�С��
	m_SrcRect.SetRect(0, 0, m_pCImage->GetWidth(), m_pCImage->GetHeight());
	// ������������
	m_fScaleX = 1;
	m_fScaleY = 1;
	// ����͸����
	m_nAlpha = 255;
}

bool Image::preload(LPCTSTR fileName, bool fromRes)
{
	// �ж�ͼƬ�Ƿ��Ѿ�����
	if (s_mCImages.find(fileName) != s_mCImages.end())
	{
		return true;
	}
	// ����ͼƬ
	CImage* cImage = nullptr;
	if (fromRes)
	{
		cImage = new CImage();
		// ����Դ����ͼƬ����֧�� PNG��
		cImage->LoadFromResource(GetModuleHandle(NULL), fileName);
	}
	else
	{
		//�ж�ͼƬ·���Ƿ����
		if (!PathFileExists(fileName))
		{
			return false;
		}
		cImage = new CImage();
		cImage->Load(fileName);
	}
	// ����ʧ��
	if (!cImage || cImage->IsNull())
	{
		return false;
	}
	// ȷ�ϸ�ͼ����� Alpha ͨ��
	if (cImage->GetBPP() == 32)
	{
		// ͸��ͼƬ����
		CrossImage(*cImage);
	}
	s_mCImages.insert(map<TString, CImage*>::value_type(fileName, cImage));

	return true;
}

void Image::saveScreenshot()
{
	TString savePath;
	// ��ȡ����λ��
	if (FileUtils::getSaveFilePath(savePath, _T("��ͼ���浽"), _T("jpg")))
	{
		// ���洰�ڽ�ͼ
		IMAGE image;
		getimage(&image, 0, 0, App::getWidth(), App::getHeight());
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

CImage* GetCImage(TString name, bool fromRes)
{
	if (Image::preload(name.c_str()))
	{
		return s_mCImages.at(name);
	}
	else
	{
		return nullptr;
	}
}