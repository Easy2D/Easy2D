#pragma once
#include "enodes.h"

namespace e2d
{


class ActionManager;
class ActionTwo;
class ActionLoop;
class ActionSequence;
class EActionTwoAtSameTime;
class TransitionFade;

class Action :
	public Object
{
	friend ActionManager;
	friend ActionTwo;
	friend ActionLoop;
	friend ActionSequence;
	friend EActionTwoAtSameTime;

public:
	Action();

	virtual ~Action();

	// ��ȡ��������״̬
	virtual bool isRunning();

	// ��ʼ����
	virtual void setTarget(
		Node* pTarget	/* ִ�иö�����Ŀ�� */
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
		String name
	);

	// ��ȡһ���µ�������
	virtual Action * reverse() const;

	// ��ȡһ���µĿ�������
	virtual Action * clone() const = 0;

	// ���ö���
	virtual void reset();

	// ��ȡ�ö�����ִ��Ŀ��
	virtual Node * getTarget();

protected:
	// ��ʼ������
	virtual void _init();

	// ִ�ж���
	virtual void _update();

	// ��ȡ��������״̬
	virtual bool _isEnding();

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


class ActionGradual :
	public Action
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


class ActionFadeIn :
	public ActionOpacityTo
{
public:
	// �������붯��
	ActionFadeIn(
		double duration	/* ��������ʱ�� */
	) : ActionOpacityTo(duration, 1) {}
};


class ActionFadeOut :
	public ActionOpacityTo
{
public:
	// ������������
	ActionFadeOut(
		double duration	/* ��������ʱ�� */
	) : ActionOpacityTo(duration, 0) {}
};


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


class ActionTwo :
	public Action
{
public:
	// ����������������������
	ActionTwo(
		Action * pActionFirst,		/* ��һ������ */
		Action * pActionSecond,		/* �ڶ������� */
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

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	Action*	m_pFirstAction;
	Action*	m_pSecondAction;
	bool	m_bAtSameTime;
};


class ActionSequence :
	public Action
{
public:
	// ����˳����
	ActionSequence();

	// ����˳����
	ActionSequence(
		std::initializer_list<Action*>& vActions	/* �������� */
	);

	virtual ~ActionSequence();

	// �ڽ�β��Ӷ���
	void add(
		Action * action
	);

	// �ڽ�β��Ӷ������
	void add(
		std::initializer_list<Action*>& vActions	/* �������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ActionSequence * clone() const override;

	// ��ȡ�ö������涯��
	virtual ActionSequence * reverse(
		bool actionReverse = true	/* �Ӷ����Ƿ�ִ���涯�� */
	) const;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	UINT					m_nActionIndex;
	std::vector<Action*>	m_vActions;
};


class ActionDelay :
	public Action
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


class ActionLoop :
	public Action
{
public:
	// ����ѭ������
	ActionLoop(
		Action * action,	/* ִ��ѭ���Ķ��� */
		int times = -1		/* ѭ������ */
	);

	virtual ~ActionLoop();

	// ��ȡ�ö����Ŀ�������
	virtual ActionLoop * clone() const override;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	Action * m_pAction;
	int m_nTimes;
	int m_nTotalTimes;
};


class Animation :
	public Action
{
public:
	// ����֡����
	Animation();

	// �����ض�֡�����֡����
	Animation(
		double interval		/* ֡������룩 */
	);

	// ����֡����
	Animation(
		std::initializer_list<Image*>& vImages
	);

	virtual ~Animation();

	// ��ӹؼ�֡
	void add(
		Image * frame	/* �ؼ�֡ */
	);

	// ��Ӷ���ؼ�֡
	void add(
		std::initializer_list<Image*>& vImages	/* �ؼ�֡���� */
	);

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


class ActionFunc :
	public Action
{
public:
	// ����ִ�к�������Ķ���
	ActionFunc(
		Function func /* �������� */
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

namespace e2d
{
	namespace action
	{
		// �������λ�ƶ���
		ActionMoveBy* MoveBy(
			double duration,	/* ��������ʱ�� */
			Vector vector		/* λ������ */
		);

		// ����λ�ƶ���
		ActionMoveTo* MoveTo(
			double duration,	/* ��������ʱ�� */
			Point pos			/* λ����Ŀ�������� */
		);

		// ����������Ŷ���
		ActionScaleBy* ScaleBy(
			double duration,	/* ��������ʱ�� */
			double scale		/* ���ű����仯 */
		);

		// ����������Ŷ���
		ActionScaleBy* ScaleBy(
			double duration,	/* ��������ʱ�� */
			double scaleX,		/* �������ű����仯 */
			double scaleY		/* �������ű����仯 */
		);

		// �������Ŷ���
		ActionScaleTo* ScaleTo(
			double duration,	/* ��������ʱ�� */
			double scale		/* ������Ŀ����� */
		);

		// �������Ŷ���
		ActionScaleTo* ScaleTo(
			double duration,	/* ��������ʱ�� */
			double scaleX,		/* ����������Ŀ����� */
			double scaleY		/* ����������Ŀ����� */
		);

		// ����͸������Խ��䶯��
		ActionOpacityBy* OpacityBy(
			double duration,	/* ��������ʱ�� */
			double opacity		/* ͸������Ա仯ֵ */
		);

		// ����͸���Ƚ��䶯��
		ActionOpacityTo* OpacityTo(
			double duration,	/* ��������ʱ�� */
			double opacity		/* ͸���Ƚ�����Ŀ��ֵ */
		);

		// �������붯��
		ActionFadeIn* FadeIn(
			double duration	/* ��������ʱ�� */
		);

		// ������������
		ActionFadeOut* FadeOut(
			double duration	/* ��������ʱ�� */
		);

		// ���������ת����
		ActionRotateBy* RotateBy(
			double duration,	/* ��������ʱ�� */
			double rotation		/* ��ת�Ƕȱ仯ֵ */
		);

		// ������ת����
		ActionRotateTo* RotateTo(
			double duration,	/* ��������ʱ�� */
			double rotation		/* ��ת�Ƕ���Ŀ��ֵ */
		);

		// ����������������������
		ActionTwo* Two(
			Action * pActionFirst,		/* ��һ������ */
			Action * pActionSecond,		/* �ڶ������� */
			bool bAtSameTime = false	/* ͬʱ��ʼ */
		);

		// ����˳����
		ActionSequence* Sequence(
			std::initializer_list<Action*>& vActions	/* �������� */
		);

		// ������ʱ����
		ActionDelay* Delay(
			double duration	/* �ӳ�ʱ�����룩 */
		);

		// ����ѭ������
		ActionLoop* Loop(
			Action * action,	/* ִ��ѭ���Ķ��� */
			int times = -1		/* ѭ������ */
		);

		// �����ض�֡�����֡����
		Animation* Animate(
			double interval,						/* ֡������룩 */
			std::initializer_list<Image*>& vFrames /* �ؼ�֡���� */
		);

		// ����ִ�к�������Ķ���
		ActionFunc* Func(
			Function func /* �������� */
		);
	}

	inline e2d::ActionMoveBy * e2d::action::MoveBy(double duration, Vector vector)
	{
		return new (std::nothrow) ActionMoveBy(duration, vector);
	}

	inline e2d::ActionMoveTo * e2d::action::MoveTo(double duration, Point pos)
	{
		return new (std::nothrow) ActionMoveTo(duration, pos);
	}

	inline e2d::ActionScaleBy * e2d::action::ScaleBy(double duration, double scale)
	{
		return new (std::nothrow) ActionScaleBy(duration, scale);
	}

	inline e2d::ActionScaleBy * e2d::action::ScaleBy(double duration, double scaleX, double scaleY)
	{
		return new (std::nothrow) ActionScaleBy(duration, scaleX, scaleY);
	}

	inline e2d::ActionScaleTo * e2d::action::ScaleTo(double duration, double scale)
	{
		return new (std::nothrow) ActionScaleTo(duration, scale);
	}

	inline e2d::ActionScaleTo * e2d::action::ScaleTo(double duration, double scaleX, double scaleY)
	{
		return new (std::nothrow) ActionScaleTo(duration, scaleX, scaleY);
	}

	inline e2d::ActionOpacityBy * e2d::action::OpacityBy(double duration, double opacity)
	{
		return new (std::nothrow) ActionOpacityBy(duration, opacity);
	}

	inline e2d::ActionOpacityTo * e2d::action::OpacityTo(double duration, double opacity)
	{
		return new (std::nothrow) ActionOpacityTo(duration, opacity);
	}

	inline e2d::ActionFadeIn * e2d::action::FadeIn(double duration)
	{
		return new (std::nothrow) ActionFadeIn(duration);
	}

	inline e2d::ActionFadeOut * e2d::action::FadeOut(double duration)
	{
		return new (std::nothrow) ActionFadeOut(duration);
	}

	inline e2d::ActionRotateBy * e2d::action::RotateBy(double duration, double rotation)
	{
		return new (std::nothrow) ActionRotateBy(duration, rotation);
	}

	inline e2d::ActionRotateTo * e2d::action::RotateTo(double duration, double rotation)
	{
		return new (std::nothrow) ActionRotateTo(duration, rotation);
	}

	inline e2d::ActionTwo * e2d::action::Two(Action * pActionFirst, Action * pActionSecond, bool bAtSameTime)
	{
		return new (std::nothrow) ActionTwo(pActionFirst, pActionSecond, bAtSameTime);
	}

	inline e2d::ActionSequence * e2d::action::Sequence(std::initializer_list<Action*>& vActions)
	{
		auto action = new (std::nothrow) ActionSequence();
		if (action)
		{
			action->add(vActions);
		}
		return action;
	}

	inline e2d::ActionDelay * e2d::action::Delay(double duration)
	{
		return new (std::nothrow) ActionDelay(duration);
	}

	inline e2d::ActionLoop * e2d::action::Loop(Action * action, int times)
	{
		return new (std::nothrow) ActionLoop(action, times);
	}

	inline e2d::Animation * e2d::action::Animate(double interval, std::initializer_list<Image*>& vFrames)
	{
		auto animation = new (std::nothrow) Animation(interval);
		if (animation)
		{
			animation->add(vFrames);
		}
		return animation;
	}

	inline e2d::ActionFunc * e2d::action::Func(Function func)
	{
		return new (std::nothrow) ActionFunc(func);
	}
}