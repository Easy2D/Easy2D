#pragma once
#include "e2dcommon.h"

namespace e2d
{


class SceneManager;


// �������ɶ���
class Transition :
	public Object
{
	friend SceneManager;

public:
	Transition(double duration);

	virtual ~Transition();

	// �����л������Ƿ����
	bool isDone();

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ��ʼ����������
	virtual void _init(
		Scene * prev,
		Scene * next
	);

	// ���³�������
	virtual void _update();

	// ���³�������
	virtual void _updateCustom() = 0;

	// ��Ⱦ��������
	virtual void _render();

	// ���ó�������
	virtual void _reset() = 0;

	// ֹͣ��������
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


class TransitionFade :
	public Transition
{
public:
	TransitionFade(
		double duration	/* ��������ʱ�� */
	);

	TransitionFade(
		double fadeOutDuration,	/* ǰһ����������������ʱ�� */
		double fadeInDuration	/* ��һ�������붯������ʱ�� */
	);

	// �������뵭��ʽ�ĳ����л�����
	static TransitionFade * create(
		double duration	/* ��������ʱ�� */
	);

	// �������뵭��ʽ�ĳ����л�����
	static TransitionFade * create(
		double fadeOutDuration,	/* ǰһ����������������ʱ�� */
		double fadeInDuration	/* ��һ�������붯������ʱ�� */
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
	double _fadeOutDuration;
	double _fadeInDuration;
	bool _fadeOutTransioning;
};


class TransitionEmerge :
	public Transition
{
public:
	TransitionEmerge(
		double duration	/* ���ֶ�������ʱ�� */
	);

	// ��������ʽ�ĳ����л�����
	static TransitionEmerge * create(
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


class TransitionMove :
	public Transition
{
public:
	TransitionMove(
		double moveDuration,					/* �����ƶ���������ʱ�� */
		Direction direction = Direction::LEFT	/* �����ƶ����� */
	);

	// �����ƶ�ʽ�ĳ����л�����
	static TransitionMove * create(
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