#include <easy2d.h>

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 640))
	{
		auto scene = new EScene();

		auto sprite = new ESprite(L"test2.png");
		sprite->setAnchor(-1, 0);
		// ��ȡ���ڿ��
		float width = EApp::getWidth();
		// ��ȡ���ڸ߶�
		float height = EApp::getHeight();
		// �ƶ������λ��
		sprite->setPos(width / 2, height / 2);
		//sprite->setAnchor(0, 0);
		scene->add(sprite);

		sprite->runAction(new EActionLoop(new EActionRotateBy(1, 60)));

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
