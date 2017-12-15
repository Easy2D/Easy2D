#include "winbase.h"


static HWND s_HWnd = nullptr;
static ID2D1Factory * s_pDirect2dFactory = nullptr;
static ID2D1HwndRenderTarget * s_pRenderTarget = nullptr;
static ID2D1SolidColorBrush * s_pSolidBrush = nullptr;
static IWICImagingFactory * s_pIWICFactory = nullptr;
static IDWriteFactory * s_pDWriteFactory = nullptr;
static LARGE_INTEGER s_tNow;
static LARGE_INTEGER s_tFreq;


HWND &GetHWnd()
{
	return s_HWnd;
}

ID2D1Factory * &GetFactory()
{
	if (!s_pDirect2dFactory)
	{
		// �����豸�޹���Դ�����ǵ��������ںͳ����ʱ����ͬ
		HRESULT hr = S_OK;

		// ����һ�� Direct2D ����
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &s_pDirect2dFactory);

		ASSERT(SUCCEEDED(hr), "Create Device Independent Resources Failed!");
	}
	return s_pDirect2dFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	if (!s_pRenderTarget)
	{
		// �����豸�����Դ����Щ��ԴӦ�� Direct3D �豸��ʧʱ�ؽ���
		// ���統 isVisiable ���޸ģ��ȵ�
		RECT rc;
		GetClientRect(GetHWnd(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// ����һ�� Direct2D ��ȾĿ��
		HRESULT hr;
		hr = GetFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				GetHWnd(), 
				size),
			&s_pRenderTarget
		);
		
		ASSERT(SUCCEEDED(hr), "Create Render Target Failed! Maybe you should initalize EApp first.");
	}
	return s_pRenderTarget;
}

IWICImagingFactory * &GetImagingFactory()
{
	if (!s_pIWICFactory)
	{
		// ���� WIC ��ͼ����������ͳһ������ָ�ʽ��ͼƬ
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&s_pIWICFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create WICImagingFactory Failed!");
	}
	return s_pIWICFactory;
}

IDWriteFactory * &GetDirectWriteFactory()
{
	if (!s_pDWriteFactory)
	{
		// ���� DirectWrite ����
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&s_pDWriteFactory)
		);
		ASSERT(SUCCEEDED(hr), "Create DirectWrite Factory Failed!");
	}
	return s_pDWriteFactory;
}

ID2D1SolidColorBrush * &GetSolidColorBrush()
{
	if (!s_pSolidBrush)
	{
		HRESULT hr;
		hr = GetRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White), 
			&s_pSolidBrush
		);
		ASSERT(SUCCEEDED(hr), "Create Solid Color Brush Failed!");
	}
	return s_pSolidBrush;
}

LARGE_INTEGER &GetFreq()
{
	if (s_tFreq.QuadPart == 0)
	{
		QueryPerformanceFrequency(&s_tFreq);	// ��ȡʱ��Ƶ��
	}
	return s_tFreq;
}

LARGE_INTEGER &GetNow()
{
	return s_tNow;
}

bool IsIntervalFull(const LARGE_INTEGER& tLast, const LARGE_INTEGER& tInterval)
{
	return (s_tNow.QuadPart - tLast.QuadPart) >= tInterval.QuadPart;
}

LONGLONG ToMilliseconds(LONGLONG tLast)
{
	return tLast * 1000LL / GetFreq().QuadPart;
}

void SetInterval(LARGE_INTEGER& nInterval, LONGLONG nIntervalMS)
{
	nInterval.QuadPart = static_cast<LONGLONG>(GetFreq().QuadPart * nIntervalMS / 1000.0);
}
