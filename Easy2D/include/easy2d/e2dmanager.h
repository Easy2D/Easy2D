#pragma once
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dcommon.h>
#include <stack>
#include <vector>

namespace easy2d
{

class Game;
class Input;
class Renderer;
class Node;
class Scene;
class Timer;
class Action;
class Transition;


// 场景管理器
class SceneManager
{
	friend class Node;
	friend class Game;
	friend class Renderer;

public:
	// 切换场景
	static void enter(
		Scene * scene,						/* 下一个场景的指针 */
		Transition * transition = nullptr,	/* 场景切换动作 */
		bool saveCurrentScene = true		/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void back(
		Transition * transition = nullptr	/* 场景切换动作 */
	);

	// 清空保存的所有场景
	static void clear();

	// 获取当前场景
	static Scene * getCurrentScene();

	// 获取场景栈
	static std::stack<Scene*> getSceneStack();

	// 是否正在进行转场动作
	static bool isTransitioning();

	// 分发事件
	static void dispatch(Event* evt);

private:
	// 更新场景内容
	static void __update();

	// 渲染场景画面
	static void __render(bool showBodyShape);

	// 初始化场景
	static bool __init();

	// 回收场景资源
	static void __uninit();

	// 添加事件监听器
	static void __pushDispatcher(Node* node);
};


// 动作管理器
class ActionManager
{
	friend class Game;
	friend class Node;
	friend class Action;

public:
	// 执行动作
	static void start(
		Action * action,
		Node * target,
		bool paused
	);

	// 继续名称相同的所有动作
	static void resume(
		const String& name
	);

	// 暂停名称相同的所有动作
	static void pause(
		const String& name
	);

	// 停止名称相同的所有动作
	static void stop(
		const String& name
	);

	// 获取所有名称相同的动作
	static std::vector<Action *> get(
		const String& name
	);

	// 获取所有动作
	static const std::vector<Action*>& getAll();

private:
	// 更新动作状态
	static void __update();

	// 继续绑定在节点上的所有动作
	static void __resumeAllBoundWith(
		Node * target
	);

	// 暂停绑定在节点上的所有动作
	static void __pauseAllBoundWith(
		Node * target
	);

	// 停止绑定在节点上的所有动作
	static void __stopAllBoundWith(
		Node * target
	);

	// 清空绑定在节点上的所有动作
	static void __removeAllBoundWith(
		Node * target
	);

	// 重置所有动作状态
	static void __resetAll();

	// 回收资源
	static void __uninit();
};


}