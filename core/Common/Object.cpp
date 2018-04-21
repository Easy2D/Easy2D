#include "..\ebase.h"
#include "..\emanager.h"

e2d::Object::Object()
	: m_nRefCount(0)
{
	ObjectManager::__add(this);
}

e2d::Object::~Object()
{
}

// ���ü�����һ
void e2d::Object::retain()
{
	m_nRefCount++;
}

// ���ü�����һ
void e2d::Object::release()
{
	m_nRefCount--;
	// ֪ͨ��������ˢ��
	ObjectManager::flush();
}

int e2d::Object::getRefCount() const
{
	return m_nRefCount;
}
