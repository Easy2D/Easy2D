#include <e2dbase.h>

// GC �ͷųص�ʵ�ֻ��ƣ�
// Object ���е����ü�����_refCount����һ���̶��Ϸ�ֹ���ڴ�й©
// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��GC ���Զ��ͷ��������
// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����

namespace
{
	std::set<easy2d::Object*> s_vObjectPool;
	bool s_bNotifyed = false;
}

void easy2d::GC::flush()
{
	if (!s_bNotifyed) return;

	s_bNotifyed = false;
	for (auto iter = s_vObjectPool.begin(); iter != s_vObjectPool.end();)
	{
		if ((*iter)->getRefCount() <= 0)
		{
			delete (*iter);
			iter = s_vObjectPool.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void easy2d::GC::clear()
{
	std::set<Object*> releaseThings;
	releaseThings.swap(s_vObjectPool);

	for (auto pObj : releaseThings)
	{
		pObj->release();
	}
}

void easy2d::GC::trace(easy2d::Object * pObject)
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
