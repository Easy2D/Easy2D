#include "..\easy2d.h"

Object::Object() :
	m_nRefCount(0),
	m_bAutoRelease(false)
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
	if (!m_bAutoRelease)
	{
		m_bAutoRelease = true;
		FreePool::__add(this);	// ���ö�������ͷų���
	}
}
