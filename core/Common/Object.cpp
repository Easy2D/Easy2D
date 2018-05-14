#include "..\e2dbase.h"
#include "..\e2dmanager.h"

e2d::Object::Object()
	: _refCount(0)
{
	ObjectManager::__add(this);
}

e2d::Object::~Object()
{
}

// ���ü�����һ
void e2d::Object::retain()
{
	++_refCount;
}

// ���ü�����һ
void e2d::Object::release()
{
	_refCount--;
	// ֪ͨ��������ˢ��
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return _refCount;
}
