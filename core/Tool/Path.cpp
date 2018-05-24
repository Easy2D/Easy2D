#include "..\e2dtool.h"
#include <algorithm>
#include <list>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


static e2d::String s_sLocalAppDataPath;
static e2d::String s_sTempPath;
static e2d::String s_sDataSavePath;
static std::list<e2d::String> s_vPathList;

bool e2d::Path::__init(const String& gameName)
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
		WARN("Get local AppData path failed!");
		return false;
	}
	
	// ��ȡ���ݵ�Ĭ�ϱ���·��
	s_sDataSavePath = s_sLocalAppDataPath + L"\\Easy2DGameData\\";
	if (!gameName.isEmpty())
	{
		s_sDataSavePath << gameName << L"\\";
	}
	if (!Path::exists(s_sDataSavePath))
	{
		if (!Path::createFolder(s_sDataSavePath))
		{
			s_sDataSavePath = L"";
		}
	}
	s_sDataSavePath << L"Data.ini";

	// ��ȡ��ʱ�ļ�Ŀ¼
	wchar_t path[_MAX_PATH];
	if (0 == ::GetTempPath(_MAX_PATH, path))
	{
		return false;
	}

	s_sTempPath << path << L"\\Easy2DGameTemp\\";
	if (!gameName.isEmpty())
	{
		s_sTempPath << gameName << L"\\";
	}

	if (!Path::exists(s_sTempPath))
	{
		if (!Path::createFolder(s_sTempPath))
		{
			s_sTempPath = L"";
		}
	}

	return true;
}

void e2d::Path::add(String path)
{
	path.replace(L"/", L"\\");
	if (path[path.getLength() - 1] != L'\\')
	{
		path << L"\\";
	}
	auto iter = std::find(s_vPathList.cbegin(), s_vPathList.cend(), path);
	if (iter == s_vPathList.cend())
	{
		s_vPathList.push_front(path);
	}
}

e2d::String e2d::Path::getTempPath()
{
	return s_sTempPath;
}

e2d::String e2d::Path::getExecutableFilePath()
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	if (::GetModuleFileName(nullptr, szPath, _MAX_PATH) != 0)
	{
		return String(szPath);
	}
	return String();
}

e2d::String e2d::Path::checkFilePath(const String& path)
{
	if (Path::exists(path))
	{
		return path;
	}
	else
	{
		for (auto& resPath : s_vPathList)
		{
			if (Path::exists(resPath + path))
			{
				return resPath + path;
			}
		}
	}
	return String();
}

e2d::String e2d::Path::getDataSavePath()
{
	return s_sDataSavePath;
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
	ofn.lpstrTitle = (LPCWSTR)title;								// ����
	ofn.lpstrDefExt = (LPCWSTR)defExt;							// Ĭ��׷�ӵ���չ��

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}

bool e2d::Path::createFolder(const String& dirPath)
{
	if (dirPath.isEmpty() || dirPath.getLength() >= MAX_PATH)
	{
		return false;
	}

	wchar_t tmpDirPath[_MAX_PATH] = { 0 };
	int length = dirPath.getLength();

	for (int i = 0; i < length; ++i)
	{
		tmpDirPath[i] = dirPath.at(i);
		if (tmpDirPath[i] == L'\\' || tmpDirPath[i] == L'/' || i == (length - 1))
		{
			if (::_waccess(tmpDirPath, 0) != 0)
			{
				if (::_wmkdir(tmpDirPath) != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool e2d::Path::exists(const String & path)
{
	if (path.isEmpty() || path.getLength() >= MAX_PATH)
	{
		return false;
	}
	return ::_waccess((const wchar_t *)path, 0) == 0;
}
