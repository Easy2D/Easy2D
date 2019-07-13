#include <easy2d.h>

int main()
{
	// ��ʼ����Ϸ
	Game::init();
	// ����һ������
	auto scene = Create<Scene>();
	// ����ó���
	SceneManager::enter(scene);
	// ����һ���ı�
	auto text = Create<Text>(L"Hello Easy2D!");
	// �����ı����ĵ�
	text->setPivot(0.5, 0.5);
	// �ı�λ�þ���
	text->setPos(Window::getSize() / 2);
	// ������ı���ӵ�������
	scene->add(text);
	// ��ʼ��Ϸ
	Game::start();
	return 0;
}
