#include <easy2d.h>

int main()
{
	// 初始化游戏
	Game::init();
	// 创建一个场景
	auto scene = Create<Scene>();
	// 进入该场景
	SceneManager::enter(scene);
	// 创建一个文本
	auto text = Create<Text>(L"Hello Easy2D!");
	// 设置文本中心点
	text->setPivot(0.5, 0.5);
	// 文本位置居中
	text->setPos(Window::getSize() / 2);
	// 将这个文本添加到场景中
	scene->add(text);
	// 开始游戏
	Game::start();
	return 0;
}
