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
	virtual Action * reverse() const;

	// ���ö���
	virtual void reset();

	// ��ȡ�ö�����ִ��Ŀ��
	virtual Node * getTarget();

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ��ʼ������
	virtual void _init();

	// ִ�ж���
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
	double	_fLast;
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

protected:
	// ��ʼ������
	virtual void _init() override;

	// ���¶���
	virtual void _update() override;

protected:
	double _duration;
	double _delta;
};


// ���λ�ƶ���
class MoveBy :
	public ActionGradual
{
public:
	// �������λ�ƶ���
	MoveBy(
		double duration,	/* ��������ʱ�� */
		Vector vector		/* λ������ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual MoveBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual MoveBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
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
	// ����λ�ƶ���
	MoveTo(
		double duration,	/* ��������ʱ�� */
		Point pos			/* λ����Ŀ�������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual MoveTo * clone() const override;

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
	// ����������Ŷ���
	ScaleBy(
		double duration,	/* ��������ʱ�� */
		double scale		/* ���ű����仯 */
	);

	// ����������Ŷ���
	ScaleBy(
		double duration,	/* ��������ʱ�� */
		double scaleX,		/* �������ű����仯 */
		double scaleY		/* �������ű����仯 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ScaleBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual ScaleBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
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
	// �������Ŷ���
	ScaleTo(
		double duration,	/* ��������ʱ�� */
		double scale		/* ������Ŀ����� */
	);

	// �������Ŷ���
	ScaleTo(
		double duration,	/* ��������ʱ�� */
		double scaleX,		/* ����������Ŀ����� */
		double scaleY		/* ����������Ŀ����� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual ScaleTo * clone() const override;

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
	// ����͸������Խ��䶯��
	OpacityBy(
		double duration,	/* ��������ʱ�� */
		double opacity		/* ͸������Ա仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual OpacityBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual OpacityBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
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
	// ����͸���Ƚ��䶯��
	OpacityTo(
		double duration,	/* ��������ʱ�� */
		double opacity		/* ͸���Ƚ�����Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual OpacityTo * clone() const override;

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
		double duration		/* ��������ʱ�� */
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
		double duration		/* ��������ʱ�� */
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
	// ���������ת����
	RotateBy(
		double duration,	/* ��������ʱ�� */
		double rotation		/* ��ת�Ƕȱ仯ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual RotateBy * clone() const override;

	// ��ȡ�ö����ĵ�ת
	virtual RotateBy * reverse() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
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
	// ������ת����
	RotateTo(
		double duration,	/* ��������ʱ�� */
		double rotation		/* ��ת�Ƕ���Ŀ��ֵ */
	);

	// ��ȡ�ö����Ŀ�������
	virtual RotateTo * clone() const override;

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
	// ������ʱ����
	Delay(
		double duration	/* �ӳ�ʱ�����룩 */
	);

	// ��ȡ�ö����Ŀ�������
	virtual Delay * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double _delay;
};


// ѭ������
class Loop :
	public Action
{
public:
	// ����ѭ������
	Loop(
		Action * action,	/* ִ��ѭ���Ķ��� */
		int times = -1		/* ѭ������ */
	);

	virtual ~Loop();

	// ��ȡ�ö����Ŀ�������
	virtual Loop * clone() const override;

	// ���ö���
	virtual void reset() override;

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
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
	// ����ִ�к�������Ķ���
	CallFunc(
		const Function& func /* �������� */
	);

	// ��ȡ�ö����Ŀ�������
	virtual CallFunc * clone() const override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	Function _func;
};


// ˳����
class Sequence :
	public Action
{
public:
	// ����˳����
	Sequence();

#ifdef HIGHER_THAN_VS2012
	// ����˳����
	Sequence(
		const std::initializer_list<Action*>& vActions	/* �����б� */
	);
#else
	// ����˳����
	Sequence(
		int number,				/* �������� */
		Action * action,	/* ��һ������ */
		...
	);
#endif

	virtual ~Sequence();

	// �ڽ�β��Ӷ���
	void add(
		Action * action
	);

#ifdef HIGHER_THAN_VS2012
	// �ڽ�β��Ӷ������
	void add(
		const std::initializer_list<Action*>& vActions	/* �����б� */
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

	// ִ�ж���
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
	// ����ͬ������
	Spawn();

#ifdef HIGHER_THAN_VS2012
	// ����ͬ������
	Spawn(
		const std::initializer_list<Action*>& vActions	/* �����б� */
	);
#else
	// ����ͬ������
	Spawn(
		int number,			/* �������� */
		Action * action,	/* ��һ������ */
		...
	);
#endif

	virtual ~Spawn();

	// �ڽ�β��Ӷ���
	void add(
		Action * action
	);

#ifdef HIGHER_THAN_VS2012
	// �ڽ�β��Ӷ������
	void add(
		const std::initializer_list<Action*>& vActions	/* �����б� */
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

	// ִ�ж���
	virtual void _update() override;

	// ���ö���ʱ��
	virtual void _resetTime() override;

protected:
	std::vector<Action*> _actions;
};


// ֡����
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

	// ��ȡ�ö����ĵ�ת
	virtual Animation * reverse() const override;

	// ���ö���
	virtual void reset() override;

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ��ʼ������
	virtual void _init() override;

	// ִ�ж���
	virtual void _update() override;

protected:
	double	_interval;
	UINT	_frameIndex;
	std::vector<Image*> _frames;
};


}