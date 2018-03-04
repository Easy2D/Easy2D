#include "..\emanagers.h"
#include "..\etools.h"
#include "..\enodes.h"

static std::vector<e2d::Timer*> s_vTimers;


void e2d::TimerManager::__update()
{
	if (s_vTimers.empty() || Game::isPaused())
		return;

	std::vector<Timer*>::iterator mIter;
	for (mIter = s_vTimers.begin(); mIter != s_vTimers.end();)
	{
		Timer * pTimer = (*mIter);
		// ���¶�ʱ��
		if (pTimer->isReady())
		{
			pTimer->update();
		}
		// �������Ҫ�Ķ�ʱ��
		if (pTimer->m_bClear)
		{
			pTimer->release();
			mIter = s_vTimers.erase(mIter);
		}
		else
		{
			mIter++;
		}
	}
}

void e2d::TimerManager::add(double timeOut, TimerCallback callback)
{
	auto pTimer = new Timer(callback, timeOut, 1, false, true);
	pTimer->start();
}

void e2d::TimerManager::add(Timer * pTimer)
{
	WARN_IF(pTimer == nullptr, "Timer NULL pointer exception!");

	if (pTimer)
	{
		auto findTimer = [](Timer * pTimer) -> bool
		{
			for (const auto &t : s_vTimers)
			{
				if (pTimer == t)
				{
					return true;
				}
			}
			return false;
		};

		bool bHasTimer = findTimer(pTimer);
		WARN_IF(bHasTimer, "The timer is already added, cannot be added again!");

		if (!bHasTimer)
		{
			pTimer->retain();
			s_vTimers.push_back(pTimer);
		}
	}
}

void e2d::TimerManager::start(const String & name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->start();
		}
	}
}

void e2d::TimerManager::stop(const String & name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->stop();
		}
	}
}

void e2d::TimerManager::stopAndClear(const String & name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			timer->stopAndClear();
		}
	}
}

e2d::Timer * e2d::TimerManager::get(const String & name)
{
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			return timer;
		}
	}
	return nullptr;
}

std::vector<e2d::Timer*> e2d::TimerManager::getTimers(const String & name)
{
	std::vector<Timer*> vTimers;
	for (auto timer : s_vTimers)
	{
		if (timer->getName() == name)
		{
			vTimers.push_back(timer);
		}
	}
	return std::move(vTimers);
}

void e2d::TimerManager::startAllTimers()
{
	for (auto timer : s_vTimers)
	{
		timer->start();
	}
}

void e2d::TimerManager::stopAllTimers()
{
	for (auto timer : s_vTimers)
	{
		timer->stop();
	}
}

void e2d::TimerManager::stopAndClearAllTimers()
{
	for (auto timer : s_vTimers)
	{
		timer->stop();
		timer->release();
	}
	s_vTimers.clear();
}

std::vector<e2d::Timer*> e2d::TimerManager::getAllTimers()
{
	return s_vTimers;
}

void e2d::TimerManager::__resetAllTimers()
{
	for (auto timer : s_vTimers)
	{
		timer->m_fLast = Time::getTotalTime();
	}
}

void e2d::TimerManager::__uninit()
{
	for (const auto timer : s_vTimers)
	{
		timer->release();
	}
	s_vTimers.clear();
}