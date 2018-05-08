#pragma once
#include "e2dnode.h"

namespace e2d
{


class ActionManager;
class ActionBase;
class ActionMoveBy;
class ActionMoveTo;
class ActionScaleBy;
class ActionScaleTo;
class ActionOpacityBy;
class ActionOpacityTo;
class ActionFadeIn;
class ActionFadeOut;
class ActionRotateBy;
class ActionRotateTo;
class ActionTwo;
class ActionDelay;
class ActionLoop;
class ActionFunc;
class ActionSequence;
class Animation;

// ����������
class Action
{
public:
	// �������λ�ƶ���
	static e2d::ActionMoveBy* MoveBy(
		double duration,	/* ��������ʱ�� */
		Vector vector		/* λ������ */
	);

	// ����λ�ƶ���
	static e2d::ActionMoveTo* MoveTo(
		double duration,	/* ��������ʱ�� */
		Point pos			/* λ����Ŀ�������� */
	);

	// ����������Ŷ���
	static e2d::ActionScaleBy* ScaleBy(
		double duration,	/* ��������ʱ�� */
		double scale		/* ���ű����仯 */
	);

	// ����������Ŷ���
	static e2d::ActionScaleBy* ScaleBy(
		double duration,	/* ��������ʱ�� */
		double scaleX,		/* �������ű����仯 */
		double scaleY		/* �������ű����仯 */
	);

	// �������Ŷ���
	static e2d::ActionScaleTo* ScaleTo(
		double duration,	/* ��������ʱ�� */
		double scale		/* ������Ŀ����� */
	);

	// �������Ŷ���
	static e2d::ActionScaleTo* ScaleTo(
		double duration,	/* ��������ʱ�� */
		double scaleX,		/* ����������Ŀ����� */
		double scaleY		/* ����������Ŀ����� */
	);

	// ����͸������Խ��䶯��
	static e2d::ActionOpacityBy* OpacityBy(
		double duration,	/* ��������ʱ�� */
		double opacity		/* ͸������Ա仯ֵ */
	);

	// ����͸���Ƚ��䶯��
	static e2d::ActionOpacityTo* OpacityTo(
		double duration,	/* ��������ʱ�� */
		double opacity		/* ͸���Ƚ�����Ŀ��ֵ */
	);

	// �������붯��
	static e2d::ActionFadeIn* FadeIn(
		double duration		/* ��������ʱ�� */
	);

	// ������������
	static e2d::ActionFadeOut* FadeOut(
		double duration		/* ��������ʱ�� */
	);

	// ���������ת����
	static e2d::ActionRotateBy* RotateBy(
		double duration,	/* ��������ʱ�� */
		double rotation		/* ��ת�Ƕȱ仯ֵ */
	);

	// ������ת����
	static e2d::ActionRotateTo* RotateTo(
		double duration,	/* ��������ʱ�� */
		double rotation		/* ��ת�Ƕ���Ŀ��ֵ */
	);

	// ����������������������
	static e2d::ActionTwo* Two(
		ActionBase * pActionFirst,		/* ��һ������ */
		ActionBase * pActionSecond,		/* �ڶ������� */
		bool bAtSameTime = false		/* ͬʱ��ʼ */
	);

	// ������ʱ����
	static e2d::ActionDelay* Delay(
		double duration		/* �ӳ�ʱ�����룩 */
	);

	// ����ѭ������
	static e2d::ActionLoop* Loop(
		ActionBase * action,	/* ִ��ѭ���Ķ��� */
		int times = -1			/* ѭ������ */
	);

	// ����ִ�к�������Ķ���
	static e2d::ActionFunc* Func(
		const Function& func		/* �������� */
	);

#ifdef HIGHER_THAN_VS2012
	// ����˳����
	static e2d::ActionSequence* Sequence(
		const std::initializer_list<ActionBase*>& vActions	/* �����б� */
	);

	// �����ض�֡�����֡����
	static e2d::Animation* Animation(
		double interval,									/* ֡������룩 */
		const std::initializer_list<Image*>& vFrames		/* �ؼ�֡�б� */
	);
#else
	// ����˳����
	static e2d::ActionSequence* Sequence(
		int number,			/* �������� */
		ActionBase * action1,	/* ��һ������ */
		...
	);

	// �����ض�֡�����֡����
	static e2d::Animation* Animation(
		double interval,	/* ֡������룩 */
		int number,			/* ֡���� */
		Image * frame,		/* ��һ֡ */
		...
	);
#endif
};


// ��������
class ActionBase :
	public Object
{
	friend ActionManager;
	friend ActionTwo;
	friend ActionLoop;
	friend ActionSequence;

public:
	ActionBase();

	virtual ~ActionBase();

	// ��ȡ��������״̬
	virtual bool isRunning();

	// ��ʼ����
	virtual void startWithTarget(
		Node* target	/* ִ�иö�����Ŀ�� */
	);

	// ��������
	virtual void resume();

	// ��ͣ����
	virtual void pause();

	// ֹͣ����
	virtual void stop();

	// ��ȡ��������
	virtual String getName() const;

	// ���ö�������
	virtual void setName(
		const String& name
	);

	// ��ȡһ���µ�������
	virtual ActionBase * reverse() const;

	// ��ȡһ���µĿ�������
	virtual ActionBase * clone() const = 0;

	// ���ö���
	virtual void reset();

	// ��ȡ�ö�����ִ��Ŀ��
	virtual Node * getTarget();

	// ���ٶ���
	virtual void destroy() override;

protected:
	// ��ʼ������
	virtual void _init();

	// ִ�ж���
	virtual void _update();

	// ��ȡ��������״̬
	virtual bool _isDone();

	// ���ö���ʱ��
	virtual void _resetTime();

protected:
	String	m_sName;
	bool	m_bRunning;
	bool	m_bEnding;
	bool	m_bInit;
	Node *	m_pTarget;
	Scene * m_pParentScene;
	double	m_fLast;
};


// ��������
class ActionGradual :
	public ActionBase
{
public:
	// �����ض�ʱ���ĳ�������
	ActionGradual(
		double duration
	);

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	double m_fDuration;
	double m_fRateOfProgress;
};


// ���λ�ƶ���
class ActionMoveBy :
	public ActionGradual
{
public:
	// �������λ�ƶ���
	ActionMoveBy(
		double duration,	/* ��������ʱ�� */
		Vector vector		/* λ������ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionMoveBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionMoveBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	Point		m_BeginPos;
	Vector	m_MoveVec;
};


// λ�ƶ���
class ActionMoveTo :
	public ActionMoveBy
{
public:
	// ����λ�ƶ���
	ActionMoveTo(
		double duration,	/* ��������ʱ�� */
		Point pos			/* λ����Ŀ�������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionMoveTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	Point m_EndPos;
};


// ������Ŷ���
class ActionScaleBy :
	public ActionGradual
{
public:
	// ����������Ŷ���
	ActionScaleBy(
		double duration,	/* ��������ʱ�� */
		double scale		/* ���ű����仯 */
	);

	// ����������Ŷ���
	ActionScaleBy(
		double duration,	/* ��������ʱ�� */
		double scaleX,		/* �������ű����仯 */
		double scaleY		/* �������ű����仯 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionScaleBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionScaleBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double	m_nBeginScaleX;
	double	m_nBeginScaleY;
	double	m_nVariationX;
	double	m_nVariationY;
};


// ���Ŷ���
class ActionScaleTo :
	public ActionScaleBy
{
public:
	// �������Ŷ���
	ActionScaleTo(
		double duration,	/* ��������ʱ�� */
		double scale		/* ������Ŀ����� */
	);

	// �������Ŷ���
	ActionScaleTo(
		double duration,	/* ��������ʱ�� */
		double scaleX,		/* ����������Ŀ����� */
		double scaleY		/* ����������Ŀ����� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionScaleTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	double	m_nEndScaleX;
	double	m_nEndScaleY;
};


// ͸������Խ��䶯��
class ActionOpacityBy :
	public ActionGradual
{
public:
	// ����͸������Խ��䶯��
	ActionOpacityBy(
		double duration,	/* ��������ʱ�� */
		double opacity		/* ͸������Ա仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionOpacityBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionOpacityBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double m_nBeginVal;
	double m_nVariation;
};


// ͸���Ƚ��䶯��
class ActionOpacityTo :
	public ActionOpacityBy
{
public:
	// ����͸���Ƚ��䶯��
	ActionOpacityTo(
		double duration,	/* ��������ʱ�� */
		double opacity		/* ͸���Ƚ�����Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionOpacityTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	double m_nEndVal;
};


// ���붯��
class ActionFadeIn :
	public ActionOpacityTo
{
public:
	// �������붯��
	ActionFadeIn(
		double duration	/* ��������ʱ�� */
	) : ActionOpacityTo(duration, 1) {}
};


// ��������
class ActionFadeOut :
	public ActionOpacityTo
{
public:
	// ������������
	ActionFadeOut(
		double duration	/* ��������ʱ�� */
	) : ActionOpacityTo(duration, 0) {}
};


// �����ת����
class ActionRotateBy :
	public ActionGradual
{
public:
	// ���������ת����
	ActionRotateBy(
		double duration,	/* ��������ʱ�� */
		double rotation		/* ��ת�Ƕȱ仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionRotateBy * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionRotateBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double m_nBeginVal;
	double m_nVariation;
};


// ��ת����
class ActionRotateTo :
	public ActionRotateBy
{
public:
	// ������ת����
	ActionRotateTo(
		double duration,	/* ��������ʱ�� */
		double rotation		/* ��ת�Ƕ���Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionRotateTo * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	double m_nEndVal;
};


// ��϶���
class ActionTwo :
	public ActionBase
{
public:
	// ����������������������
	ActionTwo(
		ActionBase * pActionFirst,		/* ��һ������ */
		ActionBase * pActionSecond,		/* �ڶ������� */
		bool bAtSameTime = false	/* ͬʱ��ʼ */
	);

	virtual ~ActionTwo();

	// ��ȡ�ö����Ŀ�������
	virtual ActionTwo * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionTwo * reverse(
		bool actionReverse = true	/* �Ӷ����Ƿ�ִ���涯�� */
	) const;

	// ���ö���
	virtual void reset() override;

	// ���ٶ���
	virtual void destroy() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	ActionBase*	m_pFirstAction;
	ActionBase*	m_pSecondAction;
	bool	m_bAtSameTime;
};


// ˳����
class ActionSequence :
	public ActionBase
{
public:
	// ����˳����
	ActionSequence();

#ifdef HIGHER_THAN_VS2012
	// ����˳����
	ActionSequence(
		const std::initializer_list<ActionBase*>& vActions	/* �����б� */
	);
#else
	// ����˳����
	ActionSequence(
		int number,			/* �������� */
		ActionBase * action,	/* ��һ������ */
		...
	);
#endif

	virtual ~ActionSequence();

	// �ڽ�β��Ӷ���
	void add(
		ActionBase * action
	);

#ifdef HIGHER_THAN_VS2012
	// �ڽ�β��Ӷ������
	void add(
		const std::initializer_list<ActionBase*>& vActions	/* �����б� */
	);
#else
	// �ڽ�β��Ӷ������
	void add(
		int number,			/* �������� */
		ActionBase * action,	/* ��һ������ */
		...
	);
#endif

	// ��ȡ�ö����Ŀ�������
	virtual ActionSequence * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionSequence * reverse(
		bool actionReverse = true	/* �Ӷ����Ƿ�ִ���涯�� */
	) const;

	// ���ö���
	virtual void reset() override;

	// ���ٶ���
	virtual void destroy() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	UINT					m_nActionIndex;
	std::vector<ActionBase*>	m_vActions;
};


// ��ʱ����
class ActionDelay :
	public ActionBase
{
public:
	// ������ʱ����
	ActionDelay(
		double duration	/* �ӳ�ʱ�����룩 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionDelay * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double m_fDelayTime;
};


// ѭ������
class ActionLoop :
	public ActionBase
{
public:
	// ����ѭ������
	ActionLoop(
		ActionBase * action,	/* ִ��ѭ���Ķ��� */
		int times = -1		/* ѭ������ */
	);

	virtual ~ActionLoop();

	// ��ȡ�ö����Ŀ�������
	virtual ActionLoop * clone() const override;

	// ���ö���
	virtual void reset() override;

	// ���ٶ���
	virtual void destroy() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	ActionBase * m_pAction;
	int m_nTimes;
	int m_nTotalTimes;
};


// ֡����
class Animation :
	public ActionBase
{
public:
	// ����֡����
	Animation();

	// �����ض�֡�����֡����
	Animation(
		double interval		/* ֡������룩 */
	);

#ifdef HIGHER_THAN_VS2012
	// ����֡����
	Animation(
		const std::initializer_list<Image*>& vImages	/* �ؼ�֡�б� */
	);

	// �����ض�֡�����֡����
	Animation(
		double interval,								/* ֡������룩 */
		const std::initializer_list<Image*>& vImages	/* �ؼ�֡�б� */
	);
#else
	// ����֡����
	Animation(
		int number,			/* ֡���� */
		Image * frame,		/* ��һ֡ */
		...
	);

	// �����ض�֡�����֡����
	Animation(
		double interval,	/* ֡������룩 */
		int number,			/* ֡���� */
		Image * frame,		/* ��һ֡ */
		...
	);
#endif

	virtual ~Animation();

	// ��ӹؼ�֡
	void add(
		Image * frame	/* �ؼ�֡ */
	);

#ifdef HIGHER_THAN_VS2012
	// ��Ӷ���ؼ�֡
	void add(
		const std::initializer_list<Image*>& vImages	/* �ؼ�֡�б� */
	);
#else
	// ��Ӷ���ؼ�֡
	void add(
		int number,			/* ֡���� */
		Image * frame,		/* ��һ֡ */
		...
	);
#endif

	// ����ÿһ֡��ʱ����
	void setInterval(
		double interval	/* ֡������룩 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual Animation * clone() const override;

	// ��ȡ�ö������涯��
	virtual Animation * reverse() const override;

	// ���ö���
	virtual void reset() override;

	// ���ٶ���
	virtual void destroy() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double	m_fInterval;
	UINT	m_nFrameIndex;
	std::vector<Image*> m_vFrames;
};


// �ص�����
class ActionFunc :
	public ActionBase
{
public:
	// ����ִ�к�������Ķ���
	ActionFunc(
		const Function& func /* �������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionFunc * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	Function m_Callback;
};


}