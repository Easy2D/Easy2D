#include "..\etool.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


e2d::String e2d::Path::getLocalAppDataPath()
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

e2d::String e2d::Path::getTempPath()
{
	// ��ȡ��ʱ�ļ�Ŀ¼
	wchar_t path[_MAX_PATH];
	if (0 == ::GetTempPath(_MAX_PATH, path))
	{
		return L"";
	}

	// ������ʱ�ļ�Ŀ¼
	e2d::String tempFilePath;
	tempFilePath << path << L"Easy2DGameTemp\\";
	// �����ļ���
	if (!Path::createFolder(tempFilePath))
	{
		return path;
	}

	// ��ȡ AppName
	String sAppName = Game::getAppName();
	if (!sAppName.isEmpty())
	{
		// �����ļ���
		if (!Path::createFolder(tempFilePath + sAppName + L"\\"))
		{
			return std::move(tempFilePath);
		}
		tempFilePath << sAppName << L"\\";
	}
	return std::move(tempFilePath);
}

e2d::String e2d::Path::getDefaultSavePath()
{
	// ��ȡ AppData ·��
	String path = Path::getLocalAppDataPath();

	if (path.isEmpty())
	{
		WARN_IF(true, "Cannot get local AppData path!");
		return std::move(path);
	}
	
	// �����ļ���
	if (!Path::createFolder(path + L"\\Easy2DGameData"))
	{
		return std::move(path);
	}
	path << L"\\Easy2DGameData";

	// ��ȡ AppName
	String sAppName = Game::getAppName();
	if (!sAppName.isEmpty())
	{
		// �����ļ���
		if (!Path::createFolder(path + L"\\" + sAppName))
		{
			return std::move(path);
		}
		path << L"\\" << sAppName;
	}
	path << L"\\";
	
	return std::move(path);
}

e2d::String e2d::Path::getFileExtension(String filePath)
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

e2d::String e2d::Path::getSaveFilePath(const String title, const String defExt)
{
	// ��������Ի���
	OPENFILENAME ofn = { 0 };
	wchar_t strFilename[MAX_PATH] = { 0 };				// ���ڽ����ļ���
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

bool e2d::Path::createFolder(String strDirPath)
{
	if (strDirPath.isEmpty())
	{
		WARN_IF(true, "Path::createFolder Failed: Invalid directory path!");
		return false;
	}

	if (-1 == ::_waccess(strDirPath, 0))
	{
		if (0 != ::_wmkdir(strDirPath))
		{
			return false;
		}
	}
	return true;
}
