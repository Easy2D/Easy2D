#include "..\e2dmanager.h"
#include "..\e2dbase.h"

// ObjectManager �ͷųص�ʵ�ֻ��ƣ�
// Object ���е����ü�����_nRefCount����һ���̶��ϱ�֤��ָ���ʹ�ð�ȫ
// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��ObjectManager ���Զ��ͷ��������
// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����

// ��������
static std::set<e2d::Object*> s_vObjectPool;
// ��־�ͷų�ִ��״̬
static bool s_bNotifyed = false;

void e2d::ObjectManager::__update()
{
	if (!s_bNotifyed) return;

	s_bNotifyed = false;
	for (auto iter = s_vObjectPool.begin(); iter != s_vObjectPool.end();)
	{
		if ((*iter)->getRefCount() <= 0)
		{
			(*iter)->onDestroy();
			delete (*iter);
			iter = s_vObjectPool.erase(iter);
		}
		else
		{
			iter++;		// �ƶ�������
		}
	}
}

void e2d::ObjectManager::__clear()
{
	for (auto pObj : s_vObjectPool)
	{
		delete pObj;
	}
	s_vObjectPool.clear();
}

void e2d::ObjectManager::__add(e2d::Object * pObject)
{
	if (pObject)
	{
		s_vObjectPool.insert(pObject);
	}
}

void e2d::ObjectManager::flush()
{
	s_bNotifyed = true;
}
