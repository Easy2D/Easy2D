#include "..\ebase.h"

// ��һ֡�뵱ǰ֡��ʱ����
static int s_nInterval = 0;
// ��Ϸ��ʼʱ��
static float s_fTotalTime = 0;


float e2d::ETime::getTotalTime()
{
	return s_fTotalTime;
}

int e2d::ETime::getDeltaTime()
{
	return s_nInterval;
}



#if _MSC_VER > 1600

#include <thread>
#include <chrono>
using namespace std::chrono;


// ��Ϸ��ʼʱ��
static steady_clock::time_point s_tStart;
// ��ǰʱ��
static steady_clock::time_point s_tNow;
// ��һ֡ˢ��ʱ��
static steady_clock::time_point s_tLast;


bool e2d::ETime::__init()
{
	s_tStart = s_tLast = s_tNow = steady_clock::now();
	return true;
}

void e2d::ETime::__uninit()
{
}

void e2d::ETime::__updateNow()
{
	s_tNow = steady_clock::now();
	s_fTotalTime = static_cast<float>(duration_cast<milliseconds>(s_tNow - s_tStart).count()) / 1000.0f;
	s_nInterval = static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tLast).count());
}

void e2d::ETime::__updateLast()
{
	s_tLast = s_tNow;
}

void e2d::ETime::__sleep()
{
	// �������ʱ��
	int nWaitMS = 16 - s_nInterval;
	// �����̣߳��ͷ� CPU ռ��
	if (nWaitMS > 1)
	{
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}


#else


#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// ʱ��Ƶ��
static LARGE_INTEGER s_tFreq;
// ��ǰʱ��
static LARGE_INTEGER s_tNow;
// ��Ϸ��ʼʱ��
static LARGE_INTEGER s_tStart;
// ��һ֡�������ʱ��
static LARGE_INTEGER s_tLast;



bool e2d::ETime::__init()
{
	bool bRet = false;
	if (::timeBeginPeriod(1))
	{
		// �޸�ʱ�侫��
		if (::QueryPerformanceFrequency(&s_tFreq))	// ��ȡʱ��Ƶ��
		{

			if (::QueryPerformanceCounter(&s_tNow))		// ˢ�µ�ǰʱ��
			{
				s_tStart = s_tLast = s_tNow;
				bRet = true;
			}
		}
	}
	return bRet;
}

void e2d::ETime::__uninit()
{
	::timeEndPeriod(1);	// ����ʱ�侫��
}

void e2d::ETime::__updateNow()
{
	::QueryPerformanceCounter(&s_tNow);
	s_fTotalTime = static_cast<float>(s_tNow.QuadPart - s_tStart.QuadPart) / s_tFreq.QuadPart;
	s_nInterval = static_cast<int>((s_tNow.QuadPart - s_tLast.QuadPart) * 1000LL / s_tFreq.QuadPart);
}

void e2d::ETime::__updateLast()
{
	s_tLast = s_tNow;
}

void e2d::ETime::__sleep()
{
	// �������ʱ��
	int nWaitMS = 16 - s_nInterval;
	// �����̣߳��ͷ� CPU ռ��
	if (nWaitMS > 1)
	{
		::Sleep(nWaitMS);
	}
}


#endif // _MSC_VER > 1600
