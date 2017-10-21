#pragma once
#include "enodes.h"
#include <chrono>

namespace e2d
{

class EActionManager;
class EActionTwo;
class EActionLoop;
class EActionSequence;
class EActionTwoAtSameTime;
class ETransitionFade;

class EAction :
	public EObject
{
	friend EActionManager;
	friend EActionTwo;
	friend EActionLoop;
	friend EActionSequence;
	friend EActionTwoAtSameTime;

public:
	EAction();

	virtual ~EAction();

	// ��ȡ��������״̬
	virtual bool isRunning();

	// ��������
	virtual void start();

	// ��������
	virtual void resume();

	// ��ͣ����
	virtual void pause();

	// ֹͣ����
	virtual void stop();

	// ��ȡһ���µĿ�������
	virtual EAction * clone() const = 0;

	// ��ȡһ���µ�������
	virtual EAction * reverse() const;

	// ��ȡִ�иö�����Ŀ��
	virtual ENode * getTarget();

	// ���ö���ÿһ֡��ʱ����
	virtual void setInterval(
		LONGLONG milliSeconds
	);

	// ���ö���ִ��Ŀ��
	virtual void setTarget(
		ENode * node
	);

protected:
	// ��ʼ������
	virtual void _init();

	// ִ�ж���
	virtual void _callOn() = 0;

	// ��ȡ��������״̬
	virtual bool _isEnding();

	// ���ö���
	virtual void _reset();

protected:
	bool		m_bRunning;
	bool		m_bEnding;
	bool		m_bInit;
	ENode *		m_pTarget;
	EScene *	m_pParentScene;
	LONGLONG	m_nAnimationInterval;
	std::chrono::steady_clock::time_point m_tLast;
};


class EActionGradual :
	public EAction
{
public:
	// ����ʱ������
	EActionGradual(
		float duration
	);

protected:
	// �ж϶����Ƿ����
	bool _isEnd() const;

	// �ж���ʱ�Ƿ��㹻
	bool _isDelayEnough();

	// ��ʼ������
	virtual void _init() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float m_fDuration;
	float m_fTotalDuration;
	float m_fRateOfProgress;
};


class EActionMoveBy :
	public EActionGradual
{
public:
	// �������λ�ƶ���
	EActionMoveBy(
		float duration, /* ��������ʱ�� */
		EVec vector		/* λ������ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionMoveBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionMoveBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	EPoint	m_BeginPos;
	EVec	m_MoveVector;
};


class EActionMoveTo :
	public EActionMoveBy
{
public:
	// ����λ�ƶ���
	EActionMoveTo(
		float duration, /* ��������ʱ�� */
		EPoint pos		/* λ����Ŀ�������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionMoveTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���ö���
	virtual void _reset() override;

protected:
	EPoint m_EndPos;
};


class EActionScaleBy :
	public EActionGradual
{
public:
	// ����������Ŷ���
	EActionScaleBy(
		float duration, /* ��������ʱ�� */
		float scale		/* ���ű����仯 */
	);

	// ����������Ŷ���
	EActionScaleBy(
		float duration, /* ��������ʱ�� */
		float scaleX,	/* �������ű����仯 */
		float scaleY	/* �������ű����仯 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionScaleBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionScaleBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float	m_nBeginScaleX;
	float	m_nBeginScaleY;
	float	m_nVariationX;
	float	m_nVariationY;
};


class EActionScaleTo :
	public EActionScaleBy
{
public:
	// �������Ŷ���
	EActionScaleTo(
		float duration, /* ��������ʱ�� */
		float scale		/* ������Ŀ����� */
	);

	// �������Ŷ���
	EActionScaleTo(
		float duration, /* ��������ʱ�� */
		float scaleX,	/* ����������Ŀ����� */
		float scaleY	/* ����������Ŀ����� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionScaleTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float	m_nEndScaleX;
	float	m_nEndScaleY;
};


class EActionOpacityBy :
	public EActionGradual
{
public:
	// ����͸������Խ��䶯��
	EActionOpacityBy(
		float duration, /* ��������ʱ�� */
		float opacity	/* ͸������Ա仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionOpacityBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionOpacityBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float m_nBeginVal;
	float m_nVariation;
};


class EActionOpacityTo :
	public EActionOpacityBy
{
public:
	// ����͸���Ƚ��䶯��
	EActionOpacityTo(
		float duration,	/* ��������ʱ�� */
		float opacity	/* ͸���Ƚ�����Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionOpacityTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float m_nEndVal;
};


class EActionFadeIn :
	public EActionOpacityTo
{
public:
	// �������붯��
	EActionFadeIn(
		float duration	/* ��������ʱ�� */
	) : EActionOpacityTo(duration, 1) {}
};


class EActionFadeOut :
	public EActionOpacityTo
{
public:
	// ������������
	EActionFadeOut(
		float duration	/* ��������ʱ�� */
	) : EActionOpacityTo(duration, 0) {}
};


class EActionRotateBy :
	public EActionGradual
{
public:
	// ���������ת����
	EActionRotateBy(
		float duration,	/* ��������ʱ�� */
		float rotation	/* ��ת�Ƕȱ仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionRotateBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionRotateBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float m_nBeginVal;
	float m_nVariation;
};


class EActionRotateTo :
	public EActionRotateBy
{
public:
	// ������ת����
	EActionRotateTo(
		float duration,	/* ��������ʱ�� */
		float rotation	/* ��ת�Ƕ���Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionRotateTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���ö���
	virtual void _reset() override;

protected:
	float m_nEndVal;
};


class EActionTwo :
	public EAction
{
public:
	// ����������������������
	EActionTwo(
		EAction * actionFirst,	/* ��һ������ */
		EAction * actionSecond	/* �ڶ������� */
	);

	virtual ~EActionTwo();

	// ��ȡ�ö����Ŀ�������
	virtual EActionTwo * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionTwo * reverse(
		bool actionReverse = true	/* �Ӷ����Ƿ�ִ���涯�� */
	) const;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	EAction *	m_pFirstAction;
	EAction *	m_pSecondAction;
};


class EActionSequence :
	public EAction
{
public:
	// ����˳����
	EActionSequence();

	// ����˳����
	EActionSequence(
		int number,			/* ˳�������� */
		EAction * action1,	/* ��һ������ */
		...
	);

	virtual ~EActionSequence();

	// ��˳��������Ӷ���
	void addAction(
		EAction * action	/* �����������˳����β�� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionSequence * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionSequence * reverse(
		bool actionReverse = true	/* �Ӷ����Ƿ�ִ���涯�� */
	) const;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	UINT					m_nActionIndex;
	std::vector<EAction*>	m_vActions;
};


class EActionDelay :
	public EAction
{
public:
	// ������ʱ����
	EActionDelay(
		float duration	/* �ӳ�ʱ�����룩 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionDelay * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;
};


class EActionTwoAtSameTime :
	public EAction
{
public:
	// ������������ͬʱִ�еĶ���
	EActionTwoAtSameTime(
		EAction * actionFirst,	/* ��һ������ */
		EAction * actionSecond	/* �ڶ������� */
	);

	virtual ~EActionTwoAtSameTime();

	// ��ȡ�ö����Ŀ�������
	virtual EActionTwoAtSameTime * clone() const override;

	// ��ȡ�ö������涯��
	virtual EActionTwoAtSameTime * reverse(
		bool actionReverse = true	/* �Ӷ����Ƿ�ִ���涯�� */
	) const;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	EAction *	m_pFirstAction;
	EAction *	m_pSecondAction;
};


class EActionLoop :
	public EAction
{
public:
	// ����ѭ������
	EActionLoop(
		EAction * action	/* ִ��ѭ���Ķ��� */
	);

	virtual ~EActionLoop();

	// ��ȡ�ö����Ŀ�������
	virtual EActionLoop * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	EAction * m_Action;
};


class EAnimation :
	public EAction
{
public:
	// ����֡����
	EAnimation();

	// �����ض�֡�����֡����
	EAnimation(
		LONGLONG frameDelay	/* ֡��������룩 */
	);

	virtual ~EAnimation();

	// ���֡
	void addFrame(
		ESpriteFrame * frame /* ���֡ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EAnimation * clone() const override;

	// ��ȡ�ö������涯��
	virtual EAnimation * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	UINT m_nFrameIndex;
	EVector<ESpriteFrame*> m_vFrames;
};


class EActionCallback :
	public EAction
{
public:
	// ����ִ�лص������Ķ���
	EActionCallback(
		const std::function<void()> & callback /* �ص����� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual EActionCallback * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _callOn() override;

	// ���ö���
	virtual void _reset() override;

protected:
	std::function<void()> m_Callback;
};

}