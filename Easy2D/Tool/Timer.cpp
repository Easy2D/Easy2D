#include "..\easy2d.h"
#include "..\Win\winbase.h"

// �������ж�ʱ��������
static std::vector<Timer*> s_vTimers;

Timer::Timer(TString name, LONGLONG milliSeconds, const TIMER_CALLBACK & callback) :
	m_sName(name),
	m_bRunning(false),
	m_bWaiting(false),
	m_callback(callback),
	m_pParentScene(nullptr)
{
	setInterval(milliSeconds);		// ���ö�ʱ����ʱ����
}

Timer::~Timer()
{
}

void Timer::start()
{
	// ��־�ö�ʱ����������
	m_bRunning = true;
	// ��¼��ǰʱ��
	m_nLast = steady_clock::now();
}

void Timer::stop()
{
	m_bRunning = false;	// ��־�ö�ʱ����ֹͣ
}

void Timer::wait()
{
	m_bWaiting = true;
}

void Timer::notify()
{
	m_bWaiting = false;
}

bool Timer::isRunning()
{
	return m_bRunning && !m_bWaiting;			// ��ȡ�ö�ʱ��������״̬
}

void Timer::setInterval(LONGLONG milliSeconds)
{
	// ���ö�ʱ����ʱ����
	m_nAnimationInterval = milliSeconds;
}

void Timer::setCallback(const TIMER_CALLBACK & callback)
{
	m_callback = callback;		// ����ص�����
}

void Timer::setName(TString name)
{
	m_sName = name;				// �޸Ķ�ʱ������
}

LONGLONG Timer::getInterval() const
{
	return m_nAnimationInterval;// ��ȡ��ʱ����ʱ����
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
	// ѭ���������еĶ�ʱ��
	for (auto timer : s_vTimers)
	{
		// ����ʱ��δ���У����������ʱ��
		if (!timer->isRunning()) 
		{
			continue;
		}
		// �ж�ʱ�����Ƿ��㹻
		while (duration_cast<milliseconds>(GetNow() - timer->m_nLast).count() > timer->m_nAnimationInterval)
		{
			// ���¼�¼ʱ��
			timer->m_nLast += milliseconds(timer->m_nAnimationInterval);
			// ���лص�����
			timer->m_callback();
		}
	}
}

void Timer::addTimer(Timer * timer)
{
	// ������ʱ��
	timer->start();
	// ���ڳ�����
	timer->m_pParentScene = EApp::getLoadingScene();
	// ���ö�ʱ����������
	s_vTimers.push_back(timer);
}

void Timer::addTimer(TString name, const TIMER_CALLBACK & callback)
{
	addTimer(name, 20, callback);
}

void Timer::addTimer(TString name, LONGLONG milliSeconds, const TIMER_CALLBACK & callback)
{
	// ������ʱ��
	auto timer = new Timer(name, milliSeconds, callback);
	// ��Ӷ�ʱ��
	addTimer(timer);
}

void Timer::startTimer(TString name)
{
	// ����������ͬ�Ķ�ʱ��
	for (auto timer : s_vTimers)
	{
		if (timer->m_sName == name && timer->m_pParentScene == EApp::getCurrentScene())
		{
			// ������ʱ��
			timer->start();
		}
	}
}

void Timer::stopTimer(TString name)
{
	// ����������ͬ�Ķ�ʱ��
	for (auto timer : s_vTimers)
	{
		if (timer->m_sName == name && timer->m_pParentScene == EApp::getCurrentScene())
		{
			// ֹͣ��ʱ��
			timer->stop();
		}
	}
}

void Timer::delTimer(TString name)
{
	// ����������
	std::vector<Timer*>::iterator iter;
	// ѭ���������ж�ʱ��
	for (iter = s_vTimers.begin(); iter != s_vTimers.end();)
	{
		// ������ͬ���ƵĶ�ʱ��
		if ((*iter)->m_sName == name && (*iter)->m_pParentScene == EApp::getCurrentScene())
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			iter = s_vTimers.erase(iter);
		}
		else
		{
			iter++;
		}
	}
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

void Timer::notifyAllSceneTimers(Scene * scene)
{
	for (auto t : s_vTimers)
	{
		if (t->m_pParentScene == scene)
		{
			t->notify();
		}
	}
}

void Timer::waitAllSceneTimers(Scene * scene)
{
	for (auto t : s_vTimers)
	{
		if (t->m_pParentScene == scene)
		{
			t->wait();
		}
	}
}

void Timer::clearAllSceneTimers(Scene * scene)
{
	// ����������
	std::vector<Timer*>::iterator iter;
	// ѭ���������ж�ʱ��
	for (iter = s_vTimers.begin(); iter != s_vTimers.end();)
	{
		// ������ͬ���ƵĶ�ʱ��
		if ((*iter)->m_pParentScene == scene)
		{
			// ɾ���ö�ʱ��
			delete (*iter);
			iter = s_vTimers.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
