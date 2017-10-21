#pragma once
#include "ebase.h"

namespace e2d
{

class ETransition :
	public EObject
{
	friend EApp;
public:
	ETransition() { this->autoRelease(); }

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
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
		bool &transitional
	) override;

protected:
	float m_fFadeOutDuration;
	float m_fFadeInDuration;
};


class ETransitionEmerge :
	public ETransition
{
public:
	// ��������ʽ�ĳ����л�����
	ETransitionEmerge(
		float emergeDuration	/* ���ֶ�������ʱ�� */
	);

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fEmergeDuration;
};


class ETransitionMove :
	public ETransition
{
public:
	enum MOVE_DIRECT
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	// �����ƶ�ʽ�ĳ����л�����
	ETransitionMove(
		float moveDuration,	/* �����ƶ���������ʱ�� */
		MOVE_DIRECT direct	/* �����ƶ����� */
	);

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fMoveDuration;
	MOVE_DIRECT m_Direct;
};


class ETransitionScale :
	public ETransition
{
public:
	// ��������ʽ�ĳ����л�����
	ETransitionScale(
		float scaleOutDuration,	/* ��һ���������Ŷ�������ʱ�� */
		float scaleInDuration	/* �ڶ����������Ŷ�������ʱ�� */
	);

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fScaleOutDuration;
	float m_fScaleInDuration;
};


class ETransitionScaleEmerge :
	public ETransition
{
public:
	enum SCALE_EMERGE_MODE
	{
		ENTER,
		BACK
	};

	// �������Ÿ���ʽ�ĳ����л�����
	ETransitionScaleEmerge(
		float duration,			/* ������������ʱ�� */
		SCALE_EMERGE_MODE mode	/* �����ƶ����� */
	);

protected:
	// ���浱ǰ��������һ������ָ�룬�Ϳ��Ƴ����л��ı���
	virtual void _setTarget(
		EScene * prev,
		EScene * next,
		bool &transitional
	) override;

protected:
	float m_fDuration;
	SCALE_EMERGE_MODE m_Mode;
};

}