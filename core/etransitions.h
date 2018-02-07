#pragma once
#include "ecommon.h"

namespace e2d
{


class SceneManager;

class Transition :
	public Obj
{
	friend SceneManager;

public:
	Transition(float duration);

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
		Scene * prev,
		Scene * next
	);

protected:
	bool m_bEnd;
	float m_fLast;
	float m_fDuration;
	float m_fRateOfProgress;
	Scene * m_pPrevScene;
	Scene * m_pNextScene;
};


class TransitionFade :
	public Transition
{
public:
	// �������뵭��ʽ�ĳ����л�����
	TransitionFade(
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


class TransitionEmerge :
	public Transition
{
public:
	// ��������ʽ�ĳ����л�����
	TransitionEmerge(
		float duration	/* ���ֶ�������ʱ�� */
	);

protected:
	// ���¶���
	virtual void _update() override;

	virtual void _init() override;

	virtual void _reset() override;
};


class TransitionMove :
	public Transition
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
	TransitionMove(
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
	Vector m_Vec;
	Point m_NextPos;
};

}