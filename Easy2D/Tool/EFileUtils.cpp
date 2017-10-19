#include "..\etools.h"
#include "..\Win\winbase.h"
#include <sstream>
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

e2d::EString e2d::EFileUtils::getDefaultSavePath()
{
	EString path = EFileUtils::getLocalAppDataPath();
	WARN_IF(path.empty(), "Cannot get local AppData path!");

	path.append(L"\\");
	path.append(EApp::getAppName());

	if (_waccess(path.c_str(), 0) == -1)
	{
		_wmkdir(path.c_str());
	}

	path.append(L"\\DefaultData.ini");

	return path;
}

void e2d::EFileUtils::saveInt(LPCTSTR key, int value)
{
	std::wstringstream ss;
	ss << value;
	::WritePrivateProfileString(L"Default", key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void e2d::EFileUtils::saveDouble(LPCTSTR key, double value)
{
	std::wstringstream ss;
	ss << value;
	::WritePrivateProfileString(L"Default", key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void e2d::EFileUtils::saveString(LPCTSTR key, EString value)
{
	::WritePrivateProfileString(L"Default", key, value.c_str(), getDefaultSavePath().c_str());
}

int e2d::EFileUtils::getInt(LPCTSTR key, int default)
{
	return ::GetPrivateProfileInt(L"Default", key, default, getDefaultSavePath().c_str());
}

double e2d::EFileUtils::getDouble(LPCTSTR key, double default)
{
	// �� default ����ת��Ϊ�ַ���
	std::wstringstream ss;
	ss << default;
	// ��ȡ����
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(L"Default", key, ss.str().c_str(), temp, 128, getDefaultSavePath().c_str());
	// ת��Ϊ�ַ�����
	ss.str(L"");
	ss << temp;
	// ���ַ���ת��Ϊ double
	return _wtof(ss.str().c_str());
}

e2d::EString e2d::EFileUtils::geTString(LPCTSTR key, EString default)
{
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(L"Default", key, default.c_str(), temp, 128, getDefaultSavePath().c_str());
	return EString(temp);
}

e2d::EString e2d::EFileUtils::getFileExtension(const EString & filePath)
{
	EString fileExtension;
	// �ҵ��ļ����е����һ�� '.' ��λ��
	size_t pos = filePath.find_last_of('.');
	// �ж� pos �Ƿ��Ǹ���Чλ��
	if (pos != EString::npos)
	{
		// ��ȡ��չ��
		fileExtension = filePath.substr(pos, filePath.length());
		// ת��ΪСд��ĸ
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
	}

	return fileExtension;
}

e2d::EString e2d::EFileUtils::getSaveFilePath(LPCTSTR title, LPCTSTR defExt)
{
	// ��������Ի���
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// ���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);				// �ṹ���С
	ofn.hwndOwner = GetHWnd();							// ӵ���Ŵ��ھ����NULL ��ʾ�Ի����Ƿ�ģ̬��
	ofn.lpstrFilter = L"�����ļ�\0*.*\0\0";			// ���ù���
	ofn.nFilterIndex = 1;								// ����������
	ofn.lpstrFile = strFilename;						// ���շ��ص��ļ�·�����ļ���
	ofn.nMaxFile = sizeof(strFilename);					// ����������
	ofn.lpstrInitialDir = NULL;							// ��ʼĿ¼ΪĬ��
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;// Ŀ¼������ڣ������ļ�ǰ��������
	ofn.lpstrTitle = title;								// ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrDefExt = defExt;							// Ĭ��׷�ӵ���չ��

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}
