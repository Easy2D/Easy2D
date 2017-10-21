#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\etools.h"

e2d::ETransitionScaleEmerge::ETransitionScaleEmerge(float duration, SCALE_EMERGE_MODE mode)
	: m_fDuration(duration)
	, m_Mode(mode)
{
}

void e2d::ETransitionScaleEmerge::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	float prevScaleTo;
	float nextScaleStart;
	if (m_Mode == SCALE_EMERGE_MODE::ENTER)
	{
		prevScaleTo = 1.2f;
		nextScaleStart = 0.8f;
	}
	else
	{
		prevScaleTo = 0.8f;
		nextScaleStart = 1.2f;
	}

	// ��ʼ����������
	next->getRoot()->setScale(nextScaleStart);
	next->getRoot()->setOpacity(0);

	// ��һ����������
	auto prevActionFadeOut = new EActionFadeOut(m_fDuration);
	auto prevActionScaleTo = new EActionScaleTo(m_fDuration, prevScaleTo);
	auto action1 = new EActionTwoAtSameTime(
		prevActionFadeOut,
		prevActionScaleTo);
	if (prev)
	{
		prevActionFadeOut->setTarget(prev->getRoot());
		prevActionScaleTo->setTarget(prev->getRoot());
	}

	// �ڶ�����������
	auto nextActionFadeOut = new EActionFadeIn(m_fDuration);
	auto nextActionScaleTo = new EActionScaleTo(m_fDuration, 1);
	auto action2 = new EActionTwoAtSameTime(
		nextActionFadeOut,
		nextActionScaleTo);

	nextActionFadeOut->setTarget(next->getRoot());
	nextActionScaleTo->setTarget(next->getRoot());

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
