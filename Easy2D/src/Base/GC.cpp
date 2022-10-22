#include <easy2d/e2dbase.h>
#include <vector>

//
// gcnew helper
//
easy2d::__gc_helper::GCNewHelper easy2d::__gc_helper::GCNewHelper::instance;

// GC �ͷųص�ʵ�ֻ��ƣ�
// Object ���е����ü�����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��GC ���Զ��ͷ��������
// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����

namespace
{
	std::vector<easy2d::Object*> s_vObjectPool;
	bool s_bClearing = false;
}

bool easy2d::GC::isInPool(Object* pObject)
{
	if (pObject)
	{
		for (const auto& pObj : s_vObjectPool)
		{
			if (pObj == pObject)
				return true;
		}
	}
	return false;
}

bool easy2d::GC::isClearing()
{
	return s_bClearing;
}

void easy2d::GC::clear()
{
	std::vector<Object*> releaseThings;
	releaseThings.swap(s_vObjectPool);

	s_bClearing = true;
	for (auto pObj : releaseThings)
	{
		pObj->release();
	}
	s_bClearing = false;
}

void easy2d::GC::trace(easy2d::Object * pObject)
{
	if (pObject)
	{
		s_vObjectPool.push_back(pObject);
	}
}
