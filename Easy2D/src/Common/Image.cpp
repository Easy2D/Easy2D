#include <easy2d/e2dcommon.h>
#include <easy2d/e2dbase.h>
#include <easy2d/e2dtool.h>
#include <unordered_map>

namespace
{
	std::unordered_map<easy2d::String, easy2d::Image*> s_mImagesFromFile;
	std::unordered_map<easy2d::Resource, easy2d::Image*> s_mImagesFromResource;
}

namespace easy2d
{
	HRESULT LoadBitmapFromFile(ID2D1Bitmap** ppBitmap, const String& filePath);
	HRESULT LoadBitmapFromResource(ID2D1Bitmap** ppBitmap, const Resource& res);
}

easy2d::Image::Image(ID2D1Bitmap* bitmap)
	: _bitmap(bitmap)
{
	if (_bitmap)
	{
		_bitmap->AddRef();
	}
}

easy2d::Image::~Image()
{
	SafeRelease(_bitmap);
}

ID2D1Bitmap* easy2d::Image::getBitmap()
{
	return _bitmap;
}

void easy2d::Image::resetBitmap(ID2D1Bitmap* bitmap)
{
	SafeRelease(_bitmap);
	_bitmap = bitmap;
	if (_bitmap)
	{
		_bitmap->AddRef();
	}
}

float easy2d::Image::getWidth() const
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

float easy2d::Image::getHeight() const
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

easy2d::Size easy2d::Image::getSize() const
{
	if (_bitmap)
	{
		return Size(getWidth(), getHeight());
	}
	else
	{
		return Size();
	}
}

easy2d::Image* easy2d::Image::load(const String& filePath)
{
	auto iter = s_mImagesFromFile.find(filePath);
	if (iter != s_mImagesFromFile.end())
	{
		return iter->second;
	}

	ID2D1Bitmap* pBitmap = nullptr;
	HRESULT hr = LoadBitmapFromFile(&pBitmap, filePath);

	Image* image = nullptr;
	if (SUCCEEDED(hr))
	{
		image = gcnew Image(pBitmap);
		image->retain();
		s_mImagesFromFile.insert(std::make_pair(filePath, image));
	}

	SafeRelease(pBitmap);
	if (SUCCEEDED(hr))
	{
		return image;
	}

	E2D_ERROR("Load image failed! ERROR_CODE = %#X", hr);
	return nullptr;
}

easy2d::Image* easy2d::Image::load(const Resource& res)
{
	auto iter = s_mImagesFromResource.find(res);
	if (iter != s_mImagesFromResource.end())
	{
		return iter->second;
	}

	ID2D1Bitmap* pBitmap = nullptr;
	HRESULT hr = LoadBitmapFromResource(&pBitmap, res);

	Image* image = nullptr;
	if (SUCCEEDED(hr))
	{
		image = gcnew Image(pBitmap);
		image->retain();
		s_mImagesFromResource.insert(std::make_pair(res, image));
	}

	SafeRelease(pBitmap);
	if (SUCCEEDED(hr))
	{
		return image;
	}

	E2D_ERROR("Load image failed! ERROR_CODE = %#X", hr);
	return nullptr;
}

easy2d::Image* easy2d::Image::load(int resNameId, const String& resType)
{
	return load(Resource{ resNameId, resType });
}

namespace
{
}

void easy2d::Image::clearCache()
{
	for (auto pair : s_mImagesFromFile)
	{
		GC::release(pair.second);
	}
	s_mImagesFromFile.clear();

	for (auto pair : s_mImagesFromResource)
	{
		GC::release(pair.second);
	}
	s_mImagesFromResource.clear();
}

void easy2d::Image::reloadCache()
{
	for (auto pair : s_mImagesFromFile)
	{
		ID2D1Bitmap* pBitmap = nullptr;
		HRESULT hr = LoadBitmapFromFile(&pBitmap, pair.first);

		if (SUCCEEDED(hr))
		{
			pair.second->resetBitmap(pBitmap);
		}
		else
		{
			E2D_ERROR("Reload image failed! ERROR_CODE = %#X", hr);
		}
		SafeRelease(pBitmap);
	}
	for (auto pair : s_mImagesFromResource)
	{
		ID2D1Bitmap* pBitmap = nullptr;
		HRESULT hr = LoadBitmapFromResource(&pBitmap, pair.first);

		if (SUCCEEDED(hr))
		{
			pair.second->resetBitmap(pBitmap);
		}
		else
		{
			E2D_ERROR("Reload image failed! ERROR_CODE = %#X", hr);
		}
		SafeRelease(pBitmap);
	}
}

namespace easy2d
{

HRESULT LoadBitmapFromFile(ID2D1Bitmap** ppBitmap, const easy2d::String& filePath)
{
	String actualFilePath = Path::searchForFile(filePath);
	if (actualFilePath.empty())
	{
		return S_FALSE;
	}

	WideString wFilePath = NarrowToWide(filePath);

	HRESULT hr = S_OK;

	// 创建解码器
	IWICBitmapDecoder* pDecoder = nullptr;
	hr = Renderer::getIWICImagingFactory()->CreateDecoderFromFilename(
		wFilePath.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	IWICBitmapFrameDecode* pSource = nullptr;
	if (SUCCEEDED(hr))
	{
		// 创建初始化框架
		hr = pDecoder->GetFrame(0, &pSource);
	}

	IWICFormatConverter* pConverter = nullptr;
	if (SUCCEEDED(hr))
	{
		// 创建图片格式转换器
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = Renderer::getIWICImagingFactory()->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bbpPBGRA
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
		hr = Renderer::getRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			ppBitmap
		);
	}

	// 释放相关资源
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pConverter);
	return hr;
}

HRESULT LoadBitmapFromResource(ID2D1Bitmap** ppBitmap, const easy2d::Resource& res)
{
	auto resData = res.loadData();
	HRESULT hr = resData.isValid() ? S_OK : E_FAIL;

	IWICStream* pStream = nullptr;
	if (SUCCEEDED(hr))
	{
		// 创建 WIC 流
		hr = Renderer::getIWICImagingFactory()->CreateStream(&pStream);
	}

	if (SUCCEEDED(hr))
	{
		// 初始化流
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(resData.buffer),
			static_cast<DWORD>(resData.size)
		);
	}

	IWICBitmapDecoder* pDecoder = nullptr;
	if (SUCCEEDED(hr))
	{
		// 创建流的解码器
		hr = Renderer::getIWICImagingFactory()->CreateDecoderFromStream(
			pStream,
			nullptr,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	IWICBitmapFrameDecode* pSource = nullptr;
	if (SUCCEEDED(hr))
	{
		// 创建初始化框架
		hr = pDecoder->GetFrame(0, &pSource);
	}

	IWICFormatConverter* pConverter = nullptr;
	if (SUCCEEDED(hr))
	{
		// 创建图片格式转换器
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = Renderer::getIWICImagingFactory()->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bppPBGRA
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
		hr = Renderer::getRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			ppBitmap
		);
	}

	// 释放相关资源
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);
	return hr;
}

}
