#include "..\enodes.h"
#include <map>


struct ResKey
{
	ResKey() { resNameHash = 0; resTypeHash = 0; }
	
	bool operator < (ResKey const& key) const
	{ 
		if (resNameHash > key.resNameHash)
			return true;
		else if (resNameHash == key.resNameHash)
			return resTypeHash > key.resTypeHash;
		else 
			return false;
	}

	size_t resNameHash;
	size_t resTypeHash;
};

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;
static std::map<ResKey, ID2D1Bitmap*> s_mBitmapsFromResource;

static ID2D1Bitmap * GetBitmapFromFile(const e2d::EString & fileName);
static ID2D1Bitmap * GetBitmapFromResource(const e2d::EString & resourceName, const e2d::EString & resourceType);


e2d::ESprite::ESprite()
{
}

e2d::ESprite::ESprite(const EString & imageFileName)
	: ESprite()
{
	setImage(imageFileName);
}

e2d::ESprite::ESprite(const EString & resourceName, const EString & resourceType)
	: ESprite()
{
	setImage(resourceName, resourceType);
}

void e2d::ESprite::setImage(const EString & fileName)
{
	WARN_IF(fileName.empty(), "ESprite cannot load bitmap from NULL file name.");

	if (fileName.empty() || m_sFileName == fileName)
		return;

	m_sFileName = fileName;

	SafeReleaseInterface(&pBitmap);
	pBitmap = GetBitmapFromFile(m_sFileName);

	ASSERT(pBitmap, "ESprite create device resources failed!");

	this->setSize(pBitmap->GetSize().width, pBitmap->GetSize().height);
}

void e2d::ESprite::setImage(const EString & resourceName, const EString & resourceType)
{
	WARN_IF(resourceName.empty() || resourceType.empty(), "ESprite cannot load bitmap from NULL resource.");

	if (resourceName.empty() || resourceType.empty())
		return;

	if (m_sResourceName == resourceName && m_sResourceType == resourceType)
		return;

	if (!m_sFileName.empty())
		m_sFileName.clear();

	m_sResourceName = resourceName;
	m_sResourceType = resourceType;

	SafeReleaseInterface(&pBitmap);
	pBitmap = GetBitmapFromResource(resourceName, resourceType);

	ASSERT(pBitmap, "ESprite create device resources failed!");

	this->setSize(pBitmap->GetSize().width, pBitmap->GetSize().height);
}

void e2d::ESprite::_onRender()
{
	if (pBitmap)
	{
		// Draw bitmap
		GetRenderTarget()->DrawBitmap(
			pBitmap,
			D2D1::RectF(0, 0, getRealWidth(), getRealHeight()),
			m_fDisplayOpacity
		);
	}
}

bool e2d::ESprite::preloadImage(const EString & fileName)
{
	std::hash<e2d::EString> h;
	size_t hash = h(fileName);

	if (s_mBitmapsFromFile.find(hash) != s_mBitmapsFromFile.end())
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
	hr = GetImagingFactory()->CreateDecoderFromFilename(
		fileName.c_str(),
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
		hr = GetImagingFactory()->CreateFormatConverter(&pConverter);
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
		hr = GetRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}
	if (SUCCEEDED(hr))
	{
		// ����ͼƬָ���ͼƬ�� Hash ��
		std::hash<e2d::EString> h;
		size_t hash = h(fileName);

		s_mBitmapsFromFile.insert(
			std::map<size_t, ID2D1Bitmap*>::value_type(
				hash,
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

bool e2d::ESprite::preloadImage(const EString & resourceName, const EString & resourceType)
{
	std::hash<e2d::EString> h;

	ResKey key;
	key.resNameHash = h(resourceName);
	key.resTypeHash = h(resourceType);

	if (s_mBitmapsFromResource.find(key) != s_mBitmapsFromResource.end())
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
	imageResHandle = ::FindResourceW(HINST_THISCOMPONENT, resourceName.c_str(), resourceType.c_str());

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
		hr = GetImagingFactory()->CreateStream(&pStream);
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
		hr = GetImagingFactory()->CreateDecoderFromStream(
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
		hr = GetImagingFactory()->CreateFormatConverter(&pConverter);
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
		hr = GetRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		std::hash<e2d::EString> h;

		ResKey key;
		key.resNameHash = h(resourceName);
		key.resTypeHash = h(resourceType);

		s_mBitmapsFromResource.insert(
			std::map<ResKey, ID2D1Bitmap*>::value_type(
				key,
				pBitmap)
		);
	}

	// �ͷ������Դ
	SafeReleaseInterface(&pDecoder);
	SafeReleaseInterface(&pSource);
	SafeReleaseInterface(&pStream);
	SafeReleaseInterface(&pConverter);
	SafeReleaseInterface(&pScaler);

	return SUCCEEDED(hr);
}

void e2d::ESprite::clearCache()
{
	for (auto child : s_mBitmapsFromFile)
	{
		SafeReleaseInterface(&child.second);
	}
	for (auto child : s_mBitmapsFromResource)
	{
		SafeReleaseInterface(&child.second);
	}
	s_mBitmapsFromFile.clear();
	s_mBitmapsFromResource.clear();
}




ID2D1Bitmap * GetBitmapFromFile(const e2d::EString & fileName)
{
	if (!e2d::ESprite::preloadImage(fileName))
	{
		return nullptr;
	}

	std::hash<e2d::EString> h;
	size_t hash = h(fileName);

	return s_mBitmapsFromFile.at(hash);
}

ID2D1Bitmap * GetBitmapFromResource(const e2d::EString & resourceName, const e2d::EString & resourceType)
{
	if (!e2d::ESprite::preloadImage(resourceName, resourceType))
	{
		return nullptr;
	}

	ResKey key;
	std::hash<e2d::EString> h;
	key.resNameHash = h(resourceName);
	key.resTypeHash = h(resourceType);

	return s_mBitmapsFromResource.at(key);
}