#include "..\etransitions.h"
#include "..\eactions.h"

e2d::ETransitionFade::ETransitionFade(float fadeOutDuration, float fadeInDuration)
	: m_fFadeOutDuration(fadeOutDuration)
	, m_fFadeInDuration(fadeInDuration)
{
}

void e2d::ETransitionFade::_setTarget(EScene * prev, EScene * next, bool & enterNextScene, bool & transitional)
{
	// ��һ����������
	auto action1 = new EActionFadeOut(m_fFadeOutDuration);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}

	// �л����ڶ������������ڶ�����͸������Ϊ 0
	auto action2 = new EActionCallback([&, next] {
		enterNextScene = true;
		next->getRoot()->setOpacity(0);
	});
	
	// �ڶ�����������
	auto action3 = new EActionFadeIn(m_fFadeInDuration);
	action3->setTarget(next->getRoot());

	// ��־��������
	auto action4 = new EActionCallback([&] {
		transitional = false;
	});

	// ���˳����
	EActionManager::addAction(new EActionSequence(4, action1, action2, action3, action4));
}