#include "..\ebase.h"
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
// ��һ֡�뵱ǰ֡��ʱ����
static int s_nInterval = 0;
// ��Ϸ��ʼʱ��
static float s_fTotalTime = 0;
// ÿһ֡���
static milliseconds s_tExceptedInvertal;


float e2d::ETime::getTotalTime()
{
	return s_fTotalTime;
}

int e2d::ETime::getDeltaTime()
{
	return s_nInterval;
}

bool e2d::ETime::__init()
{
	s_tStart = s_tLastUpdate = s_tFixedUpdate = s_tNow = steady_clock::now();
	s_tExceptedInvertal = milliseconds(17);
	return true;
}

void e2d::ETime::__uninit()
{
}

bool e2d::ETime::__isReady()
{
	return s_tExceptedInvertal < duration_cast<milliseconds>(s_tNow - s_tFixedUpdate);
}

void e2d::ETime::__updateNow()
{
	// ˢ��ʱ��
	s_tNow = steady_clock::now();
}

void e2d::ETime::__updateLast()
{
	s_tFixedUpdate += s_tExceptedInvertal;
	s_tLastUpdate = s_tNow;

	s_tNow = steady_clock::now();
	s_nInterval = static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tLastUpdate).count());
	s_fTotalTime = static_cast<float>(duration_cast<milliseconds>(s_tNow - s_tStart).count()) / 1000.0f;
}

void e2d::ETime::__sleep()
{
	// �������ʱ��
	int nWaitMS = 16 - static_cast<int>(duration_cast<milliseconds>(s_tNow - s_tFixedUpdate).count());
	
	if (nWaitMS > 1)
	{
		// �����̣߳��ͷ� CPU ռ��
		std::this_thread::sleep_for(milliseconds(nWaitMS));
	}
}