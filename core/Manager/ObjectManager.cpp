#include "..\emanagers.h"
#include "..\ebase.h"

// ObjectManager �ͷųص�ʵ�ֻ��ƣ�
// Object ���е����ü�����m_nRefCount����֤��ָ���ʹ�ð�ȫ
// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��ObjectManager ���Զ��ͷ��������
// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����
// �����Զ��ͷ�

// ��������
static std::vector<e2d::Object*> s_vObjectPool;
// ��־�ͷų�ִ��״̬
static bool s_bNotifyed = false;

void e2d::ObjectManager::__update()
{
	if (!s_bNotifyed) return;

	s_bNotifyed = false;
	// ����������
	static std::vector<e2d::Object*>::iterator iter;
	// ѭ�����������е����ж���
	for (iter = s_vObjectPool.begin(); iter != s_vObjectPool.end();)
	{
		if ((*iter)->getReferenceCount() <= 0)
		{
			// ����������õļ���С�ڵ��� 0, �ͷŸö���
			delete (*iter);
			// ���ͷų���ɾ���ö���
			iter = s_vObjectPool.erase(iter);
		}
		else
		{
			iter++;		// �ƶ�������
		}
	}
}

void e2d::ObjectManager::__clearAllObjects()
{
	for (const auto &obj : s_vObjectPool)
	{
		delete obj;
	}
	s_vObjectPool.clear();
}

void e2d::ObjectManager::add(e2d::Object * nptr)
{
	if (!nptr->m_bManaged)
	{
		nptr->m_bManaged = true;
		s_vObjectPool.push_back(nptr);	// ��һ����������ͷų���
	}
}

void e2d::ObjectManager::clear()
{
	s_bNotifyed = true;
}
