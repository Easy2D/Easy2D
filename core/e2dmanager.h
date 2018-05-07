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
class ActionBase;
class Music;
class Collider;
class TransitionBase;

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
		TransitionBase * transition = nullptr,	/* �����л����� */
		bool saveCurrentScene = true		/* �Ƿ񱣴浱ǰ���� */
	);

	// ������һ����
	static void back(
		TransitionBase * transition = nullptr	/* �����л����� */
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
	friend ActionBase;

public:
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
	static std::vector<ActionBase *> get(
		const String& strActionName
	);

	// ��ȡ���ж���
	static std::vector<ActionBase*> getAll();

private:
	// ���¶���״̬
	static void __update();

	// ��Ӷ���
	static void __add(
		ActionBase * pAction
	);

	// ɾ������
	static void __remove(
		ActionBase * pAction
	);

	// ִ�ж���
	static void __startAction(
		ActionBase * pAction,
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
		Function func,				/* �������û�����ʱ��ִ�к��� */
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
		Function func,				/* ��������ײʱ��ִ�к��� */
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

	// ��ȡ��ײ����ʱ��������
	static Node * getActiveNode();

	// ��ȡ��ײ����ʱ�ı�����
	static Node * getPassiveNode();

	// �ж���ײ�Ƿ��ɸýڵ�����
	// ����ǣ�����������ײ�Ľڵ�ָ�룬���򷵻ؿ�
	static Node * isCausedBy(
		Node * pNode
	);

	// �жϷ�����ײ�Ľڵ������Ƿ���ͬ
	// ����ͬ��������ָ�룬���򷵻ؿ�
	static Node * isCausedBy(
		const String& name
	);

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