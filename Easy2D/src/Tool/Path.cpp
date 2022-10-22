#include <easy2d/e2dtool.h>
#include <algorithm>
#include <list>
#include <commdlg.h>
#include <io.h> // _access
#include <direct.h> // _mkdir
#include <shlobj_core.h> // SHGetFolderPathA
#include <Shlobj.h> // CSIDL_LOCAL_APPDATA

static easy2d::String s_sLocalAppDataPath;
static easy2d::String s_sTempPath;
static easy2d::String s_sDataSavePath;
static std::list<easy2d::String> s_vPathList;

bool easy2d::Path::__init(const String& gameName)
{
	char szPath[MAX_PATH];
	HRESULT hr = SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, szPath);

	if (SUCCEEDED(hr))
	{
		s_sLocalAppDataPath = szPath;
	}
	else
	{
		E2D_WARNING("Get local AppData path failed!");
		return false;
	}
	
	// ��ȡ���ݵ�Ĭ�ϱ���·��
	s_sDataSavePath = s_sLocalAppDataPath + "\\Easy2DGameData\\";
	if (!gameName.empty())
	{
		s_sDataSavePath.append(gameName).append("\\");
	}
	if (!Path::exists(s_sDataSavePath))
	{
		if (!Path::createFolder(s_sDataSavePath))
		{
			s_sDataSavePath = "";
		}
	}
	s_sDataSavePath.append("Data.ini");

	// ��ȡ��ʱ�ļ�Ŀ¼
	char path[_MAX_PATH];
	if (0 == ::GetTempPathA(_MAX_PATH, path))
	{
		return false;
	}

	s_sTempPath.append(path).append("\\Easy2DGameTemp\\");
	if (!gameName.empty())
	{
		s_sTempPath.append(gameName).append("\\");
	}

	if (!Path::exists(s_sTempPath))
	{
		if (!Path::createFolder(s_sTempPath))
		{
			s_sTempPath = "";
		}
	}

	return true;
}

void easy2d::Path::add(String path)
{
	if (path[path.length() - 1] != L'\\' && path[path.length() - 1] != L'/')
	{
		path.append("\\");
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
	char szPath[_MAX_PATH] = { 0 };
	if (::GetModuleFileNameA(nullptr, szPath, _MAX_PATH) != 0)
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
	// �����ļ�
	HANDLE hFile = ::CreateFileA(destFilePath.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return String();

	// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
	Resource res{ resNameId, resType };
	auto data = res.loadData();

	if (data.isValid())
	{
		// д���ļ�
		DWORD dwWrite = 0;
		::WriteFile(hFile, data.buffer, (DWORD)data.size, &dwWrite, NULL);
		::CloseHandle(hFile);
		return destFilePath;
	}
	else
	{
		::CloseHandle(hFile);
		::DeleteFileA(destFilePath.c_str());
		return String();
	}
}

easy2d::String easy2d::Path::getDataSavePath()
{
	return s_sDataSavePath;
}

easy2d::String easy2d::Path::getSaveFilePath(const String& title, const String& defExt)
{
	// ��������Ի���
	OPENFILENAMEA ofn = { 0 };
	char strFilename[MAX_PATH] = { 0 };					// ���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);				// �ṹ���С
	ofn.hwndOwner = Window::getHWnd();					// ���ھ��
	ofn.lpstrFilter = "�����ļ�\0*.*\0\0";				// ���ù���
	ofn.nFilterIndex = 1;								// ����������
	ofn.lpstrFile = strFilename;						// ���շ��ص��ļ�·�����ļ���
	ofn.nMaxFile = sizeof(strFilename);					// ����������
	ofn.lpstrInitialDir = nullptr;						// ��ʼĿ¼ΪĬ��
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = title.c_str();						// ����
	ofn.lpstrDefExt = defExt.c_str();					// Ĭ��׷�ӵ���չ��

	if (GetSaveFileNameA(&ofn))
	{
		return strFilename;
	}
	return "";
}

bool easy2d::Path::createFolder(const String& dirPath)
{
	if (dirPath.empty() || dirPath.length() >= MAX_PATH)
	{
		return false;
	}

	char tmpDirPath[_MAX_PATH] = { 0 };
	int length = (int)dirPath.length();

	for (int i = 0; i < length; ++i)
	{
		tmpDirPath[i] = dirPath.at(i);
		if (tmpDirPath[i] == L'\\' || tmpDirPath[i] == L'/' || i == (length - 1))
		{
			if (_access_s(tmpDirPath, 0) != 0)
			{
				if (_mkdir(tmpDirPath) != 0)
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
	return _access_s(path.c_str(), 0) == 0;
}
