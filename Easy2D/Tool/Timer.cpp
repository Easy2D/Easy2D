#include "..\easy2d.h"

// �������ж�ʱ��������
static std::vector<Timer*> s_vTimers;

Timer::Timer(TString name, UINT ms, const TIMER_CALLBACK & callback) :
	m_sName(name),
	m_bRunning(false),
	m_callback(callback),
	m_pParentScene(nullptr)
{
	setInterval(ms);			// ���ö�ʱ����ʱ����
}

Timer::~Timer()
{
}

void Timer::start()
{
	// ��־�ö�ʱ����������
	this->m_bRunning = true;
	// ��¼��ǰʱ��
	QueryPerformanceCounter(&m_nLast);
}

void Timer::stop()
{
	this->m_bRunning = false;	// ��־�ö�ʱ����ֹͣ
}

bool Timer::isRunning()
{
	return m_bRunning;			// ��ȡ�ö�ʱ��������״̬
}

void Timer::setInterval(UINT ms)
{
	// ���ö�ʱ����ʱ����
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	m_nAnimationInterval.QuadPart = (LONGLONG)(ms / 1000.0 * nFreq.QuadPart);
	// ����ʱ������ʱ��
	this->m_nMilliSeconds = ms;
}

void Timer::setCallback(const TIMER_CALLBACK & callback)
{
	m_callback = callback;		// ����ص�����
}

void Timer::setName(TString name)
{
	m_sName = name;				// �޸Ķ�ʱ������
}

UINT Timer::getInterval() const
{
	return m_nMilliSeconds;		// ��ȡ��ʱ����ʱ����
}

TString Timer::getName() const
{
	return m_sName;				// ��ȡ��ʱ��������
}

void Timer::__exec()
{
	// ��ʱ������Ϊ��
	if (!s_vTimers.size())
	{
		return;
	}
	// ��ȡ��ǰʱ��
	static LARGE_INTEGER nNow;
	QueryPerformanceCounter(&nNow);
	// ѭ���������еĶ�ʱ��
	for (auto timer : s_vTimers)
	{
		// ����ʱ��δ���У����������ʱ��
		if (!timer->m_bRunning) 
		{
			continue;
		}
		// �ж�ʱ�����Ƿ��㹻
		if (nNow.QuadPart - timer->m_nLast.QuadPart > timer->m_nAnimationInterval.QuadPart)
		{
			// ������ķ������¼�¼ʱ��
			timer->m_nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % timer->m_nAnimationInterval.QuadPart);
			// ���лص�����
			timer->m_callback();
		}
	}
}

void Timer::addTimer(Timer * timer)
{
	// ������ʱ��
	timer->start();
	timer->m_pParentScene = App::getCurrentScene();
	// ���ö�ʱ����������
	s_vTimers.push_back(timer);
}

void Timer::addTimer(TString name, UINT ms, const TIMER_CALLBACK & callback)
{
	// ������ʱ��
	auto timer = new Timer(name, ms, callback);
	// ��Ӷ�ʱ��
	addTimer(timer);
}

Timer * Timer::getTimer(TString name)
{
	// �����Ƿ�����ͬ���ƵĶ�ʱ��
	for (auto timer : s_vTimers)
	{
		if (timer->m_sName == name)
		{
			// ���ҵ������ظö�ʱ����ָ��
			return timer;
		}
	}
	// ��δ�ҵ������ؿ�ָ��
	return nullptr;
}

bool Timer::startTimer(TString name)
{
	// ����ָ�����ƵĶ�ʱ�������ҵ��ö�ʱ��
	auto t = getTimer(name);
	if (t)
	{
		// ������ʱ��
		t->start();
		return true;
	}
	// ��δ�ҵ�ͬ�����ƵĶ�ʱ�������� false
	return false;
}

bool Timer::stopTimer(TString name)
{
	// ָֹͣ�����ƵĶ�ʱ�������ҵ��ö�ʱ��
	auto t = getTimer(name);
	if (t)
	{
		// ֹͣ��ʱ��
		t->stop();
		return true;
	}
	// ��δ�ҵ�ͬ�����ƵĶ�ʱ�������� false
	return false;
}

bool Timer::delTimer(TString name)
{
	// ����������
	std::vector<Timer*>::iterator iter;
	// ѭ���������ж�ʱ��
	for (iter = s_vTimers.begin(); iter != s_vTimers.end(); iter++)
	{
		// ������ͬ���ƵĶ�ʱ��
		if ((*iter)->m_sName == name)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			s_vTimers.erase(iter);
			return true;
		}
	}
	// ��δ�ҵ�ͬ�����ƵĶ�ʱ�������� false
	return false;
}

void Timer::clearAllTimers()
{
	// ɾ�����ж�ʱ��
	for (auto t : s_vTimers)
	{
		delete t;
	}
	// �������
	s_vTimers.clear();
}

void Timer::bindTimersWithScene(Scene * scene)
{
	for (auto t : s_vTimers)
	{
		if (!t->m_pParentScene)
		{
			t->m_pParentScene = App::getCurrentScene();
		}
	}
}

void Timer::stopAllSceneTimers(Scene * scene)
{
	for (auto t : s_vTimers)
	{
		if (t->m_pParentScene == scene)
		{
			t->stop();
		}
	}
}
