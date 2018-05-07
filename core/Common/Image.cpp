#include "..\e2dnode.h"
#include <map>

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;
static std::map<int, ID2D1Bitmap*> s_mBitmapsFromResource;


e2d::Image::Image()
	: m_pBitmap(nullptr)
	, m_fSourceCropX(0)
	, m_fSourceCropY(0)
	, m_fSourceCropWidth(0)
	, m_fSourceCropHeight(0)
{
}

e2d::Image::Image(const String& filePath)
	: m_pBitmap(nullptr)
{
	this->open(filePath);
}

e2d::Image::Image(int resNameId, const String& resType)
	: m_pBitmap(nullptr)
{
	this->open(resNameId, resType);
}

e2d::Image::Image(const String& filePath, double cropX, double cropY, double cropWidth, double cropHeight)
	: m_pBitmap(nullptr)
{
	this->open(filePath);
	this->crop(cropX, cropY, cropWidth, cropHeight);
}

e2d::Image::Image(int resNameId, const String& resType, double cropX, double cropY, double cropWidth, double cropHeight)
	: m_pBitmap(nullptr)
{
	this->open(resNameId, resType);
	this->crop(cropX, cropY, cropWidth, cropHeight);
}

e2d::Image::~Image()
{
}

bool e2d::Image::open(const String& filePath)
{
	WARN_IF(filePath.isEmpty(), "Image cannot load bitmap from NULL file name.");

	if (filePath.isEmpty())
		return false;

	if (!Image::preload(filePath))
	{
		WARN_IF(true, "Load Image from file failed!");
		return false;
	}

	m_pBitmap = s_mBitmapsFromFile.at(filePath.getHashCode());
	m_fSourceCropX = m_fSourceCropY = 0;
	m_fSourceCropWidth = m_pBitmap->GetSize().width;
	m_fSourceCropHeight = m_pBitmap->GetSize().height;
	return true;
}

bool e2d::Image::open(int resNameId, const String& resType)
{
	if (!Image::preload(resNameId, resType))
	{
		WARN_IF(true, "Load Image from file failed!");
		return false;
	}

	m_pBitmap = s_mBitmapsFromResource.at(resNameId);
	m_fSourceCropX = m_fSourceCropY = 0;
	m_fSourceCropWidth = m_pBitmap->GetSize().width;
	m_fSourceCropHeight = m_pBitmap->GetSize().height;
	return true;
}

void e2d::Image::crop(double x, double y, double width, double height)
{
	if (m_pBitmap)
	{
		m_fSourceCropX = min(max(x, 0), this->getSourceWidth());
		m_fSourceCropY = min(max(y, 0), this->getSourceHeight());
		m_fSourceCropWidth = min(max(width, 0), this->getSourceWidth() - m_fSourceCropX);
		m_fSourceCropHeight = min(max(height, 0), this->getSourceHeight() - m_fSourceCropY);
	}
}

double e2d::Image::getWidth() const
{
	return m_fSourceCropWidth;
}

double e2d::Image::getHeight() const
{
	return m_fSourceCropHeight;
}

e2d::Size e2d::Image::getSize() const
{
	return Size(m_fSourceCropWidth, m_fSourceCropHeight);
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

double e2d::Image::getCropX() const
{
	return m_fSourceCropX;
}

double e2d::Image::getCropY() const
{
	return m_fSourceCropY;
}

e2d::Point e2d::Image::getCropPos() const
{
	return Point(m_fSourceCropX, m_fSourceCropY);
}

bool e2d::Image::preload(const String& fileName)
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

bool e2d::Image::preload(int resNameId, const String& resType)
{
	if (s_mBitmapsFromResource.find(resNameId) != s_mBitmapsFromResource.end())
	{
		return true;
	}

	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	IWICBitmapScaler *pScaler = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	HRSRC imageResHandle = nullptr;
	HGLOBAL imageResDataHandle = nullptr;
	void *pImageFile = nullptr;
	DWORD imageFileSize = 0;

	// ��λ��Դ
	imageResHandle = ::FindResourceW(HINST_THISCOMPONENT, MAKEINTRESOURCE(resNameId), resType);

	hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// ������Դ
		imageResDataHandle = ::LoadResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// ��ȡ�ļ�ָ�룬��������Դ
		pImageFile = ::LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// �����С
		imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// ���� WIC ��
		hr = Renderer::getIWICImagingFactory()->CreateStream(&pStream);
	}

	if (SUCCEEDED(hr))
	{
		// ��ʼ����
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	if (SUCCEEDED(hr))
	{
		// �������Ľ�����
		hr = Renderer::getIWICImagingFactory()->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

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
		// ͼƬ��ʽת���� 32bppPBGRA
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
		s_mBitmapsFromResource.insert(std::pair<int, ID2D1Bitmap*>(resNameId, pBitmap));
	}

	// �ͷ������Դ
	SafeReleaseInterface(&pDecoder);
	SafeReleaseInterface(&pSource);
	SafeReleaseInterface(&pStream);
	SafeReleaseInterface(&pConverter);
	SafeReleaseInterface(&pScaler);

	return SUCCEEDED(hr);
}


void e2d::Image::clearCache()
{
	for (auto bitmap : s_mBitmapsFromFile)
	{
		SafeReleaseInterface(&bitmap.second);
	}
	s_mBitmapsFromFile.clear();

	for (auto bitmap : s_mBitmapsFromResource)
	{
		SafeReleaseInterface(&bitmap.second);
	}
	s_mBitmapsFromResource.clear();
}

ID2D1Bitmap * e2d::Image::getBitmap()
{
	return m_pBitmap;
}
