#include "..\etools.h"
#include <map>
#include <mmsystem.h>
#pragma comment(lib , "winmm.lib")

#define WIN_CLASS_NAME L"MciPlayerCallbackWnd"

static HINSTANCE s_hInstance = nullptr;

LRESULT WINAPI _MciPlayerProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);

class MciPlayer
{
public:
	MciPlayer();
	~MciPlayer();

	void close();
	bool open(const e2d::EString & pFileName, UINT uId);
	bool open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId);
	void play(int repeatTimes);
	void pause();
	void resume();
	void stop();
	void rewind();
	bool isPlaying();
	UINT getMusicID();

private:
	friend LRESULT WINAPI _MciPlayerProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void _sendCommand(int nCommand, DWORD_PTR param1 = 0, DWORD_PTR parma2 = 0);

	MCIDEVICEID m_dev;
	HWND        m_wnd;
	UINT		m_nMusicID;
	bool        m_bPlaying;
	int			m_nRepeatTimes;
};


MciPlayer::MciPlayer()
	: m_wnd(NULL)
	, m_dev(0L)
	, m_nMusicID(0)
	, m_bPlaying(false)
	, m_nRepeatTimes(0)
{
	if (!s_hInstance)
	{
		s_hInstance = HINST_THISCOMPONENT;

		WNDCLASS  wc;
		wc.style = 0;
		wc.lpfnWndProc = _MciPlayerProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = s_hInstance;
		wc.hIcon = 0;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WIN_CLASS_NAME;

		if (!RegisterClass(&wc) && 1410 != GetLastError())
		{
			return;
		}
	}

	m_wnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		WIN_CLASS_NAME,
		NULL,
		WS_POPUPWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		s_hInstance,
		NULL);

	if (m_wnd)
	{
		SetWindowLongPtr(m_wnd, GWLP_USERDATA, (LONG_PTR)this);
	}
}

MciPlayer::~MciPlayer()
{
	close();
	DestroyWindow(m_wnd);
}

bool MciPlayer::open(const e2d::EString & pFileName, UINT uId)
{
	if (pFileName.isEmpty())
		return false;

	close();

	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrDeviceType = 0;
	mciOpen.lpstrElementName = pFileName;

	MCIERROR mciError;
	mciError = mciSendCommand(
		0,
		MCI_OPEN,
		MCI_OPEN_ELEMENT,
		reinterpret_cast<DWORD_PTR>(&mciOpen)
	);

	if (mciError == 0)
	{
		m_dev = mciOpen.wDeviceID;
		m_nMusicID = uId;
		m_bPlaying = false;
		return true;
	}
	return false;
}

bool MciPlayer::open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId)
{
	// ���Բ����ڵ��ļ�
	if (pResouceName.isEmpty() || pResouceType.isEmpty() || musicExtension.isEmpty()) return false;

	// ��ȡ��ʱ�ļ�Ŀ¼
	e2d::EString tempFileName = e2d::EFile::getTempPath();

	// ������ʱ�ļ����ļ���
	tempFileName = tempFileName + L"\\" + uId + L"." + musicExtension;

	// ������ԴΪ��ʱ�ļ�
	if (ExtractResource(tempFileName, pResouceType, pResouceName))
	{
		return open(tempFileName, uId);
	}
	return false;
}

void MciPlayer::play(int repeatTimes)
{
	if (!m_dev)
	{
		return;
	}

	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(m_wnd);

	// ��������
	MCIERROR mciError = mciSendCommand(
		m_dev,
		MCI_PLAY,
		MCI_FROM | MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);

	if (!mciError)
	{
		m_bPlaying = true;
		m_nRepeatTimes = repeatTimes;
	}
}

void MciPlayer::close()
{
	if (m_bPlaying)
	{
		stop();
	}

	if (m_dev)
	{
		_sendCommand(MCI_CLOSE);
	}

	m_dev = 0;
	m_bPlaying = false;
}

void MciPlayer::pause()
{
	_sendCommand(MCI_PAUSE);
	m_bPlaying = false;
}

void MciPlayer::resume()
{
	_sendCommand(MCI_RESUME);
	m_bPlaying = true;
}

void MciPlayer::stop()
{
	_sendCommand(MCI_STOP);
	m_bPlaying = false;
}

void MciPlayer::rewind()
{
	if (!m_dev)
	{
		return;
	}

	mciSendCommand(m_dev, MCI_SEEK, MCI_SEEK_TO_START, 0);

	MCI_PLAY_PARMS mciPlay = { 0 };
	mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(m_wnd);

	MCIERROR mciError = mciSendCommand(
		m_dev,
		MCI_PLAY,
		MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);
	m_bPlaying = mciError ? false : true;
}

bool MciPlayer::isPlaying()
{
	return m_bPlaying;
}

UINT MciPlayer::getMusicID()
{
	return m_nMusicID;
}

void MciPlayer::_sendCommand(int nCommand, DWORD_PTR param1, DWORD_PTR parma2)
{
	// ���豸ʱ������β���
	if (!m_dev)
	{
		return;
	}
	// ��ǰ�豸���Ͳ���
	mciSendCommand(m_dev, nCommand, param1, parma2);
}




bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName)
{
	// �����ļ�
	HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
	HRSRC	hRes = ::FindResource(NULL, strResName, strResType);
	HGLOBAL	hMem = ::LoadResource(NULL, hRes);
	DWORD	dwSize = ::SizeofResource(NULL, hRes);

	// д���ļ�
	DWORD dwWrite = 0;  	// ����д���ֽ�
	::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
	::CloseHandle(hFile);

	return true;
}

LRESULT WINAPI _MciPlayerProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	MciPlayer * pPlayer = NULL;

	if (Msg == MM_MCINOTIFY
		&& wParam == MCI_NOTIFY_SUCCESSFUL
		&& (pPlayer = (MciPlayer *)GetWindowLongPtr(hWnd, GWLP_USERDATA)))
	{
		if (pPlayer->m_nRepeatTimes > 0)
		{
			pPlayer->m_nRepeatTimes--;
		}

		if (pPlayer->m_nRepeatTimes)
		{
			mciSendCommand(static_cast<MCIDEVICEID>(lParam), MCI_SEEK, MCI_SEEK_TO_START, 0);

			MCI_PLAY_PARMS mciPlay = { 0 };
			mciPlay.dwCallback = reinterpret_cast<DWORD_PTR>(hWnd);
			mciSendCommand(static_cast<MCIDEVICEID>(lParam), MCI_PLAY, MCI_NOTIFY, reinterpret_cast<DWORD_PTR>(&mciPlay));
		}
		else
		{
			pPlayer->m_bPlaying = false;
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}




typedef std::pair<UINT, MciPlayer *> MusicPair;
typedef std::map<UINT, MciPlayer *> MusicList;

static MusicList& getMciPlayerList()
{
	static MusicList s_List;
	return s_List;
}


UINT e2d::EMusic::play(const EString & musicFilePath, int repeatTimes)
{
	UINT nRet = preload(musicFilePath);

	if (nRet)
	{
		getMciPlayerList()[nRet]->play(repeatTimes);
	}
	return nRet;
}

UINT e2d::EMusic::play(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension, int repeatTimes)
{
	UINT nRet = preload(musicResourceName, musicResourceType, musicExtension);

	if (nRet)
	{
		getMciPlayerList()[nRet]->play(repeatTimes);
	}
	return nRet;
}

UINT e2d::EMusic::preload(const EString & musicFilePath)
{
	if (musicFilePath.isEmpty()) 
		return 0;

	UINT nRet = musicFilePath.hash();

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) 
		return nRet;

	getMciPlayerList().insert(MusicPair(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicFilePath, nRet);

	if (nRet == pPlayer->getMusicID()) return nRet;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	return 0;
}

UINT e2d::EMusic::preload(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension)
{
	if (musicResourceName.isEmpty() || musicResourceType.isEmpty())
		return 0;

	UINT nRet = musicResourceName.hash();

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) 
		return nRet;

	getMciPlayerList().insert(MusicPair(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicResourceName, musicResourceType, musicExtension, nRet);

	if (nRet == pPlayer->getMusicID()) return nRet;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	return 0;
}

bool e2d::EMusic::resume(UINT musicId)
{
	MusicList::iterator p = getMciPlayerList().find(musicId);
	if (p != getMciPlayerList().end())
	{
		p->second->resume();
		return true;
	}
	return false;
}

bool e2d::EMusic::resume(const EString & musicName)
{
	return resume(musicName.hash());;
}

bool e2d::EMusic::pause(UINT musicId)
{
	MusicList::iterator p = getMciPlayerList().find(musicId);
	if (p != getMciPlayerList().end())
	{
		p->second->pause();
		return true;
	}
	return false;
}

bool e2d::EMusic::pause(const EString & musicName)
{
	return pause(musicName.hash());
}

bool e2d::EMusic::stop(UINT musicId)
{
	MusicList::iterator p = getMciPlayerList().find(musicId);
	if (p != getMciPlayerList().end())
	{
		p->second->stop();
		return true;
	}
	return false;
}

bool e2d::EMusic::stop(const EString & musicName)
{
	return stop(musicName.hash());;
}

void e2d::EMusic::pauseAllMusics()
{
	for (auto iter = getMciPlayerList().begin(); iter != getMciPlayerList().end(); iter++)
	{
		(*iter).second->pause();
	}
}

void e2d::EMusic::resumeAllMusics()
{
	for (auto iter = getMciPlayerList().begin(); iter != getMciPlayerList().end(); iter++)
	{
		(*iter).second->resume();
	}
}

void e2d::EMusic::stopAllMusics()
{
	for (auto iter = getMciPlayerList().begin(); iter != getMciPlayerList().end(); iter++)
	{
		(*iter).second->stop();
	}
}
