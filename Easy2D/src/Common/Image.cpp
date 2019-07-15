#include <e2dcommon.h>
#include <e2dbase.h>
#include <e2dtool.h>
#include <map>

namespace
{
	std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;
	std::map<int, ID2D1Bitmap*> s_mBitmapsFromResource;
}

easy2d::Image::Image()
	: _bitmap(nullptr)
	, _cropRect()
{
}

easy2d::Image::Image(const String& filePath)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(filePath);
}

easy2d::Image::Image(int resNameId, const String& resType)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(resNameId, resType);
}

easy2d::Image::Image(const String& filePath, const Rect& cropRect)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(filePath);
	this->crop(cropRect);
}

easy2d::Image::Image(int resNameId, const String& resType, const Rect& cropRect)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(resNameId, resType);
	this->crop(cropRect);
}

easy2d::Image::~Image()
{
}

bool easy2d::Image::open(const String& filePath)
{
	if (filePath.empty()) E2D_WARNING(L"Image open failed! Invalid file name.");

	if (filePath.empty())
		return false;

	if (!Image::preload(filePath))
	{
		E2D_WARNING(L"Load Image from file failed!");
		return false;
	}

	this->_setBitmap(s_mBitmapsFromFile.at(std::hash<String>{}(filePath)));
	return true;
}

bool easy2d::Image::open(int resNameId, const String& resType)
{
	if (!Image::preload(resNameId, resType))
	{
		E2D_WARNING(L"Load Image from file failed!");
		return false;
	}

	this->_setBitmap(s_mBitmapsFromResource.at(resNameId));
	return true;
}

void easy2d::Image::crop(const Rect& cropRect)
{
	if (_bitmap)
	{
		_cropRect.origin.x = min(max(cropRect.origin.x, 0), this->getSourceWidth());
		_cropRect.origin.y = min(max(cropRect.origin.y, 0), this->getSourceHeight());
		_cropRect.size.width = min(max(cropRect.size.width, 0), this->getSourceWidth() - cropRect.origin.x);
		_cropRect.size.height = min(max(cropRect.size.height, 0), this->getSourceHeight() - cropRect.origin.y);
	}
}

float easy2d::Image::getWidth() const
{
	return _cropRect.size.width;
}

float easy2d::Image::getHeight() const
{
	return _cropRect.size.height;
}

easy2d::Size easy2d::Image::getSize() const
{
	return _cropRect.size;
}

float easy2d::Image::getSourceWidth() const
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

float easy2d::Image::getSourceHeight() const
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

easy2d::Size easy2d::Image::getSourceSize() const
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

float easy2d::Image::getCropX() const
{
	return _cropRect.origin.x;
}

float easy2d::Image::getCropY() const
{
	return _cropRect.origin.y;
}

easy2d::Point easy2d::Image::getCropPos() const
{
	return _cropRect.origin;
}

bool easy2d::Image::preload(const String& filePath)
{
	if (s_mBitmapsFromFile.find(std::hash<String>{}(filePath)) != s_mBitmapsFromFile.end())
	{
		return true;
	}

	String actualFilePath = Path::searchForFile(filePath);
	if (actualFilePath.empty())
	{
		return false;
	}

	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	// ����������
	hr = Renderer::getIWICImagingFactory()->CreateDecoderFromFilename(
		actualFilePath.c_str(),
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
				std::hash<String>{}(filePath),
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

bool easy2d::Image::preload(int resNameId, const String& resType)
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
	imageResHandle = ::FindResourceW(HINST_THISCOMPONENT, MAKEINTRESOURCE(resNameId), resType.c_str());

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
		imageFileSize = ::SizeofResource(HINST_THISCOMPONENT, imageResHandle);

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


void easy2d::Image::clearCache()
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
}

void easy2d::Image::_setBitmap(ID2D1Bitmap * bitmap)
{
	if (bitmap)
	{
		_bitmap = bitmap;
		_cropRect.origin.x = _cropRect.origin.y = 0;
		_cropRect.size.width = _bitmap->GetSize().width;
		_cropRect.size.height = _bitmap->GetSize().height;
	}
}

ID2D1Bitmap * easy2d::Image::getBitmap()
{
	return _bitmap;
}
