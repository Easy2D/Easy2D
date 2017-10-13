#include "winbase.h"
#include "..\emacros.h"

HWND hwnd = nullptr;
ID2D1Factory * pDirect2dFactory = nullptr;
ID2D1HwndRenderTarget * pRenderTarget = nullptr;


HWND &GetHWnd()
{
	return hwnd;
}

ID2D1Factory * &GetFactory()
{
	return pDirect2dFactory;
}

ID2D1HwndRenderTarget * &GetRenderTarget()
{
	return pRenderTarget;
}

void WindowCenter(HWND hWnd)
{
	// ��ȡ��Ļ�ֱ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// ��ȡ���ڴ�С
	tagRECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);
	// ���ô�������Ļ����
	SetWindowPos(
		hWnd, 
		HWND_TOP,
		(screenWidth - (rcWindow.left - rcWindow.right)) / 2,
		(screenHeight - (rcWindow.top - rcWindow.bottom)) / 2,
		(rcWindow.left - rcWindow.right),
		(rcWindow.top - rcWindow.bottom),
		SWP_SHOWWINDOW | SWP_NOSIZE
	);
}
