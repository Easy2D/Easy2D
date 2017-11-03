#include "..\Easy2D\easy2d.h"

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 320, 320))
	{
		auto scene = new EScene();

		auto sprite = new ESprite(L"elyse.png");
		sprite->setScale(0.3f);
		// ��ȡ���ڿ��
		float width = EApp::getWidth();
		// ��ȡ���ڸ߶�
		float height = EApp::getHeight();
		// �ƶ������λ��
		sprite->setPos(width / 2, height / 2);
		//sprite->setAnchor(0, 0);
		scene->add(sprite);
		auto text = new EText(L"balabalabalabalabala", L"����", 80, EColor::BLUE);
		//text->setWordWrapping(true);
		//text->setWordWrappingWidth(50);
		text->setAnchor(0, 0);
		
		auto listener = new EListenerMouseClick([=](EPoint) {
			EPoint p = EMouseMsg::getPos();
			sprite->setPos(p);
		});
		listener->bindWith(scene);
		EMsgManager::stopAllMouseListeners();
		EMsgManager::stopAllKeyboardListeners();
		scene->add(text, -1);
		app.enterScene(scene);

		app.run();
	}

	return 0;
}
