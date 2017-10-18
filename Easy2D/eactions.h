#pragma once
#include "enodes.h"
#include "etools.h"

namespace e2d
{

class ActionTwo;
class ActionNeverStop;
class ActionSequence;

class EAction :
	public EObject
{
	friend ENode;
	friend EActionManager;
	friend ActionTwo;
	friend ActionNeverStop;
	friend ActionSequence;

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
	// ���ö���ÿһ֡ʱ����
	virtual void setInterval(LONGLONG milliSeconds);
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


class Animation :
	public EAction
{
public:
	Animation(float duration);
	virtual ~Animation();

protected:
	LONGLONG m_nDuration;
	LONGLONG m_nTotalDuration;

protected:
	bool _isEnd() const;
	bool _isDelayEnough();
	virtual void _init() override;
	virtual void _reset() override;
};


class ActionMoveBy :
	public Animation
{
public:
	ActionMoveBy(float duration, EVec vector);
	virtual ~ActionMoveBy();

	virtual ActionMoveBy * copy() const override;
	virtual ActionMoveBy * reverse() const override;

protected:
	EPoint	m_BeginPos;
	EVec	m_MoveVector;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionMoveTo :
	public ActionMoveBy
{
public:
	ActionMoveTo(float duration, EPoint pos);
	virtual ~ActionMoveTo();

	virtual ActionMoveTo * copy() const override;

protected:
	EPoint m_EndPos;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};


class ActionScaleBy :
	public Animation
{
public:
	ActionScaleBy(float duration, float scaleX, float scaleY);
	virtual ~ActionScaleBy();

	virtual ActionScaleBy * copy() const override;
	virtual ActionScaleBy * reverse() const override;

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


class ActionScaleTo :
	public ActionScaleBy
{
public:
	ActionScaleTo(float duration, float scaleX, float scaleY);
	virtual ~ActionScaleTo();

	virtual ActionScaleTo * copy() const override;

protected:
	float	m_nEndScaleX;
	float	m_nEndScaleY;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};


class ActionOpacityBy :
	public Animation
{
public:
	ActionOpacityBy(float duration, float opacity);
	virtual ~ActionOpacityBy();

	virtual ActionOpacityBy * copy() const override;
	virtual ActionOpacityBy * reverse() const override;

protected:
	float m_nBeginVal;
	float m_nVariation;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionOpacityTo :
	public ActionOpacityBy
{
public:
	ActionOpacityTo(float duration, float opacity);
	virtual ~ActionOpacityTo();

	virtual ActionOpacityTo * copy() const override;

protected:
	float m_nEndVal;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};


class ActionFadeIn :
	public ActionOpacityTo
{
public:
	ActionFadeIn(float duration) : ActionOpacityTo(duration, 1) {}
};


class ActionFadeOut :
	public ActionOpacityTo
{
public:
	ActionFadeOut(float duration) : ActionOpacityTo(duration, 0) {}
};


class ActionTwo :
	public EAction
{
public:
	ActionTwo(EAction * actionFirst, EAction * actionSecond);
	virtual ~ActionTwo();

	virtual ActionTwo * copy() const override;
	virtual ActionTwo * reverse(bool actionReverse = true) const;

protected:
	EAction *	m_FirstAction;
	EAction *	m_SecondAction;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionSequence :
	public EAction
{
public:
	ActionSequence();
	ActionSequence(int number, EAction * action1, ...);
	virtual ~ActionSequence();

	void addAction(EAction * action);
	virtual ActionSequence * copy() const override;
	virtual ActionSequence * reverse(bool actionReverse = true) const;

protected:
	UINT					m_nActionIndex;
	std::vector<EAction*>	m_vActions;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionDelay :
	public EAction
{
public:
	ActionDelay(float duration);
	virtual ~ActionDelay();

	virtual ActionDelay * copy() const override;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionNeverStop :
	public EAction
{
public:
	ActionNeverStop(EAction * action);
	virtual ~ActionNeverStop();

	virtual ActionNeverStop * copy() const override;

protected:
	EAction * m_Action;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionFrames :
	public EAction
{
public:
	ActionFrames();
	ActionFrames(LONGLONG frameDelay);
	~ActionFrames();

	void addFrame(Image * frame);
	virtual ActionFrames * copy() const override;
	virtual ActionFrames * reverse() const override;

protected:
	UINT			m_nFrameIndex;
	EVector<Image*> m_vFrames;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};


class ActionCallback :
	public EAction
{
public:
	ActionCallback(const std::function<void()>& callback);
	~ActionCallback();

	virtual ActionCallback * copy() const override;

protected:
	std::function<void()> m_Callback;

protected:
	virtual void _init() override;
	virtual void _exec() override;
	virtual void _reset() override;
};

}