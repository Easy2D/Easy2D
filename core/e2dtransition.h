#pragma once
#include "e2dcommon.h"

namespace e2d
{


class SceneManager;


// ��������
class Transition :
	public Object
{
	friend SceneManager;

public:
	Transition(double duration);

	virtual ~Transition();

	// �������ɶ����Ƿ����
	bool isDone();

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ��ʼ���������ɶ���
	virtual void _init(
		Scene * prev,
		Scene * next
	);

	// ���³������ɶ���
	virtual void _update();

	// ���³������ɶ���
	virtual void _updateCustom() = 0;

	// ��Ⱦ�������ɶ���
	virtual void _render();

	// ���ó������ɶ���
	virtual void _reset() = 0;

	// ֹͣ�������ɶ���
	virtual void _stop();

protected:
	bool _end;
	double _last;
	double _duration;
	double _delta;
	Size _windowSize;
	Scene * _outScene;
	Scene * _inScene;
	ID2D1Layer * _outLayer;
	ID2D1Layer * _inLayer;
	D2D1_LAYER_PARAMETERS _outLayerParam;
	D2D1_LAYER_PARAMETERS _inLayerParam;
};


// ���뵭������
class TransitionFade :
	public Transition
{
public:
	TransitionFade(
		double duration	/* ��������ʱ�� */
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


// �������
class TransitionEmerge :
	public Transition
{
public:
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


// ��״����
class TransitionBox :
	public Transition
{
public:
	TransitionBox(
		double duration	/* ��������ʱ�� */
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


// �������
class TransitionMove :
	public Transition
{
public:
	TransitionMove(
		double moveDuration,					/* �����ƶ���������ʱ�� */
		Direction direction = Direction::LEFT	/* �����ƶ����� */
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
	Direction _direction;
	Vector _posDelta;
	Point _startPos;
};

}