#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{

class Scene;
class SceneManager;


// ��������
class Transition :
	public Object
{
	friend class SceneManager;

public:
	explicit Transition(float duration);

	virtual ~Transition();

	// �������ɶ����Ƿ����
	bool isDone();

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
	float _last;
	float _duration;
	float _delta;
	Size _windowSize;
	Scene * _outScene;
	Scene * _inScene;
	ID2D1Layer * _outLayer;
	ID2D1Layer * _inLayer;
	D2D1_LAYER_PARAMETERS _outLayerParam;
	D2D1_LAYER_PARAMETERS _inLayerParam;
};


// ���뵭������
class FadeTransition :
	public Transition
{
public:
	explicit FadeTransition(
		float duration	/* ��������ʱ�� */
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
class EmergeTransition :
	public Transition
{
public:
	explicit EmergeTransition(
		float duration	/* ���ֶ�������ʱ�� */
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
class BoxTransition :
	public Transition
{
public:
	explicit BoxTransition(
		float duration	/* ��������ʱ�� */
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
class MoveTransition :
	public Transition
{
public:
	explicit MoveTransition(
		float moveDuration,					/* �����ƶ���������ʱ�� */
		Direction direction = Direction::Left	/* �����ƶ����� */
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
	Vector2 _posDelta;
	Point _startPos;
};

}