#include "..\easy2d.h"
#include <assert.h>

// FreePool �ͷųص�ʵ�ֻ��ƣ�
/// Object ���е����ü�����m_nRef����֤��ָ���ʹ�ð�ȫ
/// ����¼�˶���ʹ�õĴ�����������Ϊ 0 ʱ��FreePool ���Զ��ͷ��������
/// ���е� Object ����Ӧ�ڱ�ʹ��ʱ������ Text ��ӵ��˳����У�
/// ���� retain ������֤�ö��󲻱�ɾ�������ڲ���ʹ��ʱ���� release ����
/// �����Զ��ͷ�

// �ͷų�����
static std::vector<Object*> pool;

void FreePool::__flush()
{
	// ����������
	std::vector<Object*>::iterator iter;
	// ѭ�����������е����ж���
	for (iter = pool.begin(); iter != pool.end();)
	{
		// ����������õļ���Ϊ 0
		if ((*iter)->m_nRef == 0)
		{
			// �ͷŸö���
			delete (*iter);
			// ���ͷų���ɾ���ö���
			iter = pool.erase(iter);
		}
		else
		{
			iter++;		// �ƶ�������
		}
	}
}

void FreePool::__add(Object * nptr)
{
#ifdef _DEBUG
	for (auto o : pool)
	{
		assert(o != nptr);	// �������ظ���ָ�����
	}
#endif

	pool.push_back(nptr);		// ��һ����������ͷų���
}
