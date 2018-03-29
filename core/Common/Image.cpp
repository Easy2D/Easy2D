#include "..\enodes.h"
#include <map>

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;


e2d::Image::Image()
	: m_pBitmap(nullptr)
	, m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
{
}

e2d::Image::Image(String strFileName)
{
	this->open(strFileName);
}

e2d::Image::Image(String strFileName, double nClipX, double nClipY, double nClipWidth, double nClipHeight)
{
	this->open(strFileName);
	this->clip(nClipX, nClipY, nClipWidth, nClipHeight);
}

e2d::Image::~Image()
{
}

void e2d::Image::open(String strFilePath)
{
	WARN_IF(strFilePath.isEmpty(), "Image cannot load bitmap from NULL file name.");

	if (strFilePath.isEmpty())
		return;

	if (!Image::preload(strFilePath))
	{
		WARN_IF(true, "Load Image from file failed!");
		return;
	}

	m_pBitmap = s_mBitmapsFromFile.at(strFilePath.getHashCode());
	m_fSourceClipX = m_fSourceClipY = 0;
	m_fSourceClipWidth = m_pBitmap->GetSize().width;
	m_fSourceClipHeight = m_pBitmap->GetSize().height;
}

void e2d::Image::clip(double x, double y, double width, double height)
{
	if (m_pBitmap)
	{
		m_fSourceClipX = min(max(x, 0), this->getSourceWidth());
		m_fSourceClipY = min(max(y, 0), this->getSourceHeight());
		m_fSourceClipWidth = min(max(width, 0), this->getSourceWidth() - m_fSourceClipX);
		m_fSourceClipHeight = min(max(height, 0), this->getSourceHeight() - m_fSourceClipY);
	}
}

double e2d::Image::getWidth() const
{
	return m_fSourceClipWidth;
}

double e2d::Image::getHeight() const
{
	return m_fSourceClipHeight;
}

e2d::Size e2d::Image::getSize() const
{
	return Size(m_fSourceClipWidth, m_fSourceClipHeight);
}

double e2d::Image::getSourceWidth() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->GetSize().width;
	}
	else
	{
		return 0;
	}
}

double e2d::Image::getSourceHeight() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->GetSize().height;
	}
	else
	{
		return 0;
	}
}

e2d::Size e2d::Image::getSourceSize() const
{
	if (m_pBitmap)
	{
		return Size(getSourceWidth(), getSourceHeight());
	}
	else
	{
		return Size();
	}
}

double e2d::Image::getClipX() const
{
	return m_fSourceClipX;
}

double e2d::Image::getClipY() const
{
	return m_fSourceClipY;
}

e2d::Point e2d::Image::getClipPos() const
{
	return Point(m_fSourceClipX, m_fSourceClipY);
}

bool e2d::Image::preload(String fileName)
{
	if (s_mBitmapsFromFile.find(fileName.getHashCode()) != s_mBitmapsFromFile.end())
	{
		return true;
	}

	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	// ����������
	hr = Renderer::getIWICImagingFactory()->CreateDecoderFromFilename(
		fileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// ������ʼ�����
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// ����ͼƬ��ʽת����
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = Renderer::getIWICImagingFactory()->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		// ͼƬ��ʽת���� 32bbpPBGRA
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}
	if (SUCCEEDED(hr))
	{
		// �� WIC λͼ����һ�� Direct2D λͼ
		hr = Renderer::getRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}
	if (SUCCEEDED(hr))
	{
		// ����ͼƬָ���ͼƬ�� Hash ��
		s_mBitmapsFromFile.insert(
			std::map<size_t, ID2D1Bitmap*>::value_type(
				fileName.getHashCode(),
				pBitmap)
		);
	}

	// �ͷ������Դ
	SafeReleaseInterface(&pDecoder);
	SafeReleaseInterface(&pSource);
	SafeReleaseInterface(&pStream);
	SafeReleaseInterface(&pConverter);

	return SUCCEEDED(hr);
}

void e2d::Image::clearCache()
{
	for (auto child : s_mBitmapsFromFile)
	{
		SafeReleaseInterface(&child.second);
	}
	s_mBitmapsFromFile.clear();
}

ID2D1Bitmap * e2d::Image::getBitmap()
{
	return m_pBitmap;
}
