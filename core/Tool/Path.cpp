#include "..\e2dtool.h"
#include <algorithm>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


static e2d::String s_sLocalAppDataPath;
static e2d::String s_sTempPath;
static e2d::String s_sDefaultSavePath;

bool e2d::Path::__init()
{
	// ��ȡ AppData\Local �ļ��е�·��
	typedef HRESULT(WINAPI* pFunSHGetKnownFolderPath)(const GUID& rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);

	PWSTR pszPath = nullptr;
	HMODULE hModule = LoadLibrary(L"shell32.dll");
	pFunSHGetKnownFolderPath SHGetKnownFolderPath = (pFunSHGetKnownFolderPath)GetProcAddress(hModule, "SHGetKnownFolderPath");
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pszPath);

	if (SUCCEEDED(hr))
	{
		s_sLocalAppDataPath = pszPath;
		CoTaskMemFree(pszPath);
	}
	else
	{
		WARN("Cannot get local AppData path!");
	}

	// ��ȡ��Ϸ����
	String sGameName = Game::getName();
	
	// ��ȡĬ�ϱ���·��
	bool bInitSavePath = false;
	do
	{
		String localAppPath = s_sLocalAppDataPath;
		if (localAppPath.isEmpty())
		{

			break;
		}
		else
		{
			s_sDefaultSavePath = localAppPath;
		}

		localAppPath << L"\\Easy2DGameData";
		if (Path::createFolder(localAppPath))
		{
			s_sDefaultSavePath = localAppPath;
		}
		else
		{
			break;
		}

		if (!sGameName.isEmpty())
		{
			localAppPath << L"\\" << sGameName;
			// �����ļ���
			if (Path::createFolder(localAppPath))
			{
				s_sDefaultSavePath = localAppPath;
			}
		}

		s_sDefaultSavePath << L"\\";
		bInitSavePath = true;
	} while (0);

	// ��ȡ��ʱ�ļ�Ŀ¼
	bool bInitTempPath = false;
	do
	{
		wchar_t path[_MAX_PATH];
		if (0 == ::GetTempPath(_MAX_PATH, path))
		{
			break;
		}
		else
		{
			s_sTempPath = path;
		}

		// ������ʱ�ļ�Ŀ¼
		String tempPath;
		tempPath << s_sTempPath << L"\\Easy2DGameTemp";
		// �����ļ���
		if (Path::createFolder(tempPath))
		{
			s_sTempPath = path;
		}
		else
		{
			break;
		}

		if (!sGameName.isEmpty())
		{
			tempPath << L"\\" << sGameName;
			// �����ļ���
			if (Path::createFolder(tempPath))
			{
				s_sTempPath = tempPath;
			}
		}

		s_sTempPath << L"\\";
		bInitTempPath = true;
	} while (0);

	return SUCCEEDED(hr) && bInitSavePath && bInitTempPath;
}

e2d::String e2d::Path::getTempPath()
{
	return s_sTempPath;
}

e2d::String e2d::Path::getDefaultSavePath()
{
	return s_sDefaultSavePath;
}

e2d::String e2d::Path::getFileExtension(const String& filePath)
{
	String fileExtension;
	// �ҵ��ļ����е����һ�� '.' ��λ��
	size_t pos = filePath.getWString().find_last_of(L'.');
	// �ж� pos �Ƿ�����Чλ��
	if (pos != std::wstring::npos)
	{
		// ��ȡ��չ��
		fileExtension = filePath.subtract(static_cast<int>(pos));
		// ת��ΪСд��ĸ
		fileExtension = fileExtension.toLower();
	}

	return fileExtension;
}

e2d::String e2d::Path::getSaveFilePath(const String& title, const String& defExt)
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
	ofn.lpstrInitialDir = nullptr;						// ��ʼĿ¼ΪĬ��
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = title;								// ����
	ofn.lpstrDefExt = defExt;							// Ĭ��׷�ӵ���չ��

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}

bool e2d::Path::createFolder(const String& dirPath)
{
	if (dirPath.isEmpty())
	{
		WARN("Path::createFolder Failed: Invalid directory path!");
		return false;
	}

	if (-1 == ::_waccess(dirPath, 0))
	{
		if (0 != ::_wmkdir(dirPath))
		{
			return false;
		}
	}
	return true;
}
