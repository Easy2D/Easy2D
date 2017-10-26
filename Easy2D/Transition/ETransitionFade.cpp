#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\emanagers.h"

e2d::ETransitionFade::ETransitionFade(float fadeOutDuration, float fadeInDuration)
	: m_fFadeOutDuration(fadeOutDuration)
	, m_fFadeInDuration(fadeInDuration)
{
}

void e2d::ETransitionFade::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	// ��ʼ����������
	next->getRoot()->setOpacity(0);

	// ��һ����������
	auto action1 = new EActionFadeOut(m_fFadeOutDuration);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}
	
	// �ڶ�����������
	auto action2 = new EActionFadeIn(m_fFadeInDuration);
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
	EActionManager::addAction(new EActionSequence(3, action1, action2, action3));
}