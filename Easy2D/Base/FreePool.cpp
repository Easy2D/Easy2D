#include "..\easy2d.h"
#include <assert.h>

// FreePool �ͷųص�ʵ�ֻ��ƣ�
/// Object ���е����ü�����m_nRefCount����֤��ָ���ʹ�ð�ȫ
/// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��FreePool ���Զ��ͷ��������
/// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
/// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����
/// �����Զ��ͷ�

// �ͷų�����
static std::vector<Object*> s_vPool;

void FreePool::__flush()
{
	// ����������
	std::vector<Object*>::iterator iter;
	// ѭ�����������е����ж���
	for (iter = s_vPool.begin(); iter != s_vPool.end();)
	{
		// ����������õļ���Ϊ 0
		if ((*iter)->m_nRefCount == 0)
		{
			// �ͷŸö���
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

void FreePool::__add(Object * nptr)
{
	s_vPool.push_back(nptr);	// ��һ����������ͷų���
}

void FreePool::__clearAllObjects()
{
	for (auto o : s_vPool)
	{
		delete o;
	}
	s_vPool.clear();
}
