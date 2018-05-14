#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

static bool s_bShowFps = false;
static float s_fDpiScaleX = 0;
static float s_fDpiScaleY = 0;
static IDWriteTextFormat * s_pTextFormat = nullptr;
static ID2D1Factory * s_pDirect2dFactory = nullptr;
static ID2D1HwndRenderTarget * s_pRenderTarget = nullptr;
static ID2D1SolidColorBrush * s_pSolidBrush = nullptr;
static IWICImagingFactory * s_pIWICFactory = nullptr;
static IDWriteFactory * s_pDWriteFactory = nullptr;
static e2d::CustomTextRenderer * s_pTextRenderer = nullptr;
static D2D1_COLOR_F s_nClearColor = D2D1::ColorF(D2D1::ColorF::Black);


bool e2d::Renderer::__createDeviceIndependentResources()
{
	// �����豸�޹���Դ�����ǵ��������ںͳ����ʱ����ͬ
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&s_pDirect2dFactory
	);

	ASSERT(SUCCEEDED(hr), "Create ID2D1Factory Failed!");

	if (SUCCEEDED(hr))
	{
		// ���� WIC ��ͼ����������ͳһ������ָ�ʽ��ͼƬ
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&s_pIWICFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create IWICImagingFactory Failed!");
	}

	if (SUCCEEDED(hr))
	{
		// ���� DirectWrite ����
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&s_pDWriteFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create IDWriteFactory Failed!");
	}

	if (SUCCEEDED(hr))
	{
		// ���������ص�ǰ��ϵͳ DPI�����ֵҲ��������������
		Renderer::getID2D1Factory()->GetDesktopDpi(&s_fDpiScaleX, &s_fDpiScaleY);
	}

	if (SUCCEEDED(hr))
	{
		// �����ı���ʽ������
		hr = s_pDWriteFactory->CreateTextFormat(
			L"",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"",
			&s_pTextFormat
		);

		if (SUCCEEDED(hr))
		{
			s_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}

	return SUCCEEDED(hr);
}

bool e2d::Renderer::__createDeviceResources()
{
	HRESULT hr = S_OK;

	if (!s_pRenderTarget)
	{
		HWND hWnd = Window::getHWnd();

		// �����豸�����Դ����Щ��ԴӦ�� Direct3D �豸��ʧʱ�ؽ���
		// ���統 isVisiable ���޸ģ��ȵ�
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// ����һ�� Direct2D ��ȾĿ��
		hr = s_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				size),
			&s_pRenderTarget
		);

		ASSERT(SUCCEEDED(hr), "Create ID2D1HwndRenderTarget Failed!");

		if (SUCCEEDED(hr))
		{
			// ������ˢ
			hr = s_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&s_pSolidBrush
			);
			ASSERT(SUCCEEDED(hr), "Create ID2D1SolidColorBrush Failed!");
		}

		if (SUCCEEDED(hr))
		{
			// �����Զ����������Ⱦ��
			s_pTextRenderer = new (std::nothrow) CustomTextRenderer(
				s_pDirect2dFactory,
				s_pRenderTarget,
				s_pSolidBrush
			);
		}
	}

	return SUCCEEDED(hr);
}

void e2d::Renderer::__discardDeviceResources()
{
	SafeReleaseInterface(s_pRenderTarget);
	SafeReleaseInterface(s_pSolidBrush);
	SafeReleaseInterface(s_pTextRenderer);
}

void e2d::Renderer::__discardResources()
{
	SafeReleaseInterface(s_pTextFormat);
	SafeReleaseInterface(s_pDirect2dFactory);
	SafeReleaseInterface(s_pRenderTarget);
	SafeReleaseInterface(s_pSolidBrush);
	SafeReleaseInterface(s_pTextRenderer);
	SafeReleaseInterface(s_pIWICFactory);
	SafeReleaseInterface(s_pDWriteFactory);
}

void e2d::Renderer::__render()
{
	HRESULT hr = S_OK;

	// �����豸�����Դ
	Renderer::__createDeviceResources();

	// ��ʼ��Ⱦ
	s_pRenderTarget->BeginDraw();
	// ʹ�ñ���ɫ�����Ļ
	s_pRenderTarget->Clear(s_nClearColor);

	// ��Ⱦ����
	SceneManager::__render();

	// ��Ⱦ FPS
	if (s_bShowFps)
	{
		static int s_nRenderTimes = 0;
		static double s_fLastRenderTime = 0;
		static String s_sFpsText;

		++s_nRenderTimes;

		double fDelay = Time::getTotalTime() - s_fLastRenderTime;
		if (fDelay >= 0.3)
		{
			s_sFpsText = String::format(L"FPS: %.1lf", (1 / fDelay) * s_nRenderTimes);
			s_fLastRenderTime = Time::getTotalTime();
			s_nRenderTimes = 0;
		}

		IDWriteTextLayout * pTextLayout = nullptr;

		hr = s_pDWriteFactory->CreateTextLayout(
			s_sFpsText,
			(UINT32)s_sFpsText.getLength(),
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

			pTextLayout->Draw(NULL, s_pTextRenderer, 10, 0);

			SafeReleaseInterface(pTextLayout);
		}
	}

	// ��ֹ��Ⱦ
	hr = s_pRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// ��� Direct3D �豸��ִ�й�������ʧ����������ǰ���豸�����Դ
		// ������һ�ε���ʱ�ؽ���Դ
		hr = S_OK;
		Renderer::__discardDeviceResources();
	}

	if (FAILED(hr))
	{
		Window::error(L"Device loss recovery failed. Exiting game.");
		Game::quit();
	}
}


e2d::Color e2d::Renderer::getBackgroundColor()
{
	return Color(s_nClearColor.r, s_nClearColor.g, s_nClearColor.b, s_nClearColor.a);
}

void e2d::Renderer::setBackgroundColor(Color color)
{
	s_nClearColor = color.toColorF();
}

void e2d::Renderer::showFps(bool show)
{
	s_bShowFps = show;
}

float e2d::Renderer::getDpiScaleX()
{
	return s_fDpiScaleX;
}

float e2d::Renderer::getDpiScaleY()
{
	return s_fDpiScaleY;
}

ID2D1Factory * e2d::Renderer::getID2D1Factory()
{
	return s_pDirect2dFactory;
}

ID2D1HwndRenderTarget * e2d::Renderer::getRenderTarget()
{
	return s_pRenderTarget;
}

ID2D1SolidColorBrush * e2d::Renderer::getSolidColorBrush()
{
	return s_pSolidBrush;
}

IWICImagingFactory * e2d::Renderer::getIWICImagingFactory()
{
	return s_pIWICFactory;
}

IDWriteFactory * e2d::Renderer::getIDWriteFactory()
{
	return s_pDWriteFactory;
}

e2d::CustomTextRenderer * e2d::Renderer::getCustomTextRenderer()
{
	return s_pTextRenderer;
}
