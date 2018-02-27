#include "..\etools.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


e2d::String e2d::File::getLocalAppDataPath()
{
	typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);

	// ��ȡ AppData\Local �ļ��е�·��
	PWSTR pszPath = NULL;
	HMODULE hModule = LoadLibrary(L"shell32.dll");
	pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pszPath);

	if (SUCCEEDED(hr))
	{
		String path = pszPath;
		CoTaskMemFree(pszPath);
		return path;
	}
	return L"";
}

e2d::String e2d::File::getTempPath()
{
	// ��ȡ��ʱ�ļ�Ŀ¼
	wchar_t path[_MAX_PATH];
	::GetTempPath(_MAX_PATH, path);

	// ������ʱ�ļ�Ŀ¼
	e2d::String tempFilePath = path + e2d::Game::getAppName();
	if (::_waccess(tempFilePath, 0) == -1)
	{
		::_wmkdir(tempFilePath);
	}
	return tempFilePath;
}

e2d::String e2d::File::getDefaultSavePath()
{
	String path = File::getLocalAppDataPath();
	WARN_IF(path.isEmpty(), "Cannot get local AppData path!");

	path += L"\\" + Game::getAppName();

	if (::_waccess(path, 0) == -1)
	{
		::_wmkdir(path);
	}

	path += L"\\DefaultData.ini";

	return path;
}

e2d::String e2d::File::getFileExtension(const String & filePath)
{
	String fileExtension;
	// �ҵ��ļ����е����һ�� '.' ��λ��
	int pos = filePath.findLastOf(L'.');
	// �ж� pos �Ƿ��Ǹ���Чλ��
	if (pos != -1)
	{
		// ��ȡ��չ��
		fileExtension = filePath.subtract(pos);
		// ת��ΪСд��ĸ
		fileExtension = fileExtension.toLower();
	}

	return fileExtension;
}

e2d::String e2d::File::getSaveFilePath(const String & title, const String & defExt)
{
	// ��������Ի���
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// ���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);				// �ṹ���С
	ofn.hwndOwner = Window::getHWnd();					// ���ھ��
	ofn.lpstrFilter = L"�����ļ�\0*.*\0\0";				// ���ù���
	ofn.nFilterIndex = 1;								// ����������
	ofn.lpstrFile = strFilename;						// ���շ��ص��ļ�·�����ļ���
	ofn.nMaxFile = sizeof(strFilename);					// ����������
	ofn.lpstrInitialDir = NULL;							// ��ʼĿ¼ΪĬ��
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = title;								// ����
	ofn.lpstrDefExt = defExt;							// Ĭ��׷�ӵ���չ��

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}