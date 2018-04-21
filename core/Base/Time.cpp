#include "..\e2dbase.h"

// ��һ֡�뵱ǰ֡��ʱ����
static int s_nInterval = 0;
// ��Ϸ��ʼʱ��
static double s_fTotalTime = 0;


double e2d::Time::getTotalTime()
{
	return s_fTotalTime;
}

int e2d::Time::getDeltaTime()
{
	return s_nInterval;
}


#if HIGHER_THAN_VS2010

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
	s_nInterval = static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tLastUpdate).count());
	s_fTotalTime = static_cast<double>(duration_cast<milliseconds>(s_tNow - s_tStart).count()) / 1000.0;
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


#else


#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// ʱ��Ƶ��
static LARGE_INTEGER s_tFreq;
// ��Ϸ��ʼʱ��
static LARGE_INTEGER s_tStart;
// ��ǰʱ��
static LARGE_INTEGER s_tNow;
// ��һ֡ˢ��ʱ��
static LARGE_INTEGER s_tFixedUpdate;
// ��һ�θ���ʱ��
static LARGE_INTEGER s_tLastUpdate;
// ÿһ֡���
static LONGLONG s_tExceptedInvertal;


bool e2d::Time::__init()
{
	::timeBeginPeriod(1);					// �޸�ʱ�侫��
	::QueryPerformanceFrequency(&s_tFreq);	// ��ȡʱ��Ƶ��
	::QueryPerformanceCounter(&s_tNow);		// ˢ�µ�ǰʱ��
	s_tStart = s_tLastUpdate = s_tFixedUpdate = s_tNow;
	s_tExceptedInvertal = 15LL * s_tFreq.QuadPart / 1000LL;
	return true;
}

void e2d::Time::__uninit()
{
	::timeEndPeriod(1);	// ����ʱ�侫��
}

bool e2d::Time::__isReady()
{
	return s_tExceptedInvertal < (s_tNow.QuadPart - s_tFixedUpdate.QuadPart);
}

void e2d::Time::__updateNow()
{
	::QueryPerformanceCounter(&s_tNow);
}

void e2d::Time::__updateLast()
{
	s_tFixedUpdate.QuadPart += s_tExceptedInvertal;
	s_tLastUpdate = s_tNow;

	::QueryPerformanceCounter(&s_tNow);
	s_nInterval = static_cast<int>((s_tNow.QuadPart - s_tLastUpdate.QuadPart) * 1000LL / s_tFreq.QuadPart);
	s_fTotalTime = static_cast<double>(s_tNow.QuadPart - s_tStart.QuadPart) / s_tFreq.QuadPart;
}

void e2d::Time::__sleep()
{
	// �������ʱ��
	int nWaitMS = 16 - static_cast<int>((s_tNow.QuadPart - s_tFixedUpdate.QuadPart) * 1000LL / s_tFreq.QuadPart);
	// �����̣߳��ͷ� CPU ռ��
	if (nWaitMS > 1)
	{
		::Sleep(nWaitMS);
	}
}


#endif