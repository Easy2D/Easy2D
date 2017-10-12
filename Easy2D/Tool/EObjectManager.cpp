#include "..\etools.h"
#include <vector>

// EObjectManager �ͷųص�ʵ�ֻ��ƣ�
/// EObject ���е����ü�����m_nRefCount����֤��ָ���ʹ�ð�ȫ
/// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��EObjectManager ���Զ��ͷ��������
/// ���е� EObject ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
/// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����
/// �����Զ��ͷ�

// �ͷų�����
static std::vector<e2d::EObject*> s_vPool;

void e2d::EObjectManager::__flush()
{
	// ����������
	static std::vector<e2d::EObject*>::iterator iter;
	// ѭ�����������е����ж���
	for (iter = s_vPool.begin(); iter != s_vPool.end();)
	{
		if ((*iter)->m_bAutoRelease && (*iter)->m_nRefCount == 0)
		{
			// ����������õļ���Ϊ 0, �ͷŸö���
			delete (*iter);
			// ���ͷų���ɾ���ö���
			iter = s_vPool.erase(iter);
		}
		else
		{
			iter++;		// �ƶ�������
		}
	}
}

void e2d::EObjectManager::add(e2d::EObject * nptr)
{
	if (!nptr->m_bManaged)
	{
		nptr->m_bManaged = true;
		s_vPool.push_back(nptr);	// ��һ����������ͷų���
	}
}

void e2d::EObjectManager::clearAllObjects()
{
	for (auto o : s_vPool)
	{
		delete o;
	}
	s_vPool.clear();
}
