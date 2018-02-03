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
class EMusic;
class EShape;
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

	// ��ʼ������
	static bool __init();

	// ���ճ�����Դ
	static void __uninit();
};


// ��ʱ��������
class ETimerManager
{
	friend EGame;
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


// ���ֹ�����
class EMusicManager
{
	friend EGame;

public:
	// ��������ļ�
	static bool add(
		const EString & strFilePath	/* �����ļ�·�� */
	);

	// ��ȡָ�����ֵ� EMusic ����
	static EMusic * get(
		const EString & strFilePath	/* �����ļ�·�� */
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


class EShapeManager
{
	friend EGame;
	friend ENode;
	friend EShape;

private:
	// ������״
	static void __updateShape(
		EShape * pActiveShape
	);

	// �����״
	static void __addShape(
		EShape * pShape
	);

	// ɾ���Ѱ󶨵���״
	static void __delShape(
		EShape * pShape
	);
};

}