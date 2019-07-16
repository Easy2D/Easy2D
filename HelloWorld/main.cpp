#include <easy2d.h>

using namespace easy2d;

int main()
{
	if (Game::init())								// ��ʼ����Ϸ
	{
		auto scene = gcnew Scene;					// ����һ������
		SceneManager::enter(scene);					// ����ó���
		
		auto text = gcnew Text(L"Hello Easy2D!");	// ����һ���ı�
		text->setAnchor(0.5, 0.5);					// �����ı����ĵ�
		text->setPos(Window::getSize() / 2);		// �ı�λ�þ���
		scene->addChild(text);						// ������ı���ӵ�������

		Game::start();								// ��ʼ��Ϸ
	}
	Game::destroy();
	return 0;
}
