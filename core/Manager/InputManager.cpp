#include "..\e2dmanager.h"
#include "..\e2dtool.h"

// ������
class Listener
{
public:
	Listener(
		e2d::Function func,
		const e2d::String& name,
		bool paused
	)
		: name(name)
		, callback(func)
		, running(!paused)
		, stopped(false)
	{
	}

	// ���¼�����״̬
	virtual void update()
	{
		if (callback)
		{
			callback();
		}
	}

public:
	bool running;
	bool stopped;
	e2d::String name;
	e2d::Function callback;
};

// ����������
static std::vector<Listener*> s_vListeners;


void e2d::InputManager::add(Function func, const String& name, bool paused)
{
	auto listener = new Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::InputManager::pause(const String& name)
{
	for (auto pListener : s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->running = false;
		}
	}
}

void e2d::InputManager::resume(const String& name)
{
	for (auto pListener : s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->running = true;
		}
	}
}

void e2d::InputManager::stop(const String& name)
{
	for (auto pListener : s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->stopped = true;
		}
	}
}

void e2d::InputManager::pauseAll()
{
	for (auto pListener : s_vListeners)
	{
		pListener->running = false;
	}
}

void e2d::InputManager::resumeAll()
{
	for (auto pListener : s_vListeners)
	{
		pListener->running = true;
	}
}

void e2d::InputManager::stopAll()
{
	for (auto pListener : s_vListeners)
	{
		pListener->stopped = true;
	}
}

void e2d::InputManager::__update()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto pListener = s_vListeners[i];
		// �����ֹͣ�ļ�����
		if (pListener->stopped)
		{
			delete pListener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ���¼�����
			pListener->update();
			++i;
		}
	}
}

void e2d::InputManager::__uninit()
{
	for (auto listener : s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}