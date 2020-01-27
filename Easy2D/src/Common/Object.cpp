#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>


easy2d::Object::Object()
	: _refCount(1)
{
	// 构造对象时，引用计数置 1
}

easy2d::Object::~Object()
{
}

void easy2d::Object::autorelease()
{
	GC::trace(this);
}

void easy2d::Object::retain()
{
	++_refCount;
}

void easy2d::Object::release()
{
	_refCount--;

	if (_refCount == 0)
	{
#ifdef E2D_DEBUG
		if (!GC::isClearing() && GC::isInPool(this))
		{
			// 不应存在引用计数为 0 且仍在 GC 池中的情况
			E2D_ERROR(L"释放引用计数为 0 的对象时其仍在 GC 池中");
		}
#endif
		delete this;
	}
}

int easy2d::Object::getRefCount() const
{
	return _refCount;
}