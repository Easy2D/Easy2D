#include "..\Easy2d.h"
#include <Digitalv.h>
#include <mmsystem.h>
#pragma comment(lib , "winmm.lib")
#include <map>

////////////////////////////////////////////////////////////////////
// MciPlayer
////////////////////////////////////////////////////////////////////

class MciPlayer
{
public:
	MciPlayer();
	~MciPlayer();

	void close();
	void open(tstring pFileName, UINT uId);
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
	tstring		m_sExt;
};


MciPlayer::MciPlayer() :
	m_dev(0L),
	m_nSoundID(0),
	m_bPlaying(false),
	m_bLoop(false),
	m_sExt(_T(""))
{
}

MciPlayer::~MciPlayer()
{
	close();	// �رղ�����
}

void MciPlayer::open(tstring pFileName, UINT uId)
{
	// ���Բ����ڵ��ļ�
	if (pFileName.empty() || !PathFileExists(pFileName.c_str())) return;
	// ��ȡ�ļ���׺��
	m_sExt = FileUtils::getFileExtension(pFileName);
	// ֹͣ��ǰ����
	close();

	// ���� MCI_OPEN_PARMS ����
	MCI_OPEN_PARMS mciOpen = { 0 };
	mciOpen.lpstrDeviceType = (LPCTSTR)MCI_ALL_DEVICE_ID;
	mciOpen.lpstrElementName = pFileName.c_str();

	// ������ļ�
	MCIERROR mciError;
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, reinterpret_cast<DWORD_PTR>(&mciOpen));
	// ���ִ���ʱ��������β���
	if (mciError) return;

	// �����豸����Ϣ
	m_dev = mciOpen.wDeviceID;
	m_nSoundID = uId;
	m_bPlaying = false;
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
	s_mciError = mciSendCommand(m_dev, MCI_PLAY, MCI_FROM | (bLoop ? MCI_DGV_PLAY_REPEAT : 0), reinterpret_cast<DWORD_PTR>(&mciPlay));
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
	// ��������
	if (m_sExt == _T(".mid"))
	{
		// midi ��֧�� MCI_RESUME ������Ӧʹ�� MCI_FROM ���ò�����ʼλ��
		// ��ȡ MCI ״̬
		MCI_STATUS_PARMS mciStatusParms;
		mciStatusParms.dwItem = MCI_STATUS_POSITION;
		_sendCommand(MCI_STATUS, MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&mciStatusParms));
		// ���ò�����ʼλ�ã�����ʼ����
		MCI_PLAY_PARMS mciPlayParms;
		mciPlayParms.dwFrom = mciStatusParms.dwReturn;
		_sendCommand(MCI_PLAY, MCI_FROM, reinterpret_cast<DWORD_PTR>(&mciPlayParms));
	}
	else
	{
		// ������������
		_sendCommand(MCI_RESUME);
		m_bPlaying = true;
	}
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
	mciSendCommand(m_dev, MCI_SEEK, MCI_SEEK_TO_START, 0);
	// ��������
	MCI_PLAY_PARMS mciPlay = { 0 };
	m_bPlaying = mciSendCommand(m_dev, MCI_PLAY, (m_bLoop ? MCI_DGV_PLAY_REPEAT : 0), reinterpret_cast<DWORD_PTR>(&mciPlay)) ? false : true;
}

void MciPlayer::setVolume(float volume)
{
	volume = min(max(volume, 0), 1);
	MCI_DGV_SETAUDIO_PARMS  mciSetAudioPara = { 0 };
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = DWORD(1000 * volume);
	mciSendCommand(m_dev, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
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
// MusicUtils
////////////////////////////////////////////////////////////////////


typedef std::map<unsigned int, MciPlayer *> MusicList;
typedef std::pair<unsigned int, MciPlayer *> Music;

static unsigned int _Hash(tstring key);


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

void MusicUtils::end()
{
	// ֹͣ��������
	getBgMciPlayer().close();
	// ֹͣ������������
	for (auto& iter : getMciPlayerList())
	{
		SAFE_DELETE(iter.second);
	}
	// ��������б�
	getMciPlayerList().clear();
	return;
}

void MusicUtils::setVolume(float volume)
{
	// ���ñ�����������
	getBgMciPlayer().setVolume(volume);
	// ����������������
	for (auto& iter : getMciPlayerList())
	{
		iter.second->setVolume(volume);
	}
}

void MusicUtils::setVolume(tstring pszFilePath, float volume)
{
	unsigned int nRet = ::_Hash(pszFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->setVolume(volume);
	}
}

void MusicUtils::playBackgroundMusic(tstring pszFilePath, bool bLoop)
{
	if (pszFilePath.empty())
	{
		return;
	}

	getBgMciPlayer().open(pszFilePath, ::_Hash(pszFilePath));
	getBgMciPlayer().play(bLoop);
}

void MusicUtils::stopBackgroundMusic(bool bReleaseData)
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

void MusicUtils::pauseBackgroundMusic()
{
	getBgMciPlayer().pause();
}

void MusicUtils::resumeBackgroundMusic()
{
	getBgMciPlayer().resume();
}

void MusicUtils::rewindBackgroundMusic()
{
	getBgMciPlayer().rewind();
}

bool MusicUtils::isBackgroundMusicPlaying()
{
	return getBgMciPlayer().isPlaying();
}

void MusicUtils::setBackgroundMusicVolume(float volume)
{
	getBgMciPlayer().setVolume(volume);
}

unsigned int MusicUtils::playMusic(tstring pszFilePath, bool bLoop)
{
	unsigned int nRet = ::_Hash(pszFilePath);

	preloadMusic(pszFilePath);

	MusicList::iterator p = getMciPlayerList().find(nRet);
	if (p != getMciPlayerList().end())
	{
		p->second->play(bLoop);
	}
	return nRet;
}

void MusicUtils::stopMusic(unsigned int nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->stop();
	}
}

void MusicUtils::preloadMusic(tstring pszFilePath)
{
	if (pszFilePath.empty()) return;

	int nRet = ::_Hash(pszFilePath);

	if (getMciPlayerList().end() != getMciPlayerList().find(nRet)) return;

	getMciPlayerList().insert(Music(nRet, new MciPlayer()));
	MciPlayer * pPlayer = getMciPlayerList()[nRet];
	pPlayer->open(pszFilePath, nRet);

	if (nRet == pPlayer->getSoundID()) return;

	delete pPlayer;
	getMciPlayerList().erase(nRet);
	nRet = 0;
}

void MusicUtils::pauseMusic(unsigned int nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->pause();
	}
}

void MusicUtils::pauseAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->pause();
	}
}

void MusicUtils::resumeMusic(unsigned int nSoundId)
{
	MusicList::iterator p = getMciPlayerList().find(nSoundId);
	if (p != getMciPlayerList().end())
	{
		p->second->resume();
	}
}

void MusicUtils::resumeAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->resume();
	}
}

void MusicUtils::stopAllMusics()
{
	for (auto& iter : getMciPlayerList())
	{
		iter.second->stop();
	}
}

void MusicUtils::unloadMusic(LPCTSTR pszFilePath)
{
	unsigned int nID = ::_Hash(pszFilePath);
	
	MusicList::iterator p = getMciPlayerList().find(nID);
	if (p != getMciPlayerList().end())
	{
		SAFE_DELETE(p->second);
		getMciPlayerList().erase(nID);
	}
}



unsigned int _Hash(tstring key)
{
	unsigned int len = unsigned(key.size());
	unsigned int hash = 0;

	for (unsigned i = 0; i < len; i++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)(unsigned char)toupper(key[i]);
	}
	return (hash);
}