#include <easy2d/e2dbase.h>
#include <thread>
#include <chrono>
using namespace std::chrono;


// 游戏开始时间
static steady_clock::time_point s_tStart;
// 当前时间
static steady_clock::time_point s_tNow;
// 上一帧刷新时间
static steady_clock::time_point s_tLast;
// 固定的刷新时间
static steady_clock::time_point s_tFixed;
// 每一帧间隔
static nanoseconds s_tExceptedInvertal;


float easy2d::Time::getTotalTime()
{
	return duration_cast<microseconds>(s_tNow - s_tStart).count() / 1000.f / 1000.f;
}

unsigned int easy2d::Time::getTotalTimeMilliseconds()
{
	return static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

float easy2d::Time::getDeltaTime()
{
	return duration_cast<microseconds>(s_tNow - s_tLast).count() / 1000.f / 1000.f;
}

unsigned int easy2d::Time::getDeltaTimeMilliseconds()
{
	return static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tLast).count());
}

void easy2d::Time::__init(int expectedFPS)
{
	if (expectedFPS > 0)
	{
		s_tExceptedInvertal = duration_cast<nanoseconds>(seconds(1)) / expectedFPS;
	}
	else
	{
		s_tExceptedInvertal = nanoseconds(0);
	}
	s_tStart = s_tFixed = s_tLast = s_tNow = steady_clock::now();
}

bool easy2d::Time::__isReady()
{
	return s_tExceptedInvertal < (s_tNow - s_tFixed);
}

void easy2d::Time::__updateNow()
{
	// 刷新时间
	s_tNow = steady_clock::now();
}

void easy2d::Time::__updateLast()
{
	s_tFixed += s_tExceptedInvertal;

	s_tLast = s_tNow;
	s_tNow = steady_clock::now();
}

void easy2d::Time::__reset()
{
	s_tLast = s_tFixed = s_tNow = steady_clock::now();
}

void easy2d::Time::__sleep()
{
	if (s_tExceptedInvertal.count())
	{
		// 计算挂起时长
		auto wait = duration_cast<nanoseconds>(s_tExceptedInvertal - (s_tNow - s_tFixed));
		if (wait > milliseconds(1))
		{
			// 挂起线程，释放 CPU 占用
			std::this_thread::sleep_for(wait);
		}
	}
}
