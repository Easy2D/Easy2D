#pragma once
#include <easy2d/e2dcommon.h>
#include <easy2d/e2dbase.h>

namespace easy2d
{


class Node;
class Loop;
class Sequence;
class Spawn;
class ActionManager;


// ��������
class Action :
	public Object
{
	friend class ActionManager;
	friend class Loop;
	friend class Sequence;
	friend class Spawn;

public:
	Action();

	virtual ~Action();

	// ��ȡ��������״̬
	virtual bool isRunning();

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

	// ��ȡ�����Ŀ���
	virtual Action * clone() const = 0;

	// ��ȡ�����ĵ�ת
	virtual Action * reverse() const = 0;

	// ���ö���
	virtual void reset();

	// ��ȡ�ö�����ִ��Ŀ��
	virtual Node * getTarget();

protected:
	// ��ʼ������
	virtual void _init();

	// ���¶���
	virtual void _update();

	// ��ȡ��������״̬
	virtual bool _isDone();

	// ���ö���ʱ��
	virtual void _resetTime();

	// ��ʼ����
	virtual void _startWithTarget(
		Node* target
	);

protected:
	String	_name;
	bool	_running;
	bool	_done;
	bool	_initialized;
	Node *	_target;
	float	_last;
};


// ��������
class FiniteTimeAction :
	public Action
{
public:
	// �����ض�ʱ���ĳ�������
	explicit FiniteTimeAction(
		float duration
	);

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	float _duration;
	float _delta;
};


// ���λ�ƶ���
class MoveBy :
	public FiniteTimeAction
{
public:
	explicit MoveBy(
		float duration,	/* ����ʱ�� */
		Vector2 vector		/* �ƶ����� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual MoveBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual MoveBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	Point	_startPos;
	Point	_prevPos;
	Vector2	_deltaPos;
};


// λ�ƶ���
class MoveTo :
	public MoveBy
{
public:
	explicit MoveTo(
		float duration,	/* ����ʱ�� */
		Point pos			/* Ŀ������ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual MoveTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual MoveTo * reverse() const override
	{
		E2D_WARNING(L"reverse() not supported in MoveTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	Point _endPos;
};


// �����Ծ����
class JumpBy :
	public FiniteTimeAction
{
public:
	explicit JumpBy(
		float duration,		/* ����ʱ�� */
		const Vector2& vec,		/* ��Ծ���� */
		float height,			/* ��Ծ�߶� */
		int jumps = 1			/* ��Ծ���� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual JumpBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual JumpBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	Point	_startPos;
	Vector2	_deltaPos;
	float	_height;
	int		_jumps;
	Point	_prevPos;
};


// ��Ծ����
class JumpTo :
	public JumpBy
{
public:
	explicit JumpTo(
		float duration,		/* ����ʱ�� */
		const Point& pos,		/* Ŀ������ */
		float height,			/* ��Ծ�߶� */
		int jumps = 1			/* ��Ծ���� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual JumpTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual JumpTo * reverse() const override
	{
		E2D_WARNING(L"reverse() not supported in JumpTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	Point _endPos;
};


// ������Ŷ���
class ScaleBy :
	public FiniteTimeAction
{
public:
	explicit ScaleBy(
		float duration,	/* ����ʱ�� */
		float scale		/* ��Ա仯ֵ */
	);

	explicit ScaleBy(
		float duration,	/* ����ʱ�� */
		float scaleX,		/* ����������Ա仯ֵ */
		float scaleY		/* ����������Ա仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ScaleBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual ScaleBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	float	_startScaleX;
	float	_startScaleY;
	float	_deltaX;
	float	_deltaY;
};


// ���Ŷ���
class ScaleTo :
	public ScaleBy
{
public:
	explicit ScaleTo(
		float duration,	/* ����ʱ�� */
		float scale		/* Ŀ��ֵ */
	);

	explicit ScaleTo(
		float duration,	/* ����ʱ�� */
		float scaleX,		/* ��������Ŀ��ֵ */
		float scaleY		/* ��������Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ScaleTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual ScaleTo * reverse() const override
	{
		E2D_WARNING(L"reverse() not supported in ScaleTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	float	_endScaleX;
	float	_endScaleY;
};


// ͸������Խ��䶯��
class OpacityBy :
	public FiniteTimeAction
{
public:
	explicit OpacityBy(
		float duration,	/* ����ʱ�� */
		float opacity		/* ��Ա仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual OpacityBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual OpacityBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	float _startVal;
	float _deltaVal;
};


// ͸���Ƚ��䶯��
class OpacityTo :
	public OpacityBy
{
public:
	explicit OpacityTo(
		float duration,	/* ����ʱ�� */
		float opacity		/* Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual OpacityTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual OpacityTo * reverse() const override
	{
		E2D_WARNING(L"reverse() not supported in OpacityTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	float _endVal;
};


// ���붯��
class FadeIn :
	public OpacityTo
{
public:
	// �������붯��
	explicit FadeIn(
		float duration		/* ����ʱ�� */
	)
	: OpacityTo(duration, 1) 
	{
	}
};


// ��������
class FadeOut :
	public OpacityTo
{
public:
	// ������������
	explicit FadeOut(
		float duration		/* ����ʱ�� */
	)
	: OpacityTo(duration, 0) 
	{
	}
};


// �����ת����
class RotateBy :
	public FiniteTimeAction
{
public:
	explicit RotateBy(
		float duration,	/* ����ʱ�� */
		float rotation		/* ��Ա仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual RotateBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual RotateBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	float _startVal;
	float _deltaVal;
};


// ��ת����
class RotateTo :
	public RotateBy
{
public:
	explicit RotateTo(
		float duration,	/* ����ʱ�� */
		float rotation		/* Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual RotateTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual RotateTo * reverse() const override
	{
		E2D_WARNING(L"reverse() not supported in RotateTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	float _endVal;
};


// ��ʱ����
class Delay :
	public Action
{
public:
	explicit Delay(
		float duration	/* �ӳ�ʱ�����룩 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual Delay * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual Delay * reverse() const override;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	float _delay;
	float _delta;
};


// ѭ������
class Loop :
	public Action
{
public:
	explicit Loop(
		Action * action,	/* ִ��ѭ���Ķ��� */
		int times = -1		/* ѭ������ */
	);

	virtual ~Loop();

	// ��ȡ�ö����Ŀ�������
	virtual Loop * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual Loop * reverse() const override;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	Action * _action;
	int _times;
	int _totalTimes;
};


// �ص�����
class CallFunc :
	public Action
{
public:
	explicit CallFunc(
		const Function<void()>& func /* �������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual CallFunc * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual CallFunc * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	Function<void()> _func;
};


// ˳����
class Sequence :
	public Action
{
public:
	Sequence();

	explicit Sequence(
		const std::vector<Action*>& actions	/* �����б� */
	);

	virtual ~Sequence();

	// �ڽ�β��Ӷ���
	void add(
		Action * action
	);

	// �ڽ�β��Ӷ������
	void add(
		const std::vector<Action*>& actions	/* �����б� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual Sequence * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual Sequence * reverse() const;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	UINT _currIndex;
	std::vector<Action*> _actions;
};


// ͬ������
class Spawn :
	public Action
{
public:
	Spawn();

	explicit Spawn(
		const std::vector<Action*>& actions	/* �����б� */
	);

	virtual ~Spawn();

	// �ڽ�β��Ӷ���
	void add(
		Action * action
	);

	// �ڽ�β��Ӷ������
	void add(
		const std::vector<Action*>& actions	/* �����б� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual Spawn * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual Spawn * reverse() const;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	std::vector<Action*> _actions;
};


// ֡����
class Animation :
	public Object
{
public:
	Animation();

	explicit Animation(
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	explicit Animation(
		float interval						/* ֡������룩 */
	);

	explicit Animation(
		float interval,					/* ֡������룩 */
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	virtual ~Animation();

	// ��ӹؼ�֡
	void add(
		Image * frame	/* �ؼ�֡ */
	);

	// ��Ӷ���ؼ�֡
	void add(
		const std::vector<Image*>& frames	/* �ؼ�֡�б� */
	);

	// ��ȡ֡���
	float getInterval() const;

	// ��ȡ�ؼ�֡
	const std::vector<Image*>& getFrames() const;

	// ����ÿһ֡��ʱ����
	void setInterval(
		float interval		/* ֡������룩 */
	);

	// ��ȡ֡�����Ŀ�������
	Animation * clone() const;

	// ��ȡ֡�����ĵ�ת
	Animation * reverse() const;

protected:
	float	_interval;
	std::vector<Image*> _frames;
};


// ���鶯��
class Animate :
	public Action
{
public:
	Animate();

	explicit Animate(
		Animation * animation
	);

	virtual ~Animate();

	// ��ȡ����
	virtual Animation * getAnimation() const;

	// ���ö���
	virtual void setAnimation(
		Animation * animation
	);

	// ��ȡ�ö����Ŀ�������
	virtual Animate * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual Animate * reverse() const override;

	// ���ö���
	virtual void reset() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	UINT _frameIndex;
	Animation * _animation;
};


}