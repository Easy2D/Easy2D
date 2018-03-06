#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class Game;
class Renderer;
class Object;
class Scene;
class Node;
class Timer;
class Action;
class Music;
class Shape;
class Transition;

// ���������
class ObjectManager
{
	friend Game;

public:
	// ��һ����������ڴ��
	static void add(
		e2d::Object * nptr
	);

	// �ͷ�����������ڴ�ռ�
	static void clear();

private:
	// ���¶��������
	static void __update();

	// ������ж���
	static void __uninit();
};


// ����������
class SceneManager
{
	friend Game;
	friend Renderer;

public:
	// �л�����
	static void enter(
		Scene * scene,						/* ��һ��������ָ�� */
		Transition * transition = nullptr,	/* �����л����� */
		bool saveCurrentScene = true		/* �Ƿ񱣴浱ǰ���� */
	);

	// ������һ����
	static void back(
		Transition * transition = nullptr	/* �����л����� */
	);

	// ��ձ�������г���
	static void clear();

	// ��ȡ��ǰ����
	static Scene * getCurrentScene();

	// �Ƿ����ڽ���ת������
	static bool isTransitioning();

private:
	// ���³�������
	static void __update();

	// ��Ⱦ��������
	static void __render();

	// ��ʼ������
	static bool __init();

	// ���ճ�����Դ
	static void __uninit();
};


// ��ʱ��������
class TimerManager
{
	friend Game;
	friend Node;
	friend Timer;

public:
	// �ȴ�һ��ʱ���ִ��ָ������
	static void start(
		double timeOut,			/* �ȴ���ʱ�����룩 */
		TimerCallback callback	/* ִ�еĺ��� */
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void start(
		const String &name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stop(
		const String &name
	);

	// ɾ��������ͬ���ƵĶ�ʱ��
	static void stopAndClear(
		const String &name
	);

	// ��ȡ������ͬ�Ķ�ʱ��
	static std::vector<Timer*> getTimers(
		const String & name
	);

	// �������ж�ʱ��
	static void startAllTimers();

	// ֹͣ���ж�ʱ��
	static void stopAllTimers();

	// ֹͣ��������ж�ʱ��
	static void stopAndClearAllTimers();

	// ��ȡ���ж�ʱ��
	static std::vector<Timer*> getAllTimers();

private:
	// ���¶�ʱ��
	static void __update();

	// ���һ����ʱ��
	static void __add(
		Timer * pTimer
	);

	// ���ö�ʱ��״̬
	static void __resetAllTimers();

	// ��ն�ʱ��
	static void __uninit();
};


// ����������
class ActionManager
{
	friend Game;
	friend Node;
	friend Action;

public:
	// ����������ͬ�����ж���
	static void resumeAllActions(
		const String & strActionName
	);

	// ��ͣ������ͬ�����ж���
	static void pauseAllActions(
		const String & strActionName
	);

	// ֹͣ������ͬ�����ж���
	static void stopAllActions(
		const String & strActionName
	);

	// �������ڽڵ��ϵ����ж���
	static void resumeAllActionsBindedWith(
		Node * pTargetNode
	);

	// ��ͣ���ڽڵ��ϵ����ж���
	static void pauseAllActionsBindedWith(
		Node * pTargetNode
	);

	// ֹͣ���ڽڵ��ϵ����ж���
	static void stopAllActionsBindedWith(
		Node * pTargetNode
	);

	// �������ж���
	static void resumeAllActions();

	// ��ͣ���ж���
	static void pauseAllActions();

	// ֹͣ���ж���
	static void stopAllActions();

	// ��ȡ����������ͬ�Ķ���
	static std::vector<Action *> getActions(
		const String & strActionName
	);

	// ��ȡ���ж���
	static std::vector<Action*> getAllActions();

private:
	// ���¶���״̬
	static void __update();

	// ��Ӷ���
	static void __add(
		Action * pAction
	);

	// ɾ������
	static void __remove(
		Action * pAction
	);

	// ִ�ж���
	static void __startAction(
		Action * pAction,
		Node * pTargetNode
	);

	// ��հ��ڽڵ��ϵ����ж���
	static void __clearAllActionsBindedWith(
		Node * pTargetNode
	);

	// �������ж���״̬
	static void __resetAllActions();
};


// ���ֹ�����
class MusicManager
{
	friend Game;

public:
	// Ԥ����������Դ
	static bool preload(
		const String & strFilePath	/* �����ļ�·�� */
	);

	// ��������
	static bool play(
		const String & strFilePath,	/* �����ļ�·�� */
		int nLoopCount = 0			/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ����
	static void pause(
		const String & strFilePath	/* �����ļ�·�� */
	);

	// ������������
	static void resume(
		const String & strFilePath	/* �����ļ�·�� */
	);

	// ֹͣ����
	static void stop(
		const String & strFilePath	/* �����ļ�·�� */
	);

	// ��ȡָ�����ֵ� Music ����
	static Music * get(
		const String & strFilePath	/* �����ļ�·�� */
	);

	// ��ͣ��������
	static void pauseAllMusics();

	// ����������������
	static void resumeAllMusics();

	// ֹͣ��������
	static void stopAllMusics();

	// ��ȡ IXAudio2 ����
	static IXAudio2 * getIXAudio2();

	// ��ȡ IXAudio2MasteringVoice ����
	static IXAudio2MasteringVoice * getIXAudio2MasteringVoice();

private:
	// ��ʼ�� XAudio2
	static bool __init();

	// ���������Դ
	static void __uninit();
};


class ShapeManager
{
	friend Game;
	friend Node;
	friend Shape;

private:
	// ������״
	static void __updateShape(
		Shape * pActiveShape
	);

	// �����״
	static void __addShape(
		Shape * pShape
	);

	// ɾ���Ѱ󶨵���״
	static void __delShape(
		Shape * pShape
	);
};

}