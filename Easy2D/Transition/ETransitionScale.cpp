#include "..\etransitions.h"
#include "..\eactions.h"
#include "..\emanagers.h"

e2d::ETransitionScale::ETransitionScale(float scaleOutDuration, float scaleInDuration)
	: m_fScaleOutDuration(scaleOutDuration)
	, m_fScaleInDuration(scaleInDuration)
{
}

void e2d::ETransitionScale::_setTarget(EScene * prev, EScene * next, bool & transitional)
{
	// ��ʼ����������
	next->getRoot()->setScale(0);

	// ��һ��������������ʧ
	auto action1 = new EActionScaleTo(m_fScaleOutDuration, 0);
	if (prev)
	{
		action1->setTarget(prev->getRoot());
	}

	// �ڶ�����������
	auto action2 = new EActionScaleTo(m_fScaleInDuration, 1);
	action2->setTarget(next->getRoot());

	// ��־��������
	auto action3 = new EActionCallback([&, prev, next] {
		transitional = false;
		// ��ԭ����״̬
		if (prev)
		{
			prev->getRoot()->setScale(1);
		}
		next->getRoot()->setScale(1);
	});

	// ���˳����
	EActionManager::addAction(new EActionSequence(3, action1, action2, action3));
}
