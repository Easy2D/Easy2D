#pragma once
#include "ebase.h"

namespace e2d
{

// ���������
class EObjectManager
{
	friend EApp;

public:
	// ��һ���ڵ�����ڴ��
	static void add(
		e2d::EObject * nptr
	);

	// ɾ�����нڵ�
	static void clearAllObjects();

	// ֪ͨ�ڴ��ˢ��
	static void notifyFlush();

private:
	// ˢ���ڴ��
	static void __flush();
};


// ��ʱ��
class ETimer :
	public EObject
{

};


// ��ʱ��������
class ETimerManager
{

};

}