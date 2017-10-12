#pragma once
#include "ebase.h"

namespace e2d
{

class EObjectManager
{
public:
	// ��һ���ڵ�����ͷų�
	static void add(e2d::EObject * nptr);

	// ɾ�����нڵ�
	static void clearAllObjects();

private:
	friend EApp;

	// ˢ���ڴ��
	static void __flush();
};

}