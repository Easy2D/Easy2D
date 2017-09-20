#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"
#include <algorithm>
#include <sstream>
#include <Shlobj.h>
#pragma comment(lib, "shell32.lib")

#ifndef UNICODE
	#include <io.h>
	#include <direct.h>
#endif

tstring FileUtils::getLocalAppDataPath()
{
	TCHAR m_lpszDefaultDir[MAX_PATH] = { 0 };
	TCHAR szDocument[MAX_PATH] = { 0 };

	// ��ȡ AppData\Local �ļ��е� ID
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		// ��ȡ�ļ���·��
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}

	return m_lpszDefaultDir;
}

tstring FileUtils::getDefaultSavePath()
{
	TCHAR m_lpszDefaultDir[MAX_PATH] = { 0 };
	TCHAR szDocument[MAX_PATH] = { 0 };

	// ��ȡ AppData\Local �ļ��е� ID
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		// ��ȡ�ļ���·��
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}

	tstring path = m_lpszDefaultDir;
	path.append(_T("\\"));
	path.append(App::get()->getAppName());

#ifdef UNICODE
	if (_waccess(path.c_str(), 0) == -1)
	{
		_wmkdir(path.c_str());
	}
#else
	if (_access(path.c_str(), 0) == -1)
	{
		_mkdir(path.c_str());
	}
#endif

	path.append(_T("\\DefaultData.ini"));

	return path;
}

void FileUtils::saveInt(LPCTSTR key, int value)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << value;
	::WritePrivateProfileString(_T("Default"), key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void FileUtils::saveDouble(LPCTSTR key, double value)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << value;
	::WritePrivateProfileString(_T("Default"), key, ss.str().c_str(), getDefaultSavePath().c_str());
}

void FileUtils::saveString(LPCTSTR key, tstring value)
{
	::WritePrivateProfileString(_T("Default"), key, value.c_str(), getDefaultSavePath().c_str());
}

int FileUtils::getInt(LPCTSTR key, int default)
{
	return ::GetPrivateProfileInt(_T("Default"), key, default, getDefaultSavePath().c_str());
}
#include <iostream>
using namespace std;
double FileUtils::getDouble(LPCTSTR key, double default)
{
	// �� default ����ת��Ϊ�ַ���
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << default;
	// ��ȡ����
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(_T("Default"), key, ss.str().c_str(), temp, 128, getDefaultSavePath().c_str());
	// ת��Ϊ�ַ�����
	ss.str(_T(""));
	ss << temp;
	// ���ַ���ת��Ϊ double
#ifdef UNICODE
	double d = _wtof(ss.str().c_str());
#else
	double d = atof(ss.str().c_str());
#endif
	return d;
}

tstring FileUtils::getString(LPCTSTR key, tstring default)
{
	TCHAR temp[128] = { 0 };
	::GetPrivateProfileString(_T("Default"), key, default.c_str(), temp, 128, getDefaultSavePath().c_str());
	return tstring(temp);
}

tstring FileUtils::getFileExtension(const tstring & filePath)
{
	tstring fileExtension;
	// �ҵ��ļ����е����һ�� '.' ��λ��
	size_t pos = filePath.find_last_of('.');
	// �ж� pos �Ƿ��Ǹ���Чλ��
	if (pos != tstring::npos)
	{
		// ��ȡ��չ��
		fileExtension = filePath.substr(pos, filePath.length());
		// ת��ΪСд��ĸ
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
	}

	return fileExtension;
}

bool FileUtils::getSaveFilePath(tstring& path, LPCTSTR title, LPCTSTR defExt)
{
	// ��������Ի���
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };				// ���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);				// �ṹ���С
	ofn.hwndOwner = GetHWnd();							// ӵ���Ŵ��ھ����NULL ��ʾ�Ի����Ƿ�ģ̬��
	ofn.lpstrFilter = _T("�����ļ�\0*.*\0\0");			// ���ù���
	ofn.nFilterIndex = 1;								// ����������
	ofn.lpstrFile = strFilename;						// ���շ��ص��ļ�·�����ļ���
	ofn.nMaxFile = sizeof(strFilename);					// ����������
	ofn.lpstrInitialDir = NULL;							// ��ʼĿ¼ΪĬ��
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;// Ŀ¼������ڣ������ļ�ǰ��������
	ofn.lpstrTitle = title;								// ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrDefExt = defExt;							// Ĭ��׷�ӵ���չ��

	if (GetSaveFileName(&ofn))
	{
		path = strFilename;
		return true;
	}
	return false;
}
