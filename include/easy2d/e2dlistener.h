#pragma once
#include <easy2d/e2devent.h>

namespace easy2d
{

class Node;

// �¼�����������
class ListenerBase
	: public Object
{
	friend class Node;

public:
	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ��ȡ����������״̬
	bool isRunning() const;

	// ��Ϸ��ͣʱ��Ȼ����
	void ignoreGamePaused();

protected:
	ListenerBase();

	// �����¼�
	virtual void handle(Node* target, Event* evt) = 0;

	// ��һ�δ���ʱ�Ƴ��ü�����
	void done();

private:
	bool isDone();

	void receive(Node* target, Event* evt);

protected:
	bool _running;
	bool _done;
	bool _ignoreGamePaused;
};


// �¼�������
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

	// ���ü����ص�����
	void setCallback(
		const Callback& func
	);

protected:
	// �����¼�
	virtual void handle(Node* target, Event* evt) override;

protected:
	Callback _callback;
};


// ��ť�¼�
enum class ButtonEvent
{
	MouseEntered,  // ��긡�ڰ�ť��
	MouseExited,   // ���Ӱ�ť���Ƴ�
	Pressed,       // ��갴��
	Clicked,       // �����
};


// ��ť�¼�����������
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


// ��ť�¼�������
class ButtonListener
	: public ButtonListenerBase
{
public:
	using Callback = Function<void(ButtonEvent /* evt */)>;

	ButtonListener();

	ButtonListener(
		const Callback& func
	);

	// ���ü����ص�����
	void setCallback(
		const Callback& func
	);

protected:
	// �����¼�
	virtual void handle(Node* target, ButtonEvent evt) override;

protected:
	Callback _callback;
};


// ���ذ�ť�¼�������
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

	// ���ü����ص�����
	void setCallback(
		const Callback& func
	);

	// ��ȡ����״̬
	bool getState() const;

	// ���ÿ���״̬
	void setState(bool state);

	// �л�����״̬
	void toggle();

protected:
	// �����¼�
	virtual void handle(Node* target, ButtonEvent evt) override;

protected:
	bool _state;
	Callback _callback;
};

}
