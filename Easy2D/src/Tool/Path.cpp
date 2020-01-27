#include <easy2d/e2dtool.h>
#include <algorithm>
#include <list>
#include <commdlg.h>

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2,{ b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_KNOWN_FOLDER(FOLDERID_LocalAppData, 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91);


static easy2d::String s_sLocalAppDataPath;
static easy2d::String s_sTempPath;
static easy2d::String s_sDataSavePath;
static std::list<easy2d::String> s_vPathList;

bool easy2d::Path::__init(const String& gameName)
{
	// 获取 AppData\Local 文件夹的路径
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
		E2D_WARNING(L"Get local AppData path failed!");
		return false;
	}
	
	// 获取数据的默认保存路径
	s_sDataSavePath = s_sLocalAppDataPath + L"\\Easy2DGameData\\";
	if (!gameName.empty())
	{
		s_sDataSavePath.append(gameName).append(L"\\");
	}
	if (!Path::exists(s_sDataSavePath))
	{
		if (!Path::createFolder(s_sDataSavePath))
		{
			s_sDataSavePath = L"";
		}
	}
	s_sDataSavePath.append(L"Data.ini");

	// 获取临时文件目录
	wchar_t path[_MAX_PATH];
	if (0 == ::GetTempPath(_MAX_PATH, path))
	{
		return false;
	}

	s_sTempPath.append(path).append(L"\\Easy2DGameTemp\\");
	if (!gameName.empty())
	{
		s_sTempPath.append(gameName).append(L"\\");
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

void easy2d::Path::add(String path)
{
	if (path[path.length() - 1] != L'\\' && path[path.length() - 1] != L'/')
	{
		path.append(L"\\");
	}
	auto iter = std::find(s_vPathList.cbegin(), s_vPathList.cend(), path);
	if (iter == s_vPathList.cend())
	{
		s_vPathList.push_front(path);
	}
}

easy2d::String easy2d::Path::getTempPath()
{
	return s_sTempPath;
}

easy2d::String easy2d::Path::getExecutableFilePath()
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	if (::GetModuleFileName(nullptr, szPath, _MAX_PATH) != 0)
	{
		return String(szPath);
	}
	return String();
}

easy2d::String easy2d::Path::searchForFile(const String& path)
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

easy2d::String easy2d::Path::extractResource(int resNameId, const String & resType, const String & destFileName)
{
	String destFilePath = s_sTempPath + destFileName;
	// 创建文件
	HANDLE hFile = ::CreateFile(destFilePath.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return String();

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC hRes = ::FindResource(NULL, MAKEINTRESOURCE(resNameId), resType.c_str());
	HGLOBAL hMem = ::LoadResource(NULL, hRes);
	DWORD dwSize = ::SizeofResource(NULL, hRes);

	if (hRes && hMem && dwSize)
	{
		// 写入文件
		DWORD dwWrite = 0;
		::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
		::CloseHandle(hFile);
		return destFilePath;
	}
	else
	{
		::CloseHandle(hFile);
		::DeleteFile(destFilePath.c_str());
		return String();
	}
}

easy2d::String easy2d::Path::getDataSavePath()
{
	return s_sDataSavePath;
}

easy2d::String easy2d::Path::getSaveFilePath(const String& title, const String& defExt)
{
	// 弹出保存对话框
	OPENFILENAME ofn = { 0 };
	wchar_t strFilename[MAX_PATH] = { 0 };				// 用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);				// 结构体大小
	ofn.hwndOwner = Window::getHWnd();					// 窗口句柄
	ofn.lpstrFilter = L"所有文件\0*.*\0\0";				// 设置过滤
	ofn.nFilterIndex = 1;								// 过滤器索引
	ofn.lpstrFile = strFilename;						// 接收返回的文件路径和文件名
	ofn.nMaxFile = sizeof(strFilename);					// 缓冲区长度
	ofn.lpstrInitialDir = nullptr;						// 初始目录为默认
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = title.c_str();						// 标题
	ofn.lpstrDefExt = defExt.c_str();					// 默认追加的扩展名

	if (GetSaveFileName(&ofn))
	{
		return strFilename;
	}
	return L"";
}

bool easy2d::Path::createFolder(const String& dirPath)
{
	if (dirPath.empty() || dirPath.length() >= MAX_PATH)
	{
		return false;
	}

	wchar_t tmpDirPath[_MAX_PATH] = { 0 };
	int length = (int)dirPath.length();

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

bool easy2d::Path::exists(const String & path)
{
	if (path.empty() || path.length() >= MAX_PATH)
	{
		return false;
	}
	return ::_waccess(path.c_str(), 0) == 0;
}
