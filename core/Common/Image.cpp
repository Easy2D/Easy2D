#include "..\e2dcommon.h"
#include "..\e2dbase.h"
#include <map>

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;
static std::map<int, ID2D1Bitmap*> s_mBitmapsFromResource;
static std::set<ID2D1Bitmap*> s_vBitmaps;


e2d::Image::Image()
	: _bitmap(nullptr)
	, _cropX(0)
	, _cropY(0)
	, _cropWidth(0)
	, _cropHeight(0)
{
}

e2d::Image::Image(const String& filePath)
	: _bitmap(nullptr)
{
	this->open(filePath);
}

e2d::Image::Image(int resNameId, const String& resType)
	: _bitmap(nullptr)
{
	this->open(resNameId, resType);
}

e2d::Image::Image(const String& filePath, double cropX, double cropY, double cropWidth, double cropHeight)
	: _bitmap(nullptr)
{
	this->open(filePath);
	this->crop(cropX, cropY, cropWidth, cropHeight);
}

e2d::Image::Image(int resNameId, const String& resType, double cropX, double cropY, double cropWidth, double cropHeight)
	: _bitmap(nullptr)
{
	this->open(resNameId, resType);
	this->crop(cropX, cropY, cropWidth, cropHeight);
}

e2d::Image::~Image()
{
}

bool e2d::Image::open(const String& filePath)
{
	WARN_IF(filePath.isEmpty(), "Image open failed! Invalid file name.");

	if (filePath.isEmpty())
		return false;

	if (!Image::preload(filePath))
	{
		WARN_IF(true, "Load Image from file failed!");
		return false;
	}

	this->_setBitmap(s_mBitmapsFromFile.at(filePath.getHashCode()));
	return true;
}

bool e2d::Image::open(int resNameId, const String& resType)
{
	if (!Image::preload(resNameId, resType))
	{
		WARN_IF(true, "Load Image from file failed!");
		return false;
	}

	this->_setBitmap(s_mBitmapsFromResource.at(resNameId));
	return true;
}

void e2d::Image::crop(double x, double y, double width, double height)
{
	if (_bitmap)
	{
		_cropX = min(max(x, 0), this->getSourceWidth());
		_cropY = min(max(y, 0), this->getSourceHeight());
		_cropWidth = min(max(width, 0), this->getSourceWidth() - _cropX);
		_cropHeight = min(max(height, 0), this->getSourceHeight() - _cropY);
	}
}

double e2d::Image::getWidth() const
{
	return _cropWidth;
}

double e2d::Image::getHeight() const
{
	return _cropHeight;
}

e2d::Size e2d::Image::getSize() const
{
	return Size(_cropWidth, _cropHeight);
}

double e2d::Image::getSourceWidth() const
{
	if (_bitmap)
	{
		return _bitmap->GetSize().width;
	}
	else
	{
		return 0;
	}
}

double e2d::Image::getSourceHeight() const
{
	if (_bitmap)
	{
		return _bitmap->GetSize().height;
	}
	else
	{
		return 0;
	}
}

e2d::Size e2d::Image::getSourceSize() const
{
	if (_bitmap)
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
	return _cropX;
}

double e2d::Image::getCropY() const
{
	return _cropY;
}

e2d::Point e2d::Image::getCropPos() const
{
	return Point(_cropX, _cropY);
}

bool e2d::Image::preload(const String& filePath)
{
	if (s_mBitmapsFromFile.find(filePath.getHashCode()) != s_mBitmapsFromFile.end())
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
		filePath,
		nullptr,
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
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		// �� WIC λͼ����һ�� Direct2D λͼ
		hr = Renderer::getRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			&pBitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		// ����ͼƬָ���ͼƬ�� Hash ��
		s_mBitmapsFromFile.insert(
			std::map<size_t, ID2D1Bitmap*>::value_type(
				filePath.getHashCode(),
				pBitmap)
		);
	}

	// �ͷ������Դ
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);

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
			nullptr,
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
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		// �� WIC λͼ����һ�� Direct2D λͼ
		hr = Renderer::getRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			&pBitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		s_mBitmapsFromResource.insert(std::pair<int, ID2D1Bitmap*>(resNameId, pBitmap));
	}

	// �ͷ������Դ
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);
	SafeRelease(pScaler);

	return SUCCEEDED(hr);
}


void e2d::Image::clearCache()
{
	for (auto bitmap : s_mBitmapsFromFile)
	{
		SafeRelease(bitmap.second);
	}
	s_mBitmapsFromFile.clear();

	for (auto bitmap : s_mBitmapsFromResource)
	{
		SafeRelease(bitmap.second);
	}
	s_mBitmapsFromResource.clear();

	for (auto bitmap : s_vBitmaps)
	{
		SafeRelease(bitmap);
	}
	s_vBitmaps.clear();
}

void e2d::Image::_setBitmap(ID2D1Bitmap * bitmap)
{
	if (bitmap)
	{
		_bitmap = bitmap;
		_cropX = _cropY = 0;
		_cropWidth = _bitmap->GetSize().width;
		_cropHeight = _bitmap->GetSize().height;
	}
}

ID2D1Bitmap * e2d::Image::getBitmap()
{
	return _bitmap;
}
