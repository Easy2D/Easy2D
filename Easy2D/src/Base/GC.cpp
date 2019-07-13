#include <e2dbase.h>

// GC �ͷųص�ʵ�ֻ��ƣ�
// Object ���е����ü�����_refCount����һ���̶��Ϸ�ֹ���ڴ�й©
// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��GC ���Զ��ͷ��������
// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����

// ��������
static std::set<easy2d::Object*> s_vObjectPool;
// ��־�ͷų�ִ��״̬
static bool s_bNotifyed = false;

void easy2d::GC::__update()
{
	if (!s_bNotifyed) return;

	s_bNotifyed = false;
	for (auto iter = s_vObjectPool.begin(); iter != s_vObjectPool.end();)
	{
		if ((*iter)->getRefCount() <= 0)
		{
			(*iter)->onDestroy();
			delete (*iter);
			iter = s_vObjectPool.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void easy2d::GC::__clear()
{
	for (auto pObj : s_vObjectPool)
	{
		delete pObj;
	}
	s_vObjectPool.clear();
}

void easy2d::GC::__add(easy2d::Object * pObject)
{
	if (pObject)
	{
		s_vObjectPool.insert(pObject);
	}
}

void easy2d::GC::notify()
{
	s_bNotifyed = true;
}

void easy2d::GC::flush()
{
	GC::notify();
	GC::__update();
}
