#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\etools.h"

e2d::ETransitionEmerge::ETransitionEmerge(float emergeDuration)
	: m_fEmergeDuration(emergeDuration)
{
}

void e2d::ETransitionEmerge::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	// ��ʼ����������
	next->getRoot()->setOpacity(0);

	// ��һ����������
	auto action1 = new EActionFadeOut(m_fEmergeDuration);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}

	// �ڶ�����������
	auto action2 = new EActionFadeIn(m_fEmergeDuration);
	action2->setTarget(next->getRoot());

	// ��־��������
	auto action3 = new EActionCallback([&, prev, next] {
		transitional = false;
		// ��ԭ����״̬
		if (prev)
		{
			prev->getRoot()->setOpacity(1);
		}
		next->getRoot()->setOpacity(1);
	});

	// ���˳����
	EActionManager::addAction(new EActionSequence(2, new EActionTwoAtSameTime(action1, action2), action3));
}
