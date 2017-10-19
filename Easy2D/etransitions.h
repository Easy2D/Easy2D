#pragma once
#include "ebase.h"

namespace e2d
{

class ETransition :
	public EObject
{
	friend EApp;
public:
	ETransition() {}

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &enterNextScene,
		bool &transitional
	) = 0;
};


class ETransitionFade :
	public ETransition
{
public:
	// �������뵭��ʽ�ĳ����л�����
	ETransitionFade(
		float fadeOutDuration,	/* ǰһ����������������ʱ�� */
		float fadeInDuration	/* ��һ�������붯������ʱ�� */
	);

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &enterNextScene,
		bool &transitional
	) override;

protected:
	float m_fFadeOutDuration;
	float m_fFadeInDuration;
};

}