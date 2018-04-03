#pragma once
#include "enode.h"

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

#if HIGHER_THAN_VS2012
	// ����˳����
	ActionSequence(
		const InitList<Action*>& vActions	/* �������� */
	);
#else
	// ����˳����
	ActionSequence(
		int number,			/* �������� */
		Action * action,	/* ��һ������ */
		...
	);
#endif

	virtual ~ActionSequence();

	// �ڽ�β��Ӷ���
	void add(
		Action * action
	);

#if HIGHER_THAN_VS2012
	// �ڽ�β��Ӷ������
	void add(
		const InitList<Action*>& vActions	/* �������� */
	);
#else
	// �ڽ�β��Ӷ������
	void add(
		int number,			/* �������� */
		Action * action,	/* ��һ������ */
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

#if HIGHER_THAN_VS2012
	// ����֡����
	Animation(
		const InitList<Image*>& vImages	/* �ؼ�֡���� */
	);

	// �����ض�֡�����֡����
	Animation(
		double interval,				/* ֡������룩 */
		const InitList<Image*>& vImages	/* �ؼ�֡���� */
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

#if HIGHER_THAN_VS2012
	// ��Ӷ���ؼ�֡
	void add(
		const InitList<Image*>& vImages	/* �ؼ�֡���� */
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
		inline e2d::ActionMoveBy* MoveBy(
			double duration,	/* ��������ʱ�� */
			Vector vector		/* λ������ */
		)
		{
			return new (std::nothrow) ActionMoveBy(duration, vector);
		}

		// ����λ�ƶ���
		inline e2d::ActionMoveTo* MoveTo(
			double duration,	/* ��������ʱ�� */
			Point pos			/* λ����Ŀ�������� */
		)
		{
			return new (std::nothrow) ActionMoveTo(duration, pos);
		}

		// ����������Ŷ���
		inline e2d::ActionScaleBy* ScaleBy(
			double duration,	/* ��������ʱ�� */
			double scale		/* ���ű����仯 */
		)
		{
			return new (std::nothrow) ActionScaleBy(duration, scale);
		}

		// ����������Ŷ���
		inline e2d::ActionScaleBy* ScaleBy(
			double duration,	/* ��������ʱ�� */
			double scaleX,		/* �������ű����仯 */
			double scaleY		/* �������ű����仯 */
		)
		{
			return new (std::nothrow) ActionScaleBy(duration, scaleX, scaleY);
		}

		// �������Ŷ���
		inline e2d::ActionScaleTo* ScaleTo(
			double duration,	/* ��������ʱ�� */
			double scale		/* ������Ŀ����� */
		)
		{
			return new (std::nothrow) ActionScaleTo(duration, scale);
		}

		// �������Ŷ���
		inline e2d::ActionScaleTo* ScaleTo(
			double duration,	/* ��������ʱ�� */
			double scaleX,		/* ����������Ŀ����� */
			double scaleY		/* ����������Ŀ����� */
		)
		{
			return new (std::nothrow) ActionScaleTo(duration, scaleX, scaleY);
		}

		// ����͸������Խ��䶯��
		inline e2d::ActionOpacityBy* OpacityBy(
			double duration,	/* ��������ʱ�� */
			double opacity		/* ͸������Ա仯ֵ */
		)
		{
			return new (std::nothrow) ActionOpacityBy(duration, opacity);
		}

		// ����͸���Ƚ��䶯��
		inline e2d::ActionOpacityTo* OpacityTo(
			double duration,	/* ��������ʱ�� */
			double opacity		/* ͸���Ƚ�����Ŀ��ֵ */
		)
		{
			return new (std::nothrow) ActionOpacityTo(duration, opacity);
		}

		// �������붯��
		inline e2d::ActionFadeIn* FadeIn(
			double duration		/* ��������ʱ�� */
		)
		{
			return new (std::nothrow) ActionFadeIn(duration);
		}

		// ������������
		inline e2d::ActionFadeOut* FadeOut(
			double duration		/* ��������ʱ�� */
		)
		{
			return new (std::nothrow) ActionFadeOut(duration);
		}

		// ���������ת����
		inline e2d::ActionRotateBy* RotateBy(
			double duration,	/* ��������ʱ�� */
			double rotation		/* ��ת�Ƕȱ仯ֵ */
		)
		{
			return new (std::nothrow) ActionRotateBy(duration, rotation);
		}

		// ������ת����
		inline e2d::ActionRotateTo* RotateTo(
			double duration,	/* ��������ʱ�� */
			double rotation		/* ��ת�Ƕ���Ŀ��ֵ */
		)
		{
			return new (std::nothrow) ActionRotateTo(duration, rotation);
		}

		// ����������������������
		inline e2d::ActionTwo* Two(
			Action * pActionFirst,		/* ��һ������ */
			Action * pActionSecond,		/* �ڶ������� */
			bool bAtSameTime = false	/* ͬʱ��ʼ */
		)
		{
			return new (std::nothrow) ActionTwo(pActionFirst, pActionSecond, bAtSameTime);
		}

		// ������ʱ����
		inline e2d::ActionDelay* Delay(
			double duration		/* �ӳ�ʱ�����룩 */
		)
		{
			return new (std::nothrow) ActionDelay(duration);
		}

		// ����ѭ������
		inline e2d::ActionLoop* Loop(
			Action * action,	/* ִ��ѭ���Ķ��� */
			int times = -1		/* ѭ������ */
		)
		{
			return new (std::nothrow) ActionLoop(action, times);
		}

		// ����ִ�к�������Ķ���
		inline e2d::ActionFunc* Func(
			Function func		/* �������� */
		)
		{
			return new (std::nothrow) ActionFunc(func);
		}

#if HIGHER_THAN_VS2012
		// ����˳����
		inline e2d::ActionSequence* Sequence(
			const InitList<Action*>& vActions	/* �������� */
		)
		{
			return new (std::nothrow) ActionSequence(vActions);
		}

		// �����ض�֡�����֡����
		inline e2d::Animation* Animate(
			double interval,					/* ֡������룩 */
			const InitList<Image*>& vFrames		/* �ؼ�֡���� */
		)
		{
			return new (std::nothrow) Animation(interval, vFrames);
		}
#else
		// ����˳����
		inline e2d::ActionSequence* Sequence(
			int number,			/* �������� */
			Action * action1,	/* ��һ������ */
			...
		)
		{
			auto action = new (std::nothrow) ActionSequence();
			if (action)
			{
				Action ** ppAction = &action1;

				while (number > 0)
				{
					WARN_IF((*ppAction) == nullptr, "ActionSequence NULL pointer exception!");
					action->add(*ppAction);
					ppAction++;
					number--;
				}
			}
			return action;
		}

		// �����ض�֡�����֡����
		inline e2d::Animation* Animate(
			double interval,	/* ֡������룩 */
			int number,			/* ֡���� */
			Image * frame,		/* ��һ֡ */
			...
		)
		{
			auto animation = new (std::nothrow) Animation(interval);
			if (animation)
			{
				Image ** ppImage = &frame;

				while (number > 0)
				{
					WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
					animation->add(*ppImage);
					ppImage++;
					number--;
				}
			}
			return animation;
		}
#endif
	}
}