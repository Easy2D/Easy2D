#include <easy2d.h>

int main()
{
	if (Game::init())								// ��ʼ����Ϸ
	{
		auto scene = gcnew Scene;					// ����һ������
		SceneManager::enter(scene);					// ����ó���
		
		auto text = gcnew Text(L"Hello Easy2D!");	// ����һ���ı�
		text->setPivot(0.5, 0.5);					// �����ı����ĵ�
		text->setPos(Window::getSize() / 2);		// �ı�λ�þ���
		scene->add(text);							// ������ı���ӵ�������

		Game::start();								// ��ʼ��Ϸ
	}
	Game::destroy();
	return 0;
}
