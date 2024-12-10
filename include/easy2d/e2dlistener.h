#pragma once
#include <easy2d/e2devent.h>

namespace easy2d
{

class Node;

// 事件监听器基类
class ListenerBase
	: public Object
{
	friend class Node;

public:
	// 启动监听
	void start();

	// 停止监听
	void stop();

	// 获取监听器运行状态
	bool isRunning() const;

	// 游戏暂停时仍然运行
	void ignoreGamePaused();

protected:
	ListenerBase();

	// 处理事件
	virtual void handle(Node* target, Event* evt) = 0;

	// 下一次处理时移除该监听器
	void done();

private:
	bool isDone();

	void receive(Node* target, Event* evt);

protected:
	bool _running;
	bool _done;
	bool _ignoreGamePaused;
};


// 事件监听器
class Listener
	: public ListenerBase
{
	friend class Input;

public:
	using Callback = Function<void(Event* /* evt */)>;

	Listener();

	Listener(
		const Callback& func,
		const String& name = String{},
		bool paused = false
	);

	// 设置监听回调函数
	void setCallback(
		const Callback& func
	);

protected:
	// 处理事件
	virtual void handle(Node* target, Event* evt) override;

protected:
	Callback _callback;
};


// 按钮事件
enum class ButtonEvent
{
	MouseEntered,  // 鼠标浮于按钮上
	MouseExited,   // 鼠标从按钮上移出
	Pressed,       // 鼠标按下
	Clicked,       // 鼠标点击
};


// 按钮事件监听器基类
class ButtonListenerBase
	: public ListenerBase
{
protected:
	ButtonListenerBase();

	virtual void handle(Node* target, ButtonEvent evt) = 0;

	virtual void handle(Node* target, Event* evt) override;

protected:
	bool _isHover;
	bool _isPressed;
};


// 按钮事件监听器
class ButtonListener
	: public ButtonListenerBase
{
public:
	using Callback = Function<void(ButtonEvent /* evt */)>;

	ButtonListener();

	ButtonListener(
		const Callback& func
	);

	// 设置监听回调函数
	void setCallback(
		const Callback& func
	);

protected:
	// 处理事件
	virtual void handle(Node* target, ButtonEvent evt) override;

protected:
	Callback _callback;
};


// 开关按钮事件监听器
class ToggleButtonListener
	: public ButtonListenerBase
{
public:
	using Callback = Function<void(ButtonEvent /* evt */, bool /* state */)>;

	ToggleButtonListener();

	ToggleButtonListener(
		bool toggle,
		const Callback& func
	);

	// 设置监听回调函数
	void setCallback(
		const Callback& func
	);

	// 获取开关状态
	bool getState() const;

	// 设置开关状态
	void setState(bool state);

	// 切换开关状态
	void toggle();

protected:
	// 处理事件
	virtual void handle(Node* target, ButtonEvent evt) override;

protected:
	bool _state;
	Callback _callback;
};

}
