#include "..\e2dbase.h"
#include <thread>
#include <chrono>
using namespace std::chrono;


// ��Ϸ��ʼʱ��
static steady_clock::time_point s_tStart;
// ��ǰʱ��
static steady_clock::time_point s_tNow;
// ��һ֡ˢ��ʱ��
static steady_clock::time_point s_tLastUpdate;
// ÿһ֡���
static milliseconds s_tExceptedInvertal;

// ��Ϸ��ʼʱ��
static unsigned int s_nTotalTime = 0;


double e2d::Time::getTotalTime()
{
	return s_nTotalTime / 1000.0;
}

unsigned int e2d::Time::getTotalTimeMilliseconds()
{
	return s_nTotalTime;
}

double e2d::Time::getDeltaTime()
{
	return s_tExceptedInvertal.count() / 1000.0;
}

unsigned int e2d::Time::getDeltaTimeMilliseconds()
{
	return static_cast<unsigned int>(s_tExceptedInvertal.count());
}

bool e2d::Time::__init()
{
	s_tStart = s_tLastUpdate = s_tNow = steady_clock::now();
	s_tExceptedInvertal = milliseconds(15);
	return true;
}

bool e2d::Time::__isReady()
{
	return s_tExceptedInvertal < duration_cast<milliseconds>(s_tNow - s_tLastUpdate);
}

void e2d::Time::__updateNow()
{
	// ˢ��ʱ��
	s_tNow = steady_clock::now();
}

void e2d::Time::__updateLast()
{
	s_tLastUpdate += s_tExceptedInvertal;

	s_tNow = steady_clock::now();
	s_nTotalTime = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

void e2d::Time::__reset()
{
	s_tLastUpdate = s_tNow = steady_clock::now();
	s_nTotalTime = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

void e2d::Time::__sleep()
{
	// �������ʱ��
	int nWaitMS = 16 - static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tLastUpdate).count());
	
	if (nWaitMS > 1)
	{
		// �����̣߳��ͷ� CPU ռ��
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}
