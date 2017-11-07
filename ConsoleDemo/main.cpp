#include <easy2d.h>

int main()
{
	EApp app;

	if (app.init(L"Easy2D Demo", 640, 480))
	{
		app.showConsole();
		auto scene = new EScene();

		// ����һ���ı�
		auto btnNormal = new EText(L"��");
		auto btnSelected = new EText(L"��");
		btnSelected->movePosY(2);
		auto btnNormal2 = new EText(L"��");
		auto btnSelected2 = new EText(L"��");
		btnSelected2->movePosY(2);
		// ����һ����ť
		auto button = new EButtonToggle(btnNormal, btnNormal2, btnSelected, btnSelected2);
		button->setCallback([=]() {
			//button->setEnable(false);
		});
		button->toggle();
		button->setPos(EApp::getWidth() / 2, EApp::getHeight() / 2);
		scene->add(button);

		// ������ť
		auto button2 = new EButton(new EText(L"�ر�"), [=]() { button->setEnable(!button->isEnable()); });
		button2->setPos(40, 40);
		scene->add(button2);

		app.enterScene(scene);

		app.run();
	}

	return 0;
}
