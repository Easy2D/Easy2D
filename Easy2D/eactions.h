#pragma once
#include "enodes.h"
#include "etools.h"

namespace e2d
{

class EActionTwo;
class EActionNeverStop;
class EActionSequence;

class EAction :
	public EObject
{
	friend ENode;
	friend EActionManager;
	friend EActionTwo;
	friend EActionNeverStop;
	friend EActionSequence;

public:
	EAction();

	virtual ~EAction();

	// ��ȡ��������״̬
	virtual bool isRunning();

	// ��ȡ��������״̬
	virtual bool isEnding();

	// ��������
	virtual void start();

	// ��������
	virtual void resume();

	// ��ͣ����
	virtual void pause();

	// ֹͣ����
	virtual void stop();

	// ���ö���ÿһ֡��ʱ����
	virtual void setInterval(
		LONGLONG milliSeconds
	);

	// ��ȡһ���µĿ�������
	virtual EAction * copy() const = 0;

	// ��ȡһ���µ�������
	virtual EAction * reverse() const;
	// ��ȡִ�иö�����Ŀ��
	virtual ENode * getTarget();

protected:
	// ��ʼ������
	virtual void _init();

	// ִ�ж���
	virtual void _exec() = 0;

	// ���ö���
	virtual void _reset();

	// ����ȴ�״̬
	virtual void wait();

	// ����
	virtual void notify();

protected:
	bool		m_bRunning;
	bool		m_bWaiting;
	bool		m_bEnding;
	bool		m_bInit;
	ENode *		m_pTarget;
	EScene *	m_pParentScene;
	LONGLONG	m_nAnimationInterval;
	std::chrono::steady_clock::time_point m_nLast;
};


class EAnimation :
	public EAction
{
public:
	EAnimation(float duration);
	virtual ~EAnimation();

protected:
	LONGLONG m_nDuration;
	LONGLONG m_nTotalDuration;

protected:
	bool _isEnd() const;
	bool _isDelayEnough();
	virtual void _init() override;
	virtual void _reset() override;
};


class EActionMoveBy :
	public EAnimation
{
public:
	EActionMoveBy(float duration, EVec vector);
	virtual ~EActionMoveBy();

	virtual EActionMoveBy * copy() const override;
	virtual EActionMoveBy * reverse() const override;

protected:
	EPoint	m_BeginPos;
	EVec	m_MoveVector;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionMoveTo :
	public EActionMoveBy
{
public:
	EActionMoveTo(float duration, EPoint pos);
	virtual ~EActionMoveTo();

	virtual EActionMoveTo * copy() const override;

protected:
	EPoint m_EndPos;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};


class EActionScaleBy :
	public EAnimation
{
public:
	EActionScaleBy(float duration, float scaleX, float scaleY);
	virtual ~EActionScaleBy();

	virtual EActionScaleBy * copy() const override;
	virtual EActionScaleBy * reverse() const override;

protected:
	float	m_nBeginScaleX;
	float	m_nBeginScaleY;
	float	m_nVariationX;
	float	m_nVariationY;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionScaleTo :
	public EActionScaleBy
{
public:
	EActionScaleTo(float duration, float scaleX, float scaleY);
	virtual ~EActionScaleTo();

	virtual EActionScaleTo * copy() const override;

protected:
	float	m_nEndScaleX;
	float	m_nEndScaleY;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};


class EActionOpacityBy :
	public EAnimation
{
public:
	EActionOpacityBy(float duration, float opacity);
	virtual ~EActionOpacityBy();

	virtual EActionOpacityBy * copy() const override;
	virtual EActionOpacityBy * reverse() const override;

protected:
	float m_nBeginVal;
	float m_nVariation;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionOpacityTo :
	public EActionOpacityBy
{
public:
	EActionOpacityTo(float duration, float opacity);
	virtual ~EActionOpacityTo();

	virtual EActionOpacityTo * copy() const override;

protected:
	float m_nEndVal;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};


class ActionFadeIn :
	public EActionOpacityTo
{
public:
	ActionFadeIn(float duration) : EActionOpacityTo(duration, 1) {}
};


class ActionFadeOut :
	public EActionOpacityTo
{
public:
	ActionFadeOut(float duration) : EActionOpacityTo(duration, 0) {}
};


class EActionTwo :
	public EAction
{
public:
	EActionTwo(EAction * actionFirst, EAction * actionSecond);
	virtual ~EActionTwo();

	virtual EActionTwo * copy() const override;
	virtual EActionTwo * reverse(bool actionReverse = true) const;

protected:
	EAction *	m_FirstAction;
	EAction *	m_SecondAction;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionSequence :
	public EAction
{
public:
	EActionSequence();
	EActionSequence(int number, EAction * action1, ...);
	virtual ~EActionSequence();

	void addAction(EAction * action);
	virtual EActionSequence * copy() const override;
	virtual EActionSequence * reverse(bool actionReverse = true) const;

protected:
	UINT					m_nActionIndex;
	std::vector<EAction*>	m_vActions;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionDelay :
	public EAction
{
public:
	EActionDelay(float duration);
	virtual ~EActionDelay();

	virtual EActionDelay * copy() const override;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionNeverStop :
	public EAction
{
public:
	EActionNeverStop(EAction * action);
	virtual ~EActionNeverStop();

	virtual EActionNeverStop * copy() const override;

protected:
	EAction * m_Action;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionFrames :
	public EAction
{
public:
	EActionFrames();
	EActionFrames(LONGLONG frameDelay);
	~EActionFrames();

	void addFrame(ESpriteFrame * frame);
	virtual EActionFrames * copy() const override;
	virtual EActionFrames * reverse() const override;

protected:
	UINT m_nFrameIndex;
	EVector<ESpriteFrame*> m_vFrames;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class EActionCallback :
	public EAction
{
public:
	EActionCallback(const std::function<void()>& callback);
	~EActionCallback();

	virtual EActionCallback * copy() const override;

protected:
	std::function<void()> m_Callback;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};

}