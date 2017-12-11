#include "..\etools.h"
#include "..\Win\winbase.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);

typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(
	const GUID& rfid,
	DWORD dwFlags,
	HANDLE hToken,
	PWSTR *ppszPath);

e2d::EString e2d::EFileUtils::getLocalAppDataPath()
{
	// ��ȡ AppData\Local �ļ��е�·��
	PWSTR pszPath = NULL;
	HMODULE hModule = LoadLibrary(L"shell32.dll");
	pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pszPath);

	if (SUCCEEDED(hr))
	{
		EString path = pszPath;
		CoTaskMemFree(pszPath);
		return path;
	}
	return L"";
}

e2d::EString e2d::EFileUtils::getTempPath()
{
	// ��ȡ��ʱ�ļ�Ŀ¼
	wchar_t path[_MAX_PATH];
	::GetTempPath(_MAX_PATH, path);

	// ������ʱ�ļ�Ŀ¼
	e2d::EString tempFilePath = path + e2d::EApp::getAppName();
	if (_waccess(tempFilePath, 0) == -1)
	{
		_wmkdir(tempFilePath);
	}
	return tempFilePath;
}

e2d::EString e2d::EFileUtils::getDefaultSavePath()
{
	EString path = EFileUtils::getLocalAppDataPath();
	WARN_IF(path.isEmpty(), "Cannot get local AppData path!");

	path += L"\\" + EApp::getAppName();

	if (_waccess(path, 0) == -1)
	{
		_wmkdir(path);
	}

	path += L"\\DefaultData.ini";

	return path;
}

void e2d::EFileUtils::saveInt(const EString & key, int value)
{
	::WritePrivateProfileString(L"Default", key, EString::parse(value), getDefaultSavePath());
}

void e2d::EFileUtils::saveFloat(const EString & key, float value)
{
	::WritePrivateProfileString(L"Default", key, EString::parse(value), getDefaultSavePath());
}

void e2d::EFileUtils::saveString(const EString & key, const EString & value)
{
	::WritePrivateProfileString(L"Default", key, value, getDefaultSavePath());
}

int e2d::EFileUtils::getInt(const EString & key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key, default, getDefaultSavePath());
}

float e2d::EFileUtils::getFloat(const EString & key, float default)
{
	wchar_t temp[32] = { 0 };
	::GetPrivateProfileString(L"Default", key, EString::parse(default), temp, 31, getDefaultSavePath());
	return std::stof(temp);
}

e2d::EString e2d::EFileUtils::getString(const EString & key, const EString & default)
{
	wchar_t temp[256] = { 0 };
	::GetPrivateProfileString(L"Default", key, default, temp, 255, getDefaultSavePath());
	return temp;
}

e2d::EString e2d::EFileUtils::getFileExtension(const EString & filePath)
{
	EString fileExtension;
	// �ҵ��ļ����е����һ�� '.' ��λ��
	size_t pos = filePath.findLastOf(L'.');
	// �ж� pos �Ƿ��Ǹ���Чλ��
	if (pos != -1)
	{
		// ��ȡ��չ��
		fileExtension = filePath.sub(pos, filePath.length());
		// ת��ΪСд��ĸ
		fileExtension = fileExtension.lower();
	}

	return fileExtension;
}

e2d::EString e2d::EFileUtils::getSaveFilePath(const EString & title, const EString & defExt)
{
	// ��������Ի���
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// ���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);				// �ṹ���С
	ofn.hwndOwner = GetHWnd();							// ���ھ��
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
