#include <easy2d/e2dtool.h>
#include <map>

typedef std::map<size_t, easy2d::Music *> MusicMap;

static MusicMap& GetMusicFileList()
{
	static MusicMap s_MusicFileList;
	return s_MusicFileList;
}

static MusicMap& GetMusicResList()
{
	static MusicMap s_MusicResList;
	return s_MusicResList;
}

static float s_fMusicVolume = 1.0;


bool easy2d::MusicPlayer::preload(const String& filePath)
{
	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
	{
		return true;
	}
	
	Music* music = gcnew Music;

	if (music->open(filePath))
	{
		GC::retain(music);

		music->setVolume(s_fMusicVolume);
		GetMusicFileList().insert(std::pair<size_t, Music*>(hash, music));
		return true;
	}
	return false;
}

bool easy2d::MusicPlayer::play(const String& filePath, int nLoopCount)
{
	if (MusicPlayer::preload(filePath))
	{
		size_t hash = std::hash<String>{}(filePath);
		auto music = GetMusicFileList()[hash];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void easy2d::MusicPlayer::pause(const String& filePath)
{
	if (filePath.empty())
		return;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->pause();
}

void easy2d::MusicPlayer::resume(const String& filePath)
{
	if (filePath.empty())
		return;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->resume();
}

void easy2d::MusicPlayer::stop(const String& filePath)
{
	if (filePath.empty())
		return;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->stop();
}

bool easy2d::MusicPlayer::isPlaying(const String& filePath)
{
	if (filePath.empty())
		return false;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		return GetMusicFileList()[hash]->isPlaying();

	return false;
}

bool easy2d::MusicPlayer::preload(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
	{
		return true;
	}
	
	Music* music = gcnew Music;

	if (music->open(resNameId, resType))
	{
		GC::retain(music);

		music->setVolume(s_fMusicVolume);
		GetMusicResList().insert(std::pair<size_t, Music*>(resNameId, music));
		return true;
	}
	return false;
}

bool easy2d::MusicPlayer::play(int resNameId, const String& resType, int nLoopCount)
{
	if (MusicPlayer::preload(resNameId, resType))
	{
		auto music = GetMusicResList()[resNameId];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void easy2d::MusicPlayer::pause(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void easy2d::MusicPlayer::resume(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void easy2d::MusicPlayer::stop(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->stop();
}

bool easy2d::MusicPlayer::isPlaying(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		return GetMusicResList()[resNameId]->isPlaying();
	return false;
}

float easy2d::MusicPlayer::getVolume()
{
	return s_fMusicVolume;
}

void easy2d::MusicPlayer::setVolume(float volume)
{
	s_fMusicVolume = min(max(float(volume), -224), 224);
	for (auto pair : GetMusicFileList())
	{
		pair.second->setVolume(s_fMusicVolume);
	}
}

void easy2d::MusicPlayer::pauseAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->pause();
	}
}

void easy2d::MusicPlayer::resumeAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->resume();
	}
}

void easy2d::MusicPlayer::stopAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->stop();
	}
}

void easy2d::MusicPlayer::__uninit()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->close();
		GC::release(pair.second);
	}

	for (auto pair : GetMusicResList())
	{
		pair.second->close();
		GC::release(pair.second);
	}

	GetMusicFileList().clear();
	GetMusicResList().clear();
}
