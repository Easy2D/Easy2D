#include "..\ebase.h"
#include "..\emanagers.h"

e2d::Obj::Obj()
	: m_nRefCount(0)
	, m_bManaged(false)
{
	ObjectManager::preload(this);	// ���ö�������ͷų���
}

e2d::Obj::~Obj()
{
}

// ���ü�����һ
void e2d::Obj::retain()
{
	m_nRefCount++;
}

// ���ü�����һ
void e2d::Obj::release()
{
	m_nRefCount--;
	// ֪ͨ��������ˢ��
	ObjectManager::notifyFlush();
}
