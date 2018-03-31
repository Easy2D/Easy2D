#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class Game;
class Input;
class Renderer;
class Node;
class Timer;
class Action;
class Music;
class ShapeBase;
class Transition;
class InputListener;
class CollisionListener;

// ���������
class ObjectManager
{
	friend Game;

public:
	// ��һ����������ڴ��
	static void add(
		Object * nptr
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

	// ��ȡ����ջ
	static std::stack<Scene*> getSceneStack();

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
		double timeOut,	/* �ȴ���ʱ�����룩 */
		Function func	/* ִ�еĺ��� */
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void start(
		String name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stop(
		String name
	);

	// ɾ��������ͬ���ƵĶ�ʱ��
	static void clear(
		String name
	);

	// ��ȡ������ͬ�Ķ�ʱ��
	static std::vector<Timer*> get(
		String name
	);

	// �������ж�ʱ��
	static void startAll();

	// ֹͣ���ж�ʱ��
	static void stopAll();

	// ֹͣ��������ж�ʱ��
	static void stopAndClearAll();

	// ��ȡ���ж�ʱ��
	static std::vector<Timer*> getAll();

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
	static void resume(
		String strActionName
	);

	// ��ͣ������ͬ�����ж���
	static void pause(
		String strActionName
	);

	// ֹͣ������ͬ�����ж���
	static void stop(
		String strActionName
	);

	// �������ж���
	static void resumeAll();

	// ��ͣ���ж���
	static void pauseAll();

	// ֹͣ���ж���
	static void stopAll();

	// ��ȡ����������ͬ�Ķ���
	static std::vector<Action *> get(
		String strActionName
	);

	// ��ȡ���ж���
	static std::vector<Action*> getAll();

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

	// �������ڽڵ��ϵ����ж���
	static void __resumeAllBindedWith(
		Node * pTargetNode
	);

	// ��ͣ���ڽڵ��ϵ����ж���
	static void __pauseAllBindedWith(
		Node * pTargetNode
	);

	// ֹͣ���ڽڵ��ϵ����ж���
	static void __stopAllBindedWith(
		Node * pTargetNode
	);

	// ��հ��ڽڵ��ϵ����ж���
	static void __clearAllBindedWith(
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
		String strFilePath	/* �����ļ�·�� */
	);

	// ��������
	static bool play(
		String strFilePath,	/* �����ļ�·�� */
		int nLoopCount = 0	/* �ظ����Ŵ��������� -1 Ϊѭ������ */
	);

	// ��ͣ����
	static void pause(
		String strFilePath	/* �����ļ�·�� */
	);

	// ������������
	static void resume(
		String strFilePath	/* �����ļ�·�� */
	);

	// ֹͣ����
	static void stop(
		String strFilePath	/* �����ļ�·�� */
	);

	// ��ȡָ�����ֵ� Music ����
	static Music * get(
		String strFilePath	/* �����ļ�·�� */
	);

	// ��ͣ��������
	static void pauseAll();

	// ����������������
	static void resumeAll();

	// ֹͣ��������
	static void stopAll();

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


// ���̺������Ϣ������
class InputManager
{
	friend Input;
	friend InputListener;

public:
	// ����������
	static void add(
		Function func,		/* �������û�����ʱ��ִ�к��� */
		String name = L""	/* ���������� */
	);

	// �����������
	static void start(
		String name
	);

	// ֹͣ�������
	static void stop(
		String name
	);

	// ����������
	static void clear(
		String name
	);

	// �������м�����
	static void startAll();

	// ֹͣ���м�����
	static void stopAll();

	// ������м�����
	static void clearAll();

	// ��ȡ������
	static std::vector<InputListener*> get(
		String name
	);

	// ��ȡȫ��������
	static std::vector<InputListener*> getAll();

private:
	// ����������
	static void __add(
		InputListener * pListener
	);

	// ���¼�����
	static void __update();
};


// ��ײ������
class CollisionManager
{
	friend Node;
	friend ShapeBase;
	friend CollisionListener;

public:
	// ������ر���ײ�������ܣ�Ĭ�Ϲرգ�
	static void setEnable(
		bool bEnable
	);

	// �����ײ����
	static void add(
		Function func,		/* ��������ײʱ��ִ�к��� */
		String name = L""	/* ���������� */
	);

	// ������ײ����
	static void start(
		String name
	);

	// ֹͣ��ײ����
	static void stop(
		String name
	);

	// �����ײ����
	static void clear(
		String name
	);

	// �������м�����
	static void startAll();

	// ֹͣ���м�����
	static void stopAll();

	// ������м�����
	static void clearAll();

	// ��ȡ������
	static std::vector<CollisionListener*> get(
		String name
	);

	// ��ȡȫ��������
	static std::vector<CollisionListener*> getAll();

	// �ж���ײ�Ƿ��ɸýڵ�����������ǣ�����������ײ�Ľڵ�ָ�룬���򷵻ؿգ�
	static Node* isCausedBy(
		Node * pNode
	);

	// �жϷ�����ײ�Ľڵ������Ƿ���ͬ������ͬ������ָ�룬���򷵻ؿգ�
	static Node* isCausedBy(
		String name
	);

private:
	// �����ײ����
	static void __add(
		CollisionListener * pListener
	);

	// ���¼�����
	static void __update();

	// ������״
	static void __updateShape(
		ShapeBase * pActiveShape
	);

	// �����״
	static void __addShape(
		ShapeBase * pShape
	);

	// ɾ���Ѱ󶨵���״
	static void __removeShape(
		ShapeBase * pShape
	);
};

}