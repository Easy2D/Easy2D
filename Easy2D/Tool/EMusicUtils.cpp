#include "..\etools.h"
#include "..\Win\winbase.h"
#include <map>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib , "winmm.lib")

static size_t Hash(const e2d::EString & key);
static bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);


////////////////////////////////////////////////////////////////////
// MciPlayer
////////////////////////////////////////////////////////////////////

class MciPlayer
{
public:
	MciPlayer();
	~MciPlayer();

	void close();
	bool open(const e2d::EString & pFileName, UINT uId);
	bool open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId);
	void play(bool bLoop = false);
	void pause();
	void resume();
	void stop();
	void rewind();
	void setVolume(float volume);
	bool isPlaying();
	UINT getSoundID();

private:
	void _sendCommand(int nCommand, DWORD_PTR param1 = 0, DWORD_PTR parma2 = 0);

	MCIDEVICEID m_dev;
	UINT        m_nSoundID;
	bool        m_bPlaying;
	bool		m_bLoop;
	e2d::EString m_sTempFileName;
};


MciPlayer::MciPlayer() :
	m_dev(0L),
	m_nSoundID(0),
	m_bPlaying(false),
	m_bLoop(false)
{
}

MciPlayer::~MciPlayer()
{
	close();	// �رղ�����
}

bool MciPlayer::open(const e2d::EString & pFileName, UINT uId)
{
	// ���Բ����ڵ��ļ�
	if (pFileName.empty()) 
		return false;

	// ֹͣ��ǰ����
	close();

	// ���� MCI_OPEN_PARMS ����
	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrElementName = pFileName.c_str();

	// ������ļ�
	MCIERROR mciError;
	mciError = mciSendCommand(
		0, 
		MCI_OPEN, 
		MCI_OPEN_ELEMENT | MCI_NOTIFY,
		reinterpret_cast<DWORD_PTR>(&mciOpen)
	);

	if (mciError)
	{
		return false;
	}
	else
	{
		// �����豸����Ϣ
		m_dev = mciOpen.wDeviceID;
		m_nSoundID = uId;
		m_bPlaying = false;
		return true;
	}
}

bool MciPlayer::open(const e2d::EString & pResouceName, const e2d::EString & pResouceType, const e2d::EString & musicExtension, UINT uId)
{
	// ���Բ����ڵ��ļ�
	if (pResouceName.empty() || pResouceType.empty() || musicExtension.empty()) return false;

	// ��ȡ��ʱ�ļ�Ŀ¼
	e2d::EString tempFileName = e2d::EFileUtils::getTempPath();

	// ������ʱ�ļ����ļ���
	tempFileName.append(L"\\");
	tempFileName.append(std::to_wstring(uId));
	tempFileName.append(L"." + musicExtension);

	// ������ԴΪ��ʱ�ļ�
	if (ExtractResource(tempFileName.c_str(), pResouceType.c_str(), pResouceName.c_str()))
	{
		if (open(tempFileName, uId))
		{
			m_sTempFileName = tempFileName;
			return true;
		}
	}
	return false;
}

void MciPlayer::play(bool bLoop)
{
	// �豸Ϊ��ʱ��������β���
	if (!m_dev)
	{
		return;
	}
	// ���ò��Ų���
	MCI_PLAY_PARMS mciPlay = { 0 };
	MCIERROR s_mciError;

	// ��������
	s_mciError = mciSendCommand(
		m_dev, 
		MCI_PLAY, 
		MCI_FROM | MCI_NOTIFY | (bLoop ? MCI_DGV_PLAY_REPEAT : 0),
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);
	
	// δ����ʱ���� m_bPlaying Ϊ true
	if (!s_mciError)
	{
		m_bPlaying = true;
		m_bLoop = bLoop;
	}
}

void MciPlayer::close()
{
	// ֹͣ����
	if (m_bPlaying)
	{
		stop();
	}
	// �ر��豸
	if (m_dev)
	{
		_sendCommand(MCI_CLOSE);
	}
	// ɾ����ʱ�ļ�
	if (!m_sTempFileName.empty())
	{
		DeleteFile(m_sTempFileName.c_str());
		m_sTempFileName.clear();
	}
	// �ָ�Ĭ������
	m_dev = 0;
	m_bPlaying = false;
}

void MciPlayer::pause()
{
	// ��ͣ����
	_sendCommand(MCI_PAUSE);
	m_bPlaying = false;
}

void MciPlayer::resume()
{
	// ������������
	_sendCommand(MCI_RESUME);
	m_bPlaying = true;
}

void MciPlayer::stop()
{
	// ֹͣ����
	_sendCommand(MCI_STOP);
	m_bPlaying = false;
}

void MciPlayer::rewind()
{
	// �豸Ϊ��ʱ��������β���
	if (!m_dev)
	{
		return;
	}
	// ���ò���λ��
	mciSendCommand(
		m_dev,
		MCI_SEEK, 
		MCI_SEEK_TO_START | MCI_NOTIFY, 
		0
	);
	// ��������
	MCI_PLAY_PARMS mciPlay = { 0 };
	MCIERROR s_mciError;

	// ��������
	s_mciError = mciSendCommand(
		m_dev, 
		MCI_PLAY,
		MCI_NOTIFY | (m_bLoop ? MCI_DGV_PLAY_REPEAT : 0),
		reinterpret_cast<DWORD_PTR>(&mciPlay)
	);
	m_bPlaying = s_mciError ? false : true;
}

void MciPlayer::setVolume(float volume)
{
	MCI_DGV_SETAUDIO_PARMS  mciSetAudioPara = { 0 };
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = DWORD(1000 * min(max(volume, 0), 1));
	mciSendCommand(
		m_dev, 
		MCI_SETAUDIO, 
		MCI_NOTIFY | MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM,
		(DWORD_PTR)&mciSetAudioPara
	);
}

bool MciPlayer::isPlaying()
{
	return m_bPlaying;
}

UINT MciPlayer::getSoundID()
{
	return m_nSoundID;
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




////////////////////////////////////////////////////////////////////
// e2d::EMusicUtils
////////////////////////////////////////////////////////////////////


typedef std::map<unsigned int, MciPlayer *> MusicList;
typedef std::pair<unsigned int, MciPlayer *> Music;


static MusicList& getMciPlayerList()
{
	static MusicList s_List;
	return s_List;
}

static MciPlayer& getBgMciPlayer()
{
	static MciPlayer s_Music;
	return s_Music;
}

void e2d::EMusicUtils::end()
{
	// ֹͣ��������
	getBgMciPlayer().close();
	// ֹͣ������������
	for (auto& iter : getMciPlayerList())
	{
		SafeDelete(&iter.second);
	}
	// ��������б�
	getMciPlayerList().clear();
	return;
}

void e2d::EMusicUtils::setVolume(float volume)
{
	// ���ñ�����������
	getBgMciPlayer().setVolume(volume);
	// ����������������
	for (auto& iter : getMciPlayerList())
	{
		iter.second->setVolume(volume);
	}
}

void e2d::EMusicUtils::setVolume(const EString & musicFilePath, float volume)
{
	unsigned int nRet = ::Hash(musicFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->setVolume(volume);
	}
}

void e2d::EMusicUtils::playBackgroundMusic(const EString & musicFilePath, bool bLoop)
{
	if (musicFilePath.empty())
	{
		return;
	}

	getBgMciPlayer().open(musicFilePath, ::Hash(musicFilePath));
	getBgMciPlayer().play(bLoop);
}

void e2d::EMusicUtils::playBackgroundMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension, bool loop)
{
	if (musicResourceName.empty() || musicResourceType.empty())
	{
		return;
	}

	getBgMciPlayer().open(musicResourceName, musicResourceType, musicExtension, ::Hash(musicResourceName));
	getBgMciPlayer().play(loop);
}

void e2d::EMusicUtils::stopBackgroundMusic(bool bReleaseData)
{
	if (bReleaseData)
	{
		getBgMciPlayer().close();
	}
	else
	{
		getBgMciPlayer().stop();
	}
}

void e2d::EMusicUtils::pauseBackgroundMusic()
{
	getBgMciPlayer().pause();
}

void e2d::EMusicUtils::resumeBackgroundMusic()
{
	getBgMciPlayer().resume();
}

void e2d::EMusicUtils::rewindBackgroundMusic()
{
	getBgMciPlayer().rewind();
}

bool e2d::EMusicUtils::isBackgroundMusicPlaying()
{
	return getBgMciPlayer().isPlaying();
}

void e2d::EMusicUtils::setBackgroundMusicVolume(float volume)
{
	getBgMciPlayer().setVolume(volume);
}

void e2d::EMusicUtils::playMusic(const EString & musicFilePath, bool bLoop)
{
	unsigned int nRet = ::Hash(musicFilePath);

	preloadMusic(musicFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->play(bLoop);
	}
}

void e2d::EMusicUtils::playMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension, bool loop)
{
	unsigned int nRet = ::Hash(musicResourceName);

	preloadMusic(musicResourceName, musicResourceType, musicExtension);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->play(loop);
	}
}

void e2d::EMusicUtils::_stopMusic(size_t nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->stop();
	}
}

void e2d::EMusicUtils::preloadMusic(const EString & musicFilePath)
{
	if (musicFilePath.empty()) return;

	int nRet = ::Hash(musicFilePath);

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) return;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicFilePath, nRet);

	if (nRet == pPlayer->getSoundID()) return;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	nRet = 0;
}

void e2d::EMusicUtils::preloadMusic(const EString & musicResourceName, const EString & musicResourceType, const EString & musicExtension)
{
	if (musicResourceName.empty() || musicResourceType.empty()) return;

	int nRet = ::Hash(musicResourceName);

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) return;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(musicResourceName, musicResourceType, musicExtension, nRet);

	if (nRet == pPlayer->getSoundID()) return;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	nRet = 0;
}

void e2d::EMusicUtils::_pauseMusic(size_t nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->pause();
	}
}

void e2d::EMusicUtils::pauseAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->pause();
	}
}

void e2d::EMusicUtils::_resumeMusic(size_t nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->resume();
	}
}

void e2d::EMusicUtils::resumeAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->resume();
	}
}

void e2d::EMusicUtils::stopAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->stop();
	}
}

void e2d::EMusicUtils::unloadMusic(const EString & musicFilePath)
{
	unsigned int nID = ::Hash(musicFilePath);

	MusicList::iterator p = getMciPlayerList().find(nID);
	if (p != getMciPlayerList().end())
	{
		SafeDelete(&p->second);
		getMciPlayerList().erase(nID);
	}
}




size_t Hash(const e2d::EString & key)
{
	static std::hash<e2d::EString> h;
	return h(key);
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