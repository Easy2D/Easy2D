#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class Game;
class Renderer;
class Obj;
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
	// ��һ���ڵ�����ڴ��
	static void preload(
		e2d::Obj * nptr
	);

	// ֪ͨ�ڴ��ˢ��
	static void notifyFlush();

private:
	// ˢ���ڴ��
	static void __flush();
};


// ����������
class SceneManager
{
	friend Game;
	friend Renderer;

public:
	// �л�����
	static void enterScene(
		Scene * scene,						/* ��һ��������ָ�� */
		Transition * transition = nullptr,	/* �����л����� */
		bool saveCurrentScene = true		/* �Ƿ񱣴浱ǰ���� */
	);

	// ������һ����
	static void backScene(
		Transition * transition = nullptr	/* �����л����� */
	);

	// ��ձ�������г���
	static void clearScene();

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

public:
	// �󶨶�ʱ��������
	static void preload(
		Timer * pTimer,
		Scene * pParentScene
	);

	// �󶨶�ʱ�����ڵ�
	static void preload(
		Timer * pTimer,
		Node * pParentNode
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void startTimers(
		const String &name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stopTimers(
		const String &name
	);

	// ɾ��������ͬ���ƵĶ�ʱ��
	static void deleteTimers(
		const String &name
	);

	// �������ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		Scene * pParentScene
	);

	// ֹͣ���ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		Scene * pParentScene
	);

	// �������ڽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		Node * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		Node * pParentNode
	);

	// �������ж�ʱ��
	static void startAllTimers();

	// ֹͣ���ж�ʱ��
	static void stopAllTimers();

private:
	// ���¶�ʱ��
	static void __update();

	// ��հ��ڽڵ��ϵ����ж�ʱ��
	static void __clearAllTimersBindedWith(
		Node * pParentNode
	);

	// ���ö�ʱ��״̬
	static void __resetAllTimers();
};


// ����������
class ActionManager
{
	friend Game;
	friend Node;
	friend Action;

public:
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

private:
	// ��Ӷ���
	static void _add(
		Action * pAction,
		Node * pTargetNode
	);

	// ���¶���״̬
	static void __update();

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