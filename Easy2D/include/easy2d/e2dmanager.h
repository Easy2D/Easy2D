#pragma once
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dcommon.h>

namespace easy2d
{

class Game;
class Input;
class Renderer;
class Node;
class Scene;
class Timer;
class Action;
class Transition;


// ����������
class SceneManager
{
	friend class Game;
	friend class Renderer;

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

	// �ַ��¼�
	static void dispatch(Event* evt);

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
	friend class Game;
	friend class Node;
	friend class Action;

public:
	// ִ�ж���
	static void start(
		Action * action,
		Node * target,
		bool paused
	);

	// ����������ͬ�����ж���
	static void resume(
		const String& name
	);

	// ��ͣ������ͬ�����ж���
	static void pause(
		const String& name
	);

	// ֹͣ������ͬ�����ж���
	static void stop(
		const String& name
	);

	// ��ȡ����������ͬ�Ķ���
	static std::vector<Action *> get(
		const String& name
	);

	// ��ȡ���ж���
	static const std::vector<Action*>& getAll();

private:
	// ���¶���״̬
	static void __update();

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


}