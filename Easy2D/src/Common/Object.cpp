#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>


easy2d::Object::Object()
	: _refCount(1)
{
	// �������ʱ�����ü����� 1
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
			// ��Ӧ�������ü���Ϊ 0 ������ GC ���е����
			E2D_ERROR(L"�ͷ����ü���Ϊ 0 �Ķ���ʱ������ GC ����");
		}
#endif
		delete this;
	}
}

int easy2d::Object::getRefCount() const
{
	return _refCount;
}