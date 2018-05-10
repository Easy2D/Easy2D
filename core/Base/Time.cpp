#include "..\e2dbase.h"
#include <thread>
#include <chrono>
using namespace std::chrono;


// ��Ϸ��ʼʱ��
static steady_clock::time_point s_tStart;
// ��ǰʱ��
static steady_clock::time_point s_tNow;
// ��һ֡ˢ��ʱ��
static steady_clock::time_point s_tFixedUpdate;
// ��һ�θ���ʱ��
static steady_clock::time_point s_tLastUpdate;
// ÿһ֡���
static milliseconds s_tExceptedInvertal;

// ��һ֡�뵱ǰ֡��ʱ����
static unsigned int s_nInterval = 0;
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
	return s_nInterval / 1000.0;
}

unsigned int e2d::Time::getDeltaTimeMilliseconds()
{
	return s_nInterval;
}

bool e2d::Time::__init()
{
	s_tStart = s_tLastUpdate = s_tFixedUpdate = s_tNow = steady_clock::now();
	s_tExceptedInvertal = milliseconds(15);
	return true;
}

void e2d::Time::__uninit()
{
}

bool e2d::Time::__isReady()
{
	return s_tExceptedInvertal < duration_cast<milliseconds>(s_tNow - s_tFixedUpdate);
}

void e2d::Time::__updateNow()
{
	// ˢ��ʱ��
	s_tNow = steady_clock::now();
}

void e2d::Time::__updateLast()
{
	s_tFixedUpdate += s_tExceptedInvertal;
	s_tLastUpdate = s_tNow;

	s_tNow = steady_clock::now();
	s_nInterval = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tLastUpdate).count());
	s_nTotalTime = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

void e2d::Time::__reset()
{
	s_tLastUpdate = s_tFixedUpdate = s_tNow = steady_clock::now();
	s_nInterval = 0;
	s_nTotalTime = static_cast<unsigned int>(duration_cast<milliseconds>(s_tNow - s_tStart).count());
}

void e2d::Time::__sleep()
{
	// �������ʱ��
	int nWaitMS = 16 - static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tFixedUpdate).count());
	
	if (nWaitMS > 1)
	{
		// �����̣߳��ͷ� CPU ռ��
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}
