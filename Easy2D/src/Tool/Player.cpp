#include <e2dtool.h>
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


bool easy2d::Player::preload(const String& filePath)
{
	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
	{
		return true;
	}
	else
	{
		Music * music = new (std::nothrow) Music();

		if (music->open(filePath))
		{
			music->setVolume(s_fMusicVolume);
			GetMusicFileList().insert(std::pair<size_t, Music *>(hash, music));
			return true;
		}
		else
		{
			delete music;
			music = nullptr;
		}
	}
	return false;
}

bool easy2d::Player::preload(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
	{
		return true;
	}
	else
	{
		Music * music = new (std::nothrow) Music();

		if (music->open(resNameId, resType))
		{
			music->setVolume(s_fMusicVolume);
			GetMusicResList().insert(std::pair<size_t, Music *>(resNameId, music));
			return true;
		}
		else
		{
			delete music;
			music = nullptr;
		}
	}
	return false;
}

bool easy2d::Player::play(const String& filePath, int nLoopCount)
{
	if (Player::preload(filePath))
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

bool easy2d::Player::play(int resNameId, const String& resType, int nLoopCount)
{
	if (Player::preload(resNameId, resType))
	{
		auto music = GetMusicResList()[resNameId];
		if (music->play(nLoopCount))
		{
			return true;
		}
	}
	return false;
}

void easy2d::Player::pause(const String& filePath)
{
	if (filePath.empty())
		return;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->pause();
}

void easy2d::Player::pause(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void easy2d::Player::resume(const String& filePath)
{
	if (filePath.empty())
		return;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->resume();
}

void easy2d::Player::resume(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->pause();
}

void easy2d::Player::stop(const String& filePath)
{
	if (filePath.empty())
		return;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		GetMusicFileList()[hash]->stop();
}

void easy2d::Player::stop(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		GetMusicResList()[resNameId]->stop();
}

bool easy2d::Player::isPlaying(const String& filePath)
{
	if (filePath.empty())
		return false;

	size_t hash = std::hash<String>{}(filePath);

	if (GetMusicFileList().end() != GetMusicFileList().find(hash))
		return GetMusicFileList()[hash]->isPlaying();

	return false;
}

bool easy2d::Player::isPlaying(int resNameId, const String& resType)
{
	if (GetMusicResList().end() != GetMusicResList().find(resNameId))
		return GetMusicResList()[resNameId]->isPlaying();
	return false;
}

double easy2d::Player::getVolume()
{
	return s_fMusicVolume;
}

void easy2d::Player::setVolume(double volume)
{
	s_fMusicVolume = min(max(float(volume), -224), 224);
	for (auto pair : GetMusicFileList())
	{
		pair.second->setVolume(s_fMusicVolume);
	}
}

void easy2d::Player::pauseAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->pause();
	}
}

void easy2d::Player::resumeAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->resume();
	}
}

void easy2d::Player::stopAll()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->stop();
	}
}

void easy2d::Player::__uninit()
{
	for (auto pair : GetMusicFileList())
	{
		pair.second->close();
		delete pair.second;
	}

	GetMusicFileList().clear();
}