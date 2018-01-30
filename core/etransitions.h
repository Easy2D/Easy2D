#pragma once
#include "ecommon.h"

namespace e2d
{


class ESceneManager;

class ETransition :
	public EObject
{
	friend ESceneManager;

public:
	ETransition(float duration);

	// �����л������Ƿ����
	bool isEnding();

protected:
	// ���³�������
	virtual void _update() = 0;

	// ��ʼ����������
	virtual void _init() = 0;

	// ���ó�������
	virtual void _reset() = 0;

	// ֹͣ��������
	virtual void _stop();

	// ���㳡����������
	void _calcRateOfProgress();

	// ���浱ǰ��������һ������ָ��
	void _setTarget(
		EScene * prev,
		EScene * next
	);

protected:
	bool m_bEnd;
	float m_fLast;
	float m_fDuration;
	float m_fRateOfProgress;
	EScene * m_pPrevScene;
	EScene * m_pNextScene;
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
	EVector2 m_Vec;
	EPoint m_NextPos;
};

}