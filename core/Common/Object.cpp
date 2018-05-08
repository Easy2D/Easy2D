#include "..\e2dbase.h"
#include "..\e2dmanager.h"

e2d::Object::Object()
	: _nRefCount(0)
{
	ObjectManager::__add(this);
}

e2d::Object::~Object()
{
}

// ���ü�����һ
void e2d::Object::retain()
{
	_nRefCount++;
}

// ���ü�����һ
void e2d::Object::release()
{
	_nRefCount--;
	// ֪ͨ��������ˢ��
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return _nRefCount;
}
