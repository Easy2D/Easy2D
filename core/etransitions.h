#pragma once
#include "ebase.h"

namespace e2d
{

class ETransition :
	public EObject
{
	friend EApp;

public:
	ETransition(float duration);

	// �����л������Ƿ����
	bool isEnding();

protected:
	// ���¶���
	virtual void _update() = 0;

	virtual void _init() = 0;

	virtual void _reset() = 0;

	virtual bool _isDelayEnough();

	virtual void _stop();

	// ���浱ǰ��������һ������ָ��
	void _setTarget(
		EScene * prev,
		EScene * next
	);

protected:
	bool m_bEnd;
	float m_fTotalDuration;
	float m_fDuration;
	float m_fRateOfProgress;
	EScene * m_pPrevScene;
	EScene * m_pNextScene;
	LARGE_INTEGER m_tLast;
	LARGE_INTEGER m_nAnimationInterval;
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
	// ���¶���
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;

protected:
	float m_fFadeOutDuration;
	float m_fFadeInDuration;
	bool m_bFadeOutTransioning;
};


class ETransitionEmerge :
	public ETransition
{
public:
	// ��������ʽ�ĳ����л�����
	ETransitionEmerge(
		float duration	/* ���ֶ�������ʱ�� */
	);

protected:
	// ���¶���
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;
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
		float moveDuration,			/* �����ƶ���������ʱ�� */
		MOVE_DIRECT direct = LEFT	/* �����ƶ����� */
	);

protected:
	// ���¶���
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;

protected:
	MOVE_DIRECT m_Direct;
	EVec m_Vec;
	EPoint m_NextPos;
};

}