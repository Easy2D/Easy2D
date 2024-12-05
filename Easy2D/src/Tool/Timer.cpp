#include <easy2d/e2dtool.h>
#include <easy2d/e2dnode.h>
#include <atomic>
#include <map>

namespace easy2d
{
	class TimerEntity
		: public Object
	{
	public:
		TimerEntity(
			const easy2d::Function<void()>& func,
			const easy2d::String& name,
			float interval,
			int updateTimes
		)
			: running(true)
			, removed(false)
			, runTimes(0)
			, totalTimes(updateTimes)
			, interval(max(interval, 0))
			, lastTime(easy2d::Time::getTotalTime())
			, callback(func)
			, name(name)
			, node(nullptr)
		{
		}

		void update()
		{
			if (callback)
			{
				callback();
			}

			++runTimes;
			lastTime += interval;

			if (runTimes == totalTimes)
			{
				removed = true;
			}
		}

		bool isReady() const
		{
			if (this->running)
			{
				if (this->interval == 0)
					return true;

				if ((easy2d::Time::getTotalTime() - this->lastTime) >= this->interval)
					return true;
			}
			return false;
		}

	public:
		bool	running;
		bool	removed;
		int		runTimes;
		int		totalTimes;
		float	interval;
		float	lastTime;
		Node*	node;
		easy2d::String name;
		easy2d::Function<void()> callback;
	};
}

static std::atomic<size_t> s_vTimerId = 0;
static std::map<size_t, easy2d::TimerEntity*> s_vTimers;


size_t easy2d::Timer::add(const Function<void()>& func, float interval, int updateTimes, const String& name)
{
	auto timer = gcnew TimerEntity(func, name, interval, updateTimes);
	GC::retain(timer);

	const auto id = s_vTimerId++;
	s_vTimers.insert(std::make_pair(id, timer));
	return id;
}

size_t easy2d::Timer::runDelayed(float delay, const Function<void()>& func)
{
	return Timer::add(func, delay, 1, "");
}

void easy2d::Timer::bind(size_t id, Node* node)
{
	auto iter = s_vTimers.find(id);
	if (iter != s_vTimers.end())
	{
		iter->second->node = node;
	}
}

void easy2d::Timer::removeBoundWith(Node* node)
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		if (timer->node == node)
		{
			timer->removed = true;
		}
	}
}

void easy2d::Timer::start(size_t id)
{
	auto iter = s_vTimers.find(id);
	if (iter != s_vTimers.end())
	{
		iter->second->running = true;
	}
}

void easy2d::Timer::stop(size_t id)
{
	auto iter = s_vTimers.find(id);
	if (iter != s_vTimers.end())
	{
		iter->second->running = false;
	}
}

void easy2d::Timer::remove(size_t id)
{
	auto iter = s_vTimers.find(id);
	if (iter != s_vTimers.end())
	{
		iter->second->removed = true;
	}
}

void easy2d::Timer::start(const String& name)
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		if (timer->name == name)
		{
			timer->running = true;
		}
	}
}

void easy2d::Timer::stop(const String& name)
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		if (timer->name == name)
		{
			timer->running = false;
		}
	}
}

void easy2d::Timer::remove(const String& name)
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		if (timer->name == name)
		{
			timer->removed = true;
		}
	}
}

void easy2d::Timer::stopAll()
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		timer->running = false;
	}
}

void easy2d::Timer::startAll()
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		timer->running = true;
	}
}

void easy2d::Timer::removeAll()
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		timer->removed = true;
	}
}

void easy2d::Timer::__update()
{
	if (s_vTimers.empty() || Game::isPaused())
		return;

	for (auto iter = s_vTimers.begin(); iter != s_vTimers.end();)
	{
		auto timer = iter->second;
		// 清除已停止的定时器
		if (timer->removed)
		{
			GC::release(timer);
			iter = s_vTimers.erase(iter);
		}
		else
		{
			// 更新定时器
			if (timer->isReady())
			{
				timer->update();
			}
			++iter;
		}
	}
}

void easy2d::Timer::__resetAll()
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		timer->lastTime = Time::getTotalTime();
	}
}

void easy2d::Timer::__uninit()
{
	for (const auto& pair : s_vTimers)
	{
		auto timer = pair.second;
		GC::release(timer);
	}
	s_vTimers.clear();
}
