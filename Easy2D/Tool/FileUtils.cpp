#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"
#include <algorithm>


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
