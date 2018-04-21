#pragma once
#include "e2dcommon.h"

namespace e2d
{


class SceneManager;

class Transition :
	public Object
{
	friend SceneManager;

public:
	Transition(double duration);

	virtual ~Transition();

	// �����л������Ƿ����
	bool isEnding();

	// ���ٶ���
	virtual void destroy() override;

protected:
	// ��ʼ����������
	virtual void _init(
		Scene * prev,
		Scene * next
	);

	// ���³�������
	virtual void _update();

	// ���³�������
	virtual void _updateCustom() = 0;

	// ��Ⱦ��������
	virtual void _render();

	// ���ó�������
	virtual void _reset() = 0;

	// ֹͣ��������
	virtual void _stop();

protected:
	bool m_bEnd;
	double m_fLast;
	double m_fDuration;
	double m_fRateOfProgress;
	Size m_WindowSize;
	Scene * m_pPrevScene;
	Scene * m_pNextScene;
	ID2D1Layer * m_pPrevLayer;
	ID2D1Layer * m_pNextLayer;
	D2D1_LAYER_PARAMETERS m_sPrevLayerParam;
	D2D1_LAYER_PARAMETERS m_sNextLayerParam;
};


class TransitionFade :
	public Transition
{
public:
	// �������뵭��ʽ�ĳ����л�����
	TransitionFade(
		double duration	/* ��������ʱ�� */
	);

	// �������뵭��ʽ�ĳ����л�����
	TransitionFade(
		double fadeOutDuration,	/* ǰһ����������������ʱ�� */
		double fadeInDuration	/* ��һ�������붯������ʱ�� */
	);

protected:
	// ���¶���
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;

protected:
	double m_fFadeOutDuration;
	double m_fFadeInDuration;
	bool m_bFadeOutTransioning;
};


class TransitionEmerge :
	public Transition
{
public:
	// ��������ʽ�ĳ����л�����
	TransitionEmerge(
		double duration	/* ���ֶ�������ʱ�� */
	);

protected:
	// ���¶���
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;
};


class TransitionMove :
	public Transition
{
public:
	// �����ƶ�ʽ�ĳ����л�����
	TransitionMove(
		double moveDuration,		/* �����ƶ���������ʱ�� */
		int direct = Direct::LEFT	/* �����ƶ����� */
	);

protected:
	// ���¶���
	virtual void _updateCustom() override;

	virtual void _init(
		Scene * prev,
		Scene * next
	) override;

	virtual void _reset() override;

protected:
	int m_Direct;
	Vector m_Vector;
	Point m_NextPos;
};

}