#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"

namespace e2d
{

class Game;
class Input;
class Renderer;
class Node;
class Timer;
class Action;
class Player;
class Collider;
class Transition;

// ���������
class ObjectManager
{
	friend Game;
	friend Object;

public:
	// �ͷ�����������ڴ�ռ�
	static void flush();

private:
	// ����������ڴ�ؽ��й���
	static void __add(
		Object * pObject
	);

	// ���¶��������
	static void __update();

	// ������ж���
	static void __clear();
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


// ����������
class ActionManager
{
	friend Game;
	friend Node;
	friend Action;

public:
	// ִ�ж���
	static void start(
		Action * action,
		Node * target,
		bool paused
	);

	// ����������ͬ�����ж���
	static void resume(
		const String& strActionName
	);

	// ��ͣ������ͬ�����ж���
	static void pause(
		const String& strActionName
	);

	// ֹͣ������ͬ�����ж���
	static void stop(
		const String& strActionName
	);

	// �������ж���
	static void resumeAll();

	// ��ͣ���ж���
	static void pauseAll();

	// ֹͣ���ж���
	static void stopAll();

	// ��ȡ����������ͬ�Ķ���
	static std::vector<Action *> get(
		const String& strActionName
	);

	// ��ȡ���ж���
	static const std::vector<Action*>& getAll();

private:
	// ���¶���״̬
	static void __update();

	// ��Ӷ���
	static void __add(
		Action * action
	);

	// ɾ������
	static void __remove(
		Action * action
	);

	// �������ڽڵ��ϵ����ж���
	static void __resumeAllBindedWith(
		Node * target
	);

	// ��ͣ���ڽڵ��ϵ����ж���
	static void __pauseAllBindedWith(
		Node * target
	);

	// ֹͣ���ڽڵ��ϵ����ж���
	static void __stopAllBindedWith(
		Node * target
	);

	// ��հ��ڽڵ��ϵ����ж���
	static void __clearAllBindedWith(
		Node * target
	);

	// �������ж���״̬
	static void __resetAll();

	// ������Դ
	static void __uninit();
};


// ���̺������Ϣ������
class InputManager
{
	friend Game;
	friend Input;

public:
	// ����������
	static void add(
		const Function& func,		/* �������û�����ʱ��ִ�к��� */
		const String& name = L"",	/* ���������� */
		bool paused = false			/* �Ƿ���ͣ */
	);

	// ��ͣ�������
	static void pause(
		const String& name
	);

	// ��ͣ�������
	static void resume(
		const String& name
	);

	// ֹͣ�������
	static void stop(
		const String& name
	);

	// ��ͣ���м�����
	static void pauseAll();

	// �������м�����
	static void resumeAll();

	// ֹͣ���м�����
	static void stopAll();

private:
	// ���¼�����
	static void __update();

	// ������Դ
	static void __uninit();
};


// ��ײ������
class ColliderManager
{
	friend Game;
	friend Node;
	friend Collider;

public:
	// ������ر���ײ�������ܣ�Ĭ�Ϲرգ�
	static void setEnable(
		bool enable
	);

	// �����ײ����
	static void add(
		const Function& func,		/* ��������ײʱ��ִ�к��� */
		const String& name = L"",	/* ���������� */
		bool paused = false			/* �Ƿ���ͣ */
	);

	// ��ͣ��ײ����
	static void pause(
		const String& name
	);

	// ��ͣ��ײ����
	static void resume(
		const String& name
	);

	// ֹͣ��ײ����
	static void stop(
		const String& name
	);

	// ��ͣ���м�����
	static void pauseAll();

	// �������м�����
	static void resumeAll();

	// ֹͣ���м�����
	static void stopAll();

private:
	// ���¼�����
	static void __update();

	// ������ײ��
	static void __updateCollider(
		Collider * pActiveCollider
	);

	// �����ײ��
	static void __addCollider(
		Collider * pCollider
	);

	// ɾ���Ѱ󶨵���ײ��
	static void __removeCollider(
		Collider * pCollider
	);

	// ������Դ
	static void __uninit();
};

}