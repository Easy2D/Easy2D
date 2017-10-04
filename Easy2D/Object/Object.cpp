#include "..\easy2d.h"

Object::Object() :
	m_nRefCount(0)
{
}

Object::~Object()
{
}

void Object::retain()
{
	m_nRefCount++;	// ���ü�����һ
}

void Object::release()
{
	m_nRefCount--;	// ���ü�����һ
}

void Object::autoRelease()
{
	FreePool::__add(this);	// ���ö�������ͷų���
}
