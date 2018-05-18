#pragma once
#include "e2dnode.h"

namespace e2d
{


class ActionManager;
class Loop;
class Sequence;
class Spawn;


// ��������
class Action :
	public Object
{
	friend ActionManager;
	friend Loop;
	friend Sequence;
	friend Spawn;

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

	// ���ٶ���
	virtual void onDestroy() override;

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
	double	_last;
};


// ��������
class ActionGradual :
	public Action
{
public:
	// �����ض�ʱ���ĳ�������
	ActionGradual(
		double duration
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
	double _duration;
	double _delta;
};


// ���λ�ƶ���
class MoveBy :
	public ActionGradual
{
public:
	MoveBy(
		double duration,	/* ����ʱ�� */
		Vector vector		/* �ƶ����� */
	);

	// �������λ�ƶ���
	static MoveBy * create(
		double duration,	/* ����ʱ�� */
		Vector vector		/* �ƶ����� */
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
	Vector	_deltaPos;
};


// λ�ƶ���
class MoveTo :
	public MoveBy
{
public:
	MoveTo(
		double duration,	/* ����ʱ�� */
		Point pos			/* Ŀ������ */
	);

	// ����λ�ƶ���
	static MoveTo * create(
		double duration,	/* ����ʱ�� */
		Point pos			/* Ŀ������ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual MoveTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual MoveTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in MoveTo");
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
	public ActionGradual
{
public:
	JumpBy(
		double duration,		/* ����ʱ�� */
		const Vector& vec,		/* ��Ծ���� */
		double height,			/* ��Ծ�߶� */
		int jumps				/* ��Ծ���� */
	);

	// ���������Ծ����
	static JumpBy * create(
		double duration,		/* ����ʱ�� */
		const Vector& vec,		/* ��Ծ���� */
		double height,			/* ��Ծ�߶� */
		int jumps				/* ��Ծ���� */
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
	Vector	_deltaPos;
	double	_height;
	int		_jumps;
	Point	_prevPos;
};


// ��Ծ����
class JumpTo :
	public JumpBy
{
public:
	JumpTo(
		double duration,		/* ����ʱ�� */
		const Point& pos,		/* Ŀ������ */
		double height,			/* ��Ծ�߶� */
		int jumps				/* ��Ծ���� */
	);

	// ����λ�ƶ���
	static JumpTo * create(
		double duration,		/* ����ʱ�� */
		const Point& pos,		/* Ŀ������ */
		double height,			/* ��Ծ�߶� */
		int jumps				/* ��Ծ���� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual JumpTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual JumpTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in JumpTo");
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
	public ActionGradual
{
public:
	ScaleBy(
		double duration,	/* ����ʱ�� */
		double scale		/* ��Ա仯ֵ */
	);

	ScaleBy(
		double duration,	/* ����ʱ�� */
		double scaleX,		/* ����������Ա仯ֵ */
		double scaleY		/* ����������Ա仯ֵ */
	);

	// ����������Ŷ���
	static ScaleBy * create(
		double duration,	/* ����ʱ�� */
		double scale		/* ��Ա仯ֵ */
	);

	// ����������Ŷ���
	static ScaleBy * create(
		double duration,	/* ����ʱ�� */
		double scaleX,		/* ����������Ա仯ֵ */
		double scaleY		/* ����������Ա仯ֵ */
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
	double	_startScaleX;
	double	_startScaleY;
	double	_deltaX;
	double	_deltaY;
};


// ���Ŷ���
class ScaleTo :
	public ScaleBy
{
public:
	ScaleTo(
		double duration,	/* ����ʱ�� */
		double scale		/* Ŀ��ֵ */
	);

	ScaleTo(
		double duration,	/* ����ʱ�� */
		double scaleX,		/* ��������Ŀ��ֵ */
		double scaleY		/* ��������Ŀ��ֵ */
	);

	// �������Ŷ���
	static ScaleTo * create(
		double duration,	/* ����ʱ�� */
		double scale		/* Ŀ��ֵ */
	);

	// �������Ŷ���
	static ScaleTo * create(
		double duration,	/* ����ʱ�� */
		double scaleX,		/* ��������Ŀ��ֵ */
		double scaleY		/* ��������Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ScaleTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual ScaleTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in ScaleTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	double	_endScaleX;
	double	_endScaleY;
};


// ͸������Խ��䶯��
class OpacityBy :
	public ActionGradual
{
public:
	OpacityBy(
		double duration,	/* ����ʱ�� */
		double opacity		/* ��Ա仯ֵ */
	);

	// ����͸������Խ��䶯��
	static OpacityBy * create(
		double duration,	/* ����ʱ�� */
		double opacity		/* ��Ա仯ֵ */
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
	double _startVal;
	double _deltaVal;
};


// ͸���Ƚ��䶯��
class OpacityTo :
	public OpacityBy
{
public:
	OpacityTo(
		double duration,	/* ����ʱ�� */
		double opacity		/* Ŀ��ֵ */
	);

	// ����͸���Ƚ��䶯��
	static OpacityTo * create(
		double duration,	/* ����ʱ�� */
		double opacity		/* Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual OpacityTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual OpacityTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in OpacityTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	double _endVal;
};


// ���붯��
class FadeIn :
	public OpacityTo
{
public:
	// �������붯��
	FadeIn(
		double duration		/* ����ʱ�� */
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
	FadeOut(
		double duration		/* ����ʱ�� */
	)
	: OpacityTo(duration, 0) 
	{
	}
};


// �����ת����
class RotateBy :
	public ActionGradual
{
public:
	RotateBy(
		double duration,	/* ����ʱ�� */
		double rotation		/* ��Ա仯ֵ */
	);

	// ���������ת����
	static RotateBy * create(
		double duration,	/* ����ʱ�� */
		double rotation		/* ��Ա仯ֵ */
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
	double _startVal;
	double _deltaVal;
};


// ��ת����
class RotateTo :
	public RotateBy
{
public:
	RotateTo(
		double duration,	/* ����ʱ�� */
		double rotation		/* Ŀ��ֵ */
	);

	// ������ת����
	static RotateTo * create(
		double duration,	/* ����ʱ�� */
		double rotation		/* Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual RotateTo * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual RotateTo * reverse() const override
	{
		ASSERT(false, "reverse() not supported in RotateTo");
		return nullptr;
	}

protected:
	// ��ʼ������
	virtual void _init() override;

protected:
	double _endVal;
};


// ��ʱ����
class Delay :
	public Action
{
public:
	Delay(
		double duration	/* �ӳ�ʱ�����룩 */
	);

	// ������ʱ����
	static Delay * create(
		double duration	/* �ӳ�ʱ�����룩 */
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
	double _delay;
	double _delta;
};


// ѭ������
class Loop :
	public Action
{
public:
	Loop(
		Action * action,	/* ִ��ѭ���Ķ��� */
		int times = -1		/* ѭ������ */
	);

	// ����ѭ������
	static Loop * create(
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

	// ���ٶ���
	virtual void onDestroy() override;

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
	CallFunc(
		const Function& func /* �������� */
	);

	// ����ִ�к�������Ķ���
	static CallFunc * create(
		const Function& func /* �������� */
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
	Function _func;
};


// ˳����
class Sequence :
	public Action
{
public:
	Sequence();

	Sequence(
		const std::vector<Action*>& actions	/* �����б� */
	);

	// ����˳����
	static Sequence * create();

	// ����˳����
	static Sequence * create(
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

	// ���ٶ���
	virtual void onDestroy() override;

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

	Spawn(
		const std::vector<Action*>& actions	/* �����б� */
	);

	// ����ͬ������
	static Spawn * create();

	// ����ͬ������
	static Spawn * create(
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

	// ���ٶ���
	virtual void onDestroy() override;

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

	Animation(
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	Animation(
		double interval						/* ֡������룩 */
	);

	Animation(
		double interval,					/* ֡������룩 */
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	// ����֡����
	static Animation * create();

	// ����֡����
	static Animation * create(
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	// �����ض�֡�����֡����
	static Animation * create(
		double interval						/* ֡������룩 */
	);

	// �����ض�֡�����֡����
	static Animation * create(
		double interval,					/* ֡������룩 */
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
	double getInterval() const;

	// ��ȡ�ؼ�֡
	const std::vector<Image*>& getFrames() const;

	// ����ÿһ֡��ʱ����
	void setInterval(
		double interval		/* ֡������룩 */
	);

	// ��ȡ�����Ŀ�������
	virtual Animation * clone() const;

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	double	_interval;
	std::vector<Image*> _frames;
};


// ���鶯��
class Animate :
	public Action
{
public:
	Animate();

	Animate(
		Animation * animation
	);

	// �������鶯��
	static Animate * create();

	// �������鶯��
	static Animate * create(
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

	// ���ٶ���
	virtual void onDestroy() override;

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