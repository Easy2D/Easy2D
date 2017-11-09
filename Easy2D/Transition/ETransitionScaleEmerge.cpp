#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\emanagers.h"

e2d::ETransitionScaleEmerge::ETransitionScaleEmerge(float duration, SCALE_EMERGE_MODE mode)
	: m_fDuration(duration)
	, m_Mode(mode)
{
}

void e2d::ETransitionScaleEmerge::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	float prevScaleBy;
	float nextScaleBy;
	if (m_Mode == SCALE_EMERGE_MODE::ENTER)
	{
		prevScaleBy = 0.2f;
		nextScaleBy = -0.2f;
	}
	else
	{
		prevScaleBy = -0.2f;
		nextScaleBy = 0.2f;
	}

	// ��ʼ����������
	next->getRoot()->setScale(next->getRoot()->getScaleX() - nextScaleBy, next->getRoot()->getScaleY() - nextScaleBy);
	next->getRoot()->setOpacity(0);

	// ��һ����������
	auto prevActionFadeOut = new EActionFadeOut(m_fDuration);
	auto prevActionScaleBy = new EActionScaleBy(m_fDuration, prevScaleBy);
	auto action1 = new EActionTwoAtSameTime(
		prevActionFadeOut,
		prevActionScaleBy);
	if (prev)
	{
		prevActionFadeOut->setTarget(prev->getRoot());
		prevActionScaleBy->setTarget(prev->getRoot());
	}

	// �ڶ�����������
	auto nextActionFadeOut = new EActionFadeIn(m_fDuration);
	auto nextActionScaleBy = new EActionScaleBy(m_fDuration, nextScaleBy);
	auto action2 = new EActionTwoAtSameTime(
		nextActionFadeOut,
		nextActionScaleBy);

	nextActionFadeOut->setTarget(next->getRoot());
	nextActionScaleBy->setTarget(next->getRoot());

	// ��־��������
	auto action3 = new EActionCallback([&, prev, next] {
		transitional = false;
		// ��ԭ����״̬
		if (prev)
		{
			prev->getRoot()->setScale(1);
			prev->getRoot()->setOpacity(1);
		}
		next->getRoot()->setScale(1);
		next->getRoot()->setOpacity(1);
	});

	// ���˳����
	EActionManager::addAction(new EActionSequence(2, new EActionTwoAtSameTime(action1, action2), action3));
}
