#pragma once
#include "emacros.h"
#include "ecommon.h"

namespace e2d
{

class EGame;
class ERenderer;
class EObject;
class EScene;
class ENode;
class ETimer;
class EAction;
class EGeometry;
class ETransition;
class EListenerPhysics;

// ���������
class EObjectManager
{
	friend EGame;

public:
	// ��һ���ڵ�����ڴ��
	static void add(
		e2d::EObject * nptr
	);

	// ֪ͨ�ڴ��ˢ��
	static void notifyFlush();

private:
	// ˢ���ڴ��
	static void __flush();
};


// ����������
class ESceneManager
{
	friend EGame;
	friend ERenderer;

public:
	// �л�����
	static void enterScene(
		EScene * scene,						/* ��һ��������ָ�� */
		ETransition * transition = nullptr,	/* �����л����� */
		bool saveCurrentScene = true		/* �Ƿ񱣴浱ǰ���� */
	);

	// ������һ����
	static void backScene(
		ETransition * transition = nullptr	/* �����л����� */
	);

	// ��ձ�������г���
	static void clearScene();

	// ��ȡ��ǰ����
	static EScene * getCurrentScene();

private:
	// ���³�������
	static void __update();

	// ��Ⱦ��������
	static void __render();

	// ������һ����
	static void __enterNextScene();
};


// ��ʱ��������
class ETimerManager
{
	friend EGame;
	friend EScene;
	friend ENode;

public:
	// �󶨶�ʱ��������
	static void bindTimer(
		ETimer * timer,
		EScene * pParentScene
	);

	// �󶨶�ʱ�����ڵ�
	static void bindTimer(
		ETimer * timer,
		ENode * pParentNode
	);

	// ����������ͬ���ƵĶ�ʱ��
	static void startTimers(
		const EString &name
	);

	// ֹͣ������ͬ���ƵĶ�ʱ��
	static void stopTimers(
		const EString &name
	);

	// ɾ��������ͬ���ƵĶ�ʱ��
	static void delTimers(
		const EString &name
	);

	// �������ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		EScene * pParentScene
	);

	// ֹͣ���ڳ��������ӽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		EScene * pParentScene
	);

	// �������ڽڵ��ϵ����ж�ʱ��
	static void startAllTimersBindedWith(
		ENode * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����ж�ʱ��
	static void stopAllTimersBindedWith(
		ENode * pParentNode
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
		ENode * pParentNode
	);

	// ���ö�ʱ��״̬
	static void __resetAllTimers();
};


// ����������
class EActionManager
{
	friend EGame;
	friend EScene;
	friend ENode;

public:
	// ��Ӷ���
	static void addAction(
		EAction * action
	);

	// �������ڽڵ��ϵ����ж���
	static void resumeAllActionsBindedWith(
		ENode * pTargetNode
	);

	// ��ͣ���ڽڵ��ϵ����ж���
	static void pauseAllActionsBindedWith(
		ENode * pTargetNode
	);

	// ֹͣ���ڽڵ��ϵ����ж���
	static void stopAllActionsBindedWith(
		ENode * pTargetNode
	);

	// �������ж���
	static void resumeAllActions();

	// ��ͣ���ж���
	static void pauseAllActions();

	// ֹͣ���ж���
	static void stopAllActions();

private:
	// ���¶���״̬
	static void __update();

	// ��հ��ڽڵ��ϵ����ж���
	static void __clearAllActionsBindedWith(
		ENode * pTargetNode
	);

	// �������ж���״̬
	static void __resetAllActions();
};


class EPhysicsManager
{
	friend EGame;
	friend EScene;
	friend ENode;
	friend EGeometry;

public:
	// ���������볡����
	static void bindListener(
		EListenerPhysics * listener,
		EScene * pParentScene
	);

	// ����������ڵ��
	static void bindListener(
		EListenerPhysics * listener,
		ENode * pParentNode
	);

	// ����������ͬ���Ƶļ�����
	static void startListeners(
		const EString &name
	);

	// ֹͣ������ͬ���Ƶļ�����
	static void stopListeners(
		const EString &name
	);

	// ɾ��������ͬ���Ƶļ�����
	static void delListeners(
		const EString &name
	);

	// �������ڳ��������ӽڵ��ϵ����м�����
	static void startAllListenersBindedWith(
		EScene * pParentScene
	);

	// ֹͣ���ڳ��������ӽڵ��ϵ����м�����
	static void stopAllListenersBindedWith(
		EScene * pParentScene
	);

	// �������ڽڵ��ϵ����м�����
	static void startAllListenersBindedWith(
		ENode * pParentNode
	);

	// ֹͣ���ڽڵ��ϵ����м�����
	static void stopAllListenersBindedWith(
		ENode * pParentNode
	);

	// �������м�����
	static void startAllListeners();

	// ֹͣ���м�����
	static void stopAllListeners();

private:
	// ��ռ�����������
	static void _clearManager();

	// �����״
	static void _addGeometry(
		EGeometry * geometry
	);

	// ɾ���Ѱ󶨵���״
	static void _delGeometry(
		EGeometry * geometry
	);

	// ��հ��ڽڵ��ϵ����м�����
	static void _clearAllListenersBindedWith(
		ENode * pParentNode
	);

	// ����ͼ���жϳ���
	static void PhysicsGeometryProc(
		EGeometry * pActiveGeometry
	);

	// ������ײ������ִ�г���
	static void PhysicsListenerProc();
};

}