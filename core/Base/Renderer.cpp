#include "..\ebase.h"
#include "..\emanager.h"
#include "..\enode.h"

static bool s_bShowFps = false;
static IDWriteTextFormat * s_pTextFormat = nullptr;
static ID2D1Factory * s_pDirect2dFactory = nullptr;
static ID2D1HwndRenderTarget * s_pRenderTarget = nullptr;
static ID2D1SolidColorBrush * s_pSolidBrush = nullptr;
static IWICImagingFactory * s_pIWICFactory = nullptr;
static IDWriteFactory * s_pDWriteFactory = nullptr;
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
		// �����ı���ʽ������
		hr = s_pDWriteFactory->CreateTextFormat(
			L"",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			18,
			L"",
			&s_pTextFormat
		);

		if (s_pTextFormat)
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
	}
	
	return SUCCEEDED(hr);
}

void e2d::Renderer::__discardDeviceResources()
{
	SafeReleaseInterface(&s_pRenderTarget);
	SafeReleaseInterface(&s_pSolidBrush);
}

void e2d::Renderer::__discardResources()
{
	SafeReleaseInterface(&s_pTextFormat);
	SafeReleaseInterface(&s_pDirect2dFactory);
	SafeReleaseInterface(&s_pRenderTarget);
	SafeReleaseInterface(&s_pSolidBrush);
	SafeReleaseInterface(&s_pIWICFactory);
	SafeReleaseInterface(&s_pDWriteFactory);
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
		static e2d::String s_sFpsText = L"";

		s_nRenderTimes++;

		double fDelay = Time::getTotalTime() - s_fLastRenderTime;
		if (fDelay >= 0.3)
		{
			s_sFpsText = String::format(L"FPS: %.1lf", (1 / fDelay) * s_nRenderTimes);
			s_fLastRenderTime = Time::getTotalTime();
			s_nRenderTimes = 0;
		}

		s_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		s_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		s_pRenderTarget->DrawTextW(s_sFpsText, (UINT32)s_sFpsText.getLength(), s_pTextFormat, D2D1::RectF(), s_pSolidBrush);
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
		// ��Ⱦʱ������δ֪�Ĵ����˳���Ϸ
		ASSERT(false, L"Renderer error: %#X!", hr);
		Game::quit();
	}
}


void e2d::Renderer::setBackgroundColor(UINT32 color)
{
	s_nClearColor = D2D1::ColorF(color);
}

void e2d::Renderer::showFps(bool show)
{
	s_bShowFps = show;
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
