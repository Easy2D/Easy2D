#include "easy2d.h"

Object::Object() :
	m_nRef(0)
{
	FreePool::__add(this);	// ���ö�������ͷų���
}

Object::~Object()
{
}

void Object::retain()
{
	m_nRef++;	// ���ü�����һ
}

void Object::release()
{
	m_nRef--;	// ���ü�����һ
}
