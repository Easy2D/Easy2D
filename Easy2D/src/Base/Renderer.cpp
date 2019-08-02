#include <e2dbase.h>
#include <e2dmanager.h>
#include <e2dnode.h>

namespace easy2d
{

// 文字渲染器
class TextRenderer
	: public IDWriteTextRenderer
{
private:
	TextRenderer();

	~TextRenderer();

public:
	static TextRenderer* Create(
		ID2D1Factory* pD2DFactory,
		ID2D1HwndRenderTarget* pRT,
		ID2D1SolidColorBrush* pBrush
	);

	STDMETHOD_(void, SetTextStyle)(
		CONST D2D1_COLOR_F& fillColor,
		BOOL hasOutline,
		CONST D2D1_COLOR_F& outlineColor,
		FLOAT outlineWidth,
		D2D1_LINE_JOIN outlineJoin
		);

	STDMETHOD(DrawGlyphRun)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		__in DWRITE_GLYPH_RUN const* glyphRun,
		__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawUnderline)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawStrikethrough)(
		__maybenull void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		__in DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawInlineObject)(
		__maybenull void* clientDrawingContext,
		FLOAT originX,
		FLOAT originY,
		IDWriteInlineObject* inlineObject,
		BOOL isSideways,
		BOOL isRightToLeft,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(IsPixelSnappingDisabled)(
		__maybenull void* clientDrawingContext,
		__out BOOL* isDisabled
		);

	STDMETHOD(GetCurrentTransform)(
		__maybenull void* clientDrawingContext,
		__out DWRITE_MATRIX* transform
		);

	STDMETHOD(GetPixelsPerDip)(
		__maybenull void* clientDrawingContext,
		__out FLOAT* pixelsPerDip
		);

public:
	unsigned long STDMETHODCALLTYPE AddRef();
	unsigned long STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(
		IID const& riid,
		void** ppvObject
	);

private:
	unsigned long cRefCount_;
	D2D1_COLOR_F sFillColor_;
	D2D1_COLOR_F sOutlineColor_;
	FLOAT fOutlineWidth;
	BOOL bShowOutline_;
	ID2D1Factory* pD2DFactory_;
	ID2D1HwndRenderTarget* pRT_;
	ID2D1SolidColorBrush* pBrush_;
	ID2D1StrokeStyle* pCurrStrokeStyle_;
};

TextRenderer::TextRenderer()
	: cRefCount_(0)
	, pD2DFactory_(nullptr)
	, pRT_(nullptr)
	, pBrush_(nullptr)
	, sFillColor_()
	, sOutlineColor_()
	, fOutlineWidth(1)
	, bShowOutline_(TRUE)
	, pCurrStrokeStyle_(nullptr)
{
}

TextRenderer::~TextRenderer()
{
	SafeRelease(pD2DFactory_);
	SafeRelease(pRT_);
	SafeRelease(pBrush_);
}

TextRenderer* TextRenderer::Create(
	ID2D1Factory* pD2DFactory,
	ID2D1HwndRenderTarget* pRT,
	ID2D1SolidColorBrush* pBrush
)
{
	TextRenderer* pTextRenderer = new (std::nothrow) TextRenderer();
	if (pTextRenderer)
	{
		pD2DFactory->AddRef();
		pRT->AddRef();
		pBrush->AddRef();

		pTextRenderer->pD2DFactory_ = pD2DFactory;
		pTextRenderer->pRT_ = pRT;
		pTextRenderer->pBrush_ = pBrush;
	}
	return pTextRenderer;
}

STDMETHODIMP_(void) TextRenderer::SetTextStyle(
	CONST D2D1_COLOR_F& fillColor,
	BOOL hasOutline,
	CONST D2D1_COLOR_F& outlineColor,
	FLOAT outlineWidth,
	D2D1_LINE_JOIN outlineJoin
)
{
	sFillColor_ = fillColor;
	bShowOutline_ = hasOutline;
	sOutlineColor_ = outlineColor;
	fOutlineWidth = 2 * outlineWidth;

	switch (outlineJoin)
	{
	case D2D1_LINE_JOIN_MITER:
		pCurrStrokeStyle_ = easy2d::Renderer::getMiterID2D1StrokeStyle();
		break;
	case D2D1_LINE_JOIN_BEVEL:
		pCurrStrokeStyle_ = easy2d::Renderer::getBevelID2D1StrokeStyle();
		break;
	case D2D1_LINE_JOIN_ROUND:
		pCurrStrokeStyle_ = easy2d::Renderer::getRoundID2D1StrokeStyle();
		break;
	default:
		pCurrStrokeStyle_ = nullptr;
		break;
	}
}

STDMETHODIMP TextRenderer::DrawGlyphRun(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	DWRITE_MEASURING_MODE measuringMode,
	__in DWRITE_GLYPH_RUN const* glyphRun,
	__in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
	IUnknown* clientDrawingEffect
)
{
	HRESULT hr = S_OK;

	ID2D1PathGeometry* pPathGeometry = nullptr;
	hr = pD2DFactory_->CreatePathGeometry(
		&pPathGeometry
	);

	ID2D1GeometrySink* pSink = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = pPathGeometry->Open(
			&pSink
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = glyphRun->fontFace->GetGlyphRunOutline(
			glyphRun->fontEmSize,
			glyphRun->glyphIndices,
			glyphRun->glyphAdvances,
			glyphRun->glyphOffsets,
			glyphRun->glyphCount,
			glyphRun->isSideways,
			glyphRun->bidiLevel % 2,
			pSink
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = pSink->Close();
	}

	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
	);

	ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pPathGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	if (SUCCEEDED(hr) && bShowOutline_)
	{
		pBrush_->SetColor(sOutlineColor_);

		pRT_->DrawGeometry(
			pTransformedGeometry,
			pBrush_,
			fOutlineWidth,
			pCurrStrokeStyle_
		);
	}

	if (SUCCEEDED(hr))
	{
		pBrush_->SetColor(sFillColor_);

		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush_
		);
	}

	SafeRelease(pPathGeometry);
	SafeRelease(pSink);
	SafeRelease(pTransformedGeometry);

	return hr;
}

STDMETHODIMP TextRenderer::DrawUnderline(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	__in DWRITE_UNDERLINE const* underline,
	IUnknown* clientDrawingEffect
)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		underline->offset,
		underline->width,
		underline->offset + underline->thickness
	);

	ID2D1RectangleGeometry * pRectangleGeometry = nullptr;
	hr = pD2DFactory_->CreateRectangleGeometry(
		&rect,
		&pRectangleGeometry
	);

	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
	);

	ID2D1TransformedGeometry * pTransformedGeometry = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pRectangleGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	if (SUCCEEDED(hr) && bShowOutline_)
	{
		pBrush_->SetColor(sOutlineColor_);

		pRT_->DrawGeometry(
			pTransformedGeometry,
			pBrush_,
			fOutlineWidth,
			pCurrStrokeStyle_
		);
	}

	if (SUCCEEDED(hr))
	{
		pBrush_->SetColor(sFillColor_);

		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush_
		);
	}

	SafeRelease(pRectangleGeometry);
	SafeRelease(pTransformedGeometry);

	return S_OK;
}

STDMETHODIMP TextRenderer::DrawStrikethrough(
	__maybenull void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	__in DWRITE_STRIKETHROUGH const* strikethrough,
	IUnknown* clientDrawingEffect
)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		strikethrough->offset,
		strikethrough->width,
		strikethrough->offset + strikethrough->thickness
	);

	ID2D1RectangleGeometry * pRectangleGeometry = nullptr;
	hr = pD2DFactory_->CreateRectangleGeometry(
		&rect,
		&pRectangleGeometry
	);

	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		baselineOriginX, baselineOriginY
	);

	ID2D1TransformedGeometry * pTransformedGeometry = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = pD2DFactory_->CreateTransformedGeometry(
			pRectangleGeometry,
			&matrix,
			&pTransformedGeometry
		);
	}

	if (SUCCEEDED(hr) && bShowOutline_)
	{
		pBrush_->SetColor(sOutlineColor_);

		pRT_->DrawGeometry(
			pTransformedGeometry,
			pBrush_,
			fOutlineWidth,
			pCurrStrokeStyle_
		);
	}

	if (SUCCEEDED(hr))
	{
		pBrush_->SetColor(sFillColor_);

		pRT_->FillGeometry(
			pTransformedGeometry,
			pBrush_
		);
	}

	SafeRelease(pRectangleGeometry);
	SafeRelease(pTransformedGeometry);

	return S_OK;
}

STDMETHODIMP TextRenderer::DrawInlineObject(
	__maybenull void* clientDrawingContext,
	FLOAT originX,
	FLOAT originY,
	IDWriteInlineObject* inlineObject,
	BOOL isSideways,
	BOOL isRightToLeft,
	IUnknown* clientDrawingEffect
)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(unsigned long) TextRenderer::AddRef()
{
	return InterlockedIncrement(&cRefCount_);
}

STDMETHODIMP_(unsigned long) TextRenderer::Release()
{
	unsigned long newCount = InterlockedDecrement(&cRefCount_);

	if (newCount == 0)
	{
		delete this;
		return 0;
	}

	return newCount;
}

STDMETHODIMP TextRenderer::IsPixelSnappingDisabled(
	__maybenull void* clientDrawingContext,
	__out BOOL* isDisabled
)
{
	*isDisabled = FALSE;
	return S_OK;
}

STDMETHODIMP TextRenderer::GetCurrentTransform(
	__maybenull void* clientDrawingContext,
	__out DWRITE_MATRIX* transform
)
{
	pRT_->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}

STDMETHODIMP TextRenderer::GetPixelsPerDip(
	__maybenull void* clientDrawingContext,
	__out FLOAT* pixelsPerDip
)
{
	float x, yUnused;

	pRT_->GetDpi(&x, &yUnused);
	*pixelsPerDip = x / 96;

	return S_OK;
}

STDMETHODIMP TextRenderer::QueryInterface(
	IID const& riid,
	void** ppvObject
)
{
	if (__uuidof(IDWriteTextRenderer) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IDWritePixelSnapping) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IUnknown) == riid)
	{
		*ppvObject = this;
	}
	else
	{
		*ppvObject = nullptr;
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}

}


namespace
{
	bool s_bShowFps = false;
	float s_fDpiScaleX = 0;
	float s_fDpiScaleY = 0;
	IDWriteTextFormat* s_pTextFormat = nullptr;
	ID2D1Factory* s_pDirect2dFactory = nullptr;
	ID2D1HwndRenderTarget* s_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* s_pSolidBrush = nullptr;
	IWICImagingFactory* s_pIWICFactory = nullptr;
	IDWriteFactory* s_pDWriteFactory = nullptr;
	easy2d::TextRenderer* s_pTextRenderer = nullptr;
	ID2D1StrokeStyle* s_pMiterStrokeStyle = nullptr;
	ID2D1StrokeStyle* s_pBevelStrokeStyle = nullptr;
	ID2D1StrokeStyle* s_pRoundStrokeStyle = nullptr;
	D2D1_COLOR_F s_nClearColor = D2D1::ColorF(D2D1::ColorF::Black);
}

bool easy2d::Renderer::__createDeviceIndependentResources()
{
	__discardResources();

	// 创建设备无关资源，它们的生命周期和程序的时长相同
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&s_pDirect2dFactory
	);
	E2D_ERROR_IF_FAILED(hr, L"Create ID2D1Factory failed");

	if (SUCCEEDED(hr))
	{
		hr = s_pDirect2dFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&s_pMiterStrokeStyle
		);
		E2D_ERROR_IF_FAILED(hr, L"Create ID2D1StrokeStyle failed");
	}

	if (SUCCEEDED(hr))
	{
		hr = s_pDirect2dFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_BEVEL,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&s_pBevelStrokeStyle
		);
		E2D_ERROR_IF_FAILED(hr, L"Create ID2D1StrokeStyle failed");
	}

	if (SUCCEEDED(hr))
	{
		hr = s_pDirect2dFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_ROUND,
				2.0f,
				D2D1_DASH_STYLE_SOLID,
				0.0f),
			nullptr,
			0,
			&s_pRoundStrokeStyle
		);
		E2D_ERROR_IF_FAILED(hr, L"Create ID2D1StrokeStyle failed");
	}

	if (SUCCEEDED(hr))
	{
		// 创建 WIC 绘图工厂，用于统一处理各种格式的图片
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&s_pIWICFactory)
		);
		E2D_ERROR_IF_FAILED(hr, L"Create IWICImagingFactory failed");
	}

	if (SUCCEEDED(hr))
	{
		// 创建 DirectWrite 工厂
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown * *>(&s_pDWriteFactory)
		);
		E2D_ERROR_IF_FAILED(hr, L"Create IDWriteFactory failed");
	}

	if (SUCCEEDED(hr))
	{
		// 创建文本格式化对象
		hr = s_pDWriteFactory->CreateTextFormat(
			L"",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"",
			&s_pTextFormat
		);
		E2D_ERROR_IF_FAILED(hr, L"Create IDWriteTextFormat failed");
	}

	if (SUCCEEDED(hr))
	{
		s_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}

	return SUCCEEDED(hr);
}

bool easy2d::Renderer::__createDeviceResources()
{
	HRESULT hr = S_OK;

	if (!s_pRenderTarget)
	{
		HWND hWnd = Window::getHWnd();

		HDC hdc = ::GetDC(hWnd);
		s_fDpiScaleX = (float)::GetDeviceCaps(hdc, LOGPIXELSX);
		s_fDpiScaleY = (float)::GetDeviceCaps(hdc, LOGPIXELSY);

		// 创建设备相关资源。这些资源应在 Direct3D 设备消失时重建，
		// 比如当 isVisiable 被修改，等等
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// 创建一个 Direct2D 渲染目标
		hr = s_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				size),
			&s_pRenderTarget
		);
		E2D_ERROR_IF_FAILED(hr, L"Create ID2D1HwndRenderTarget failed");

		if (SUCCEEDED(hr))
		{
			// 创建画刷
			hr = s_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&s_pSolidBrush
			);
			E2D_ERROR_IF_FAILED(hr, L"Create ID2D1SolidColorBrush failed");
		}

		if (SUCCEEDED(hr))
		{
			// 创建自定义的文字渲染器
			s_pTextRenderer = TextRenderer::Create(
				s_pDirect2dFactory,
				s_pRenderTarget,
				s_pSolidBrush
			);
		}
	}

	return SUCCEEDED(hr);
}

void easy2d::Renderer::__discardDeviceResources()
{
	SafeRelease(s_pRenderTarget);
	SafeRelease(s_pSolidBrush);
	SafeRelease(s_pTextRenderer);
	SafeRelease(s_pMiterStrokeStyle);
	SafeRelease(s_pBevelStrokeStyle);
	SafeRelease(s_pRoundStrokeStyle);
}

void easy2d::Renderer::__discardResources()
{
	__discardDeviceResources();
	SafeRelease(s_pTextFormat);
	SafeRelease(s_pDirect2dFactory);
	SafeRelease(s_pIWICFactory);
	SafeRelease(s_pDWriteFactory);
}

void easy2d::Renderer::__render()
{
	HRESULT hr = S_OK;

	// 创建设备相关资源
	Renderer::__createDeviceResources();

	// 开始渲染
	s_pRenderTarget->BeginDraw();
	// 使用背景色清空屏幕
	s_pRenderTarget->Clear(s_nClearColor);

	// 渲染场景
	SceneManager::__render();

	// 渲染 FPS
	if (s_bShowFps && s_pTextFormat)
	{
		static int s_nRenderTimes = 0;
		static float s_fLastRenderTime = 0;
		static String s_sFpsText;

		++s_nRenderTimes;

		float fDelay = Time::getTotalTime() - s_fLastRenderTime;
		if (fDelay >= 0.3)
		{
			wchar_t fpsText[20] = { 0 };
			::swprintf_s(fpsText, L"FPS: %.1lf", (1 / fDelay) * s_nRenderTimes);
			s_sFpsText = fpsText;
			s_fLastRenderTime = Time::getTotalTime();
			s_nRenderTimes = 0;
		}

		IDWriteTextLayout * pTextLayout = nullptr;

		hr = s_pDWriteFactory->CreateTextLayout(
			s_sFpsText.c_str(),
			(UINT32)s_sFpsText.length(),
			s_pTextFormat,
			0,
			0,
			&pTextLayout
		);

		if (SUCCEEDED(hr))
		{
			s_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			s_pSolidBrush->SetOpacity(1.0f);
			s_pTextRenderer->SetTextStyle(
				D2D1::ColorF(D2D1::ColorF::White),
				TRUE,
				D2D1::ColorF(D2D1::ColorF::Black, 0.4f),
				1.5f,
				D2D1_LINE_JOIN_ROUND
			);

			pTextLayout->Draw(nullptr, s_pTextRenderer, 10, 0);

			SafeRelease(pTextLayout);
		}
	}

	// 终止渲染
	hr = s_pRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
		// 并在下一次调用时重建资源
		hr = S_OK;
		Renderer::__discardDeviceResources();
	}

	if (FAILED(hr))
	{
		E2D_ERROR(L"Device loss recovery failed");
	}
}


easy2d::Color easy2d::Renderer::getBackgroundColor()
{
	return Color(s_nClearColor.r, s_nClearColor.g, s_nClearColor.b, s_nClearColor.a);
}

void easy2d::Renderer::setBackgroundColor(Color color)
{
	s_nClearColor = color.toD2DColorF();
}

void easy2d::Renderer::showFps(bool show)
{
	s_bShowFps = show;
}

float easy2d::Renderer::getDpiScaleX()
{
	return s_fDpiScaleX;
}

float easy2d::Renderer::getDpiScaleY()
{
	return s_fDpiScaleY;
}

ID2D1Factory * easy2d::Renderer::getID2D1Factory()
{
	return s_pDirect2dFactory;
}

ID2D1HwndRenderTarget * easy2d::Renderer::getRenderTarget()
{
	return s_pRenderTarget;
}

ID2D1SolidColorBrush * easy2d::Renderer::getSolidColorBrush()
{
	return s_pSolidBrush;
}

IWICImagingFactory * easy2d::Renderer::getIWICImagingFactory()
{
	return s_pIWICFactory;
}

IDWriteFactory * easy2d::Renderer::getIDWriteFactory()
{
	return s_pDWriteFactory;
}

void easy2d::Renderer::SetTextStyle(const Color& fillColor, bool hasOutline, const Color& outlineColor, float outlineWidth, LineJoin outlineJoin)
{
	s_pTextRenderer->SetTextStyle(
		fillColor.toD2DColorF(),
		hasOutline,
		outlineColor.toD2DColorF(),
		outlineWidth,
		D2D1_LINE_JOIN(outlineJoin)
	);
}

void easy2d::Renderer::DrawTextLayout(IDWriteTextLayout* layout)
{
	layout->Draw(nullptr, s_pTextRenderer, 0, 0);
}

ID2D1StrokeStyle * easy2d::Renderer::getMiterID2D1StrokeStyle()
{
	return s_pMiterStrokeStyle;
}

ID2D1StrokeStyle * easy2d::Renderer::getBevelID2D1StrokeStyle()
{
	return s_pBevelStrokeStyle;
}

ID2D1StrokeStyle * easy2d::Renderer::getRoundID2D1StrokeStyle()
{
	return s_pRoundStrokeStyle;
}
