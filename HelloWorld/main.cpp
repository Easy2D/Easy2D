#include <easy2d.h>

using namespace easy2d;

int main()
{
	if (Game::init())								// 初始化游戏
	{
		auto scene = gcnew Scene;					// 创建一个场景
		SceneManager::enter(scene);					// 进入该场景
		
		auto text = gcnew Text(L"Hello Easy2D!");	// 创建一个文本
		text->setAnchor(0.5, 0.5);					// 设置文本中心点
		text->setPos(Window::getSize() / 2);		// 文本位置居中
		scene->addChild(text);						// 将这个文本添加到场景中

		Game::start();								// 开始游戏
	}
	Game::destroy();
	return 0;
}
