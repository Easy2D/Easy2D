#include <easy2d/e2dbase.h>

//
// gcnew helper
//
easy2d::__gc_helper::GCNewHelper easy2d::__gc_helper::GCNewHelper::instance;

// GC 释放池的实现机制：
// Object 类中的引用计数（_refCount）在一定程度上防止了内存泄漏
// 它记录了对象被使用的次数，当计数为 0 时，GC 会自动释放这个对象
// 所有的 Object 对象都应在被使用时（例如 Text 添加到了场景中）
// 调用 retain 函数保证该对象不被删除，并在不再使用时调用 release 函数

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
