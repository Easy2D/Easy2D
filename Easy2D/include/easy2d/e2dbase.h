#pragma once
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dcommon.h>


// Base Classes

namespace easy2d
{


// 游戏控制
class Game
{
public:
	// 初始化游戏
	static bool init(
		const String& title = L"Easy2D",	/* 窗口标题 */
		int width = 640,					/* 窗口宽度 */
		int height = 480,					/* 窗口高度 */
		const String& mutexName = L""		/* 进程互斥体名称 */
	);

	// 启动游戏
	static void start();

	// 暂停游戏
	static void pause();

	// 继续游戏
	static void resume();

	// 结束游戏
	static void quit();

	// 回收游戏资源
	static void destroy();

	// 重置游戏内部计时
	static void reset();

	// 游戏是否暂停
	static bool isPaused();

	// 获取游戏名称
	static String getName();
};


// 窗口控制
class Window
{
	friend class Game;

public:
	// 鼠标指针样式
	enum class Cursor : int
	{
		Normal,		/* 默认指针样式 */
		Hand,		/* 手状指针 */
		No,			/* 禁止指针 */
		Wait,		/* 沙漏指针 */
		ArrowWait	/* 默认指针和小沙漏 */
	};

public:
	// 修改窗口大小
	static void setSize(
		int width,			/* 窗口宽度 */
		int height			/* 窗口高度 */
	);

	// 设置窗口标题
	static void setTitle(
		const String& title	/* 窗口标题 */
	);

	// 设置窗口图标
	static void setIcon(
		int iconID
	);

	// 设置鼠标指针样式
	static void setCursor(
		Cursor cursor
	);

	// 获取窗口标题
	static String getTitle();

	// 获取窗口宽度
	static float getWidth();

	// 获取窗口高度
	static float getHeight();

	// 获取窗口大小
	static Size getSize();

	// 获取窗口句柄
	static HWND getHWnd();

	// 是否允许响应输入法
	static void setTypewritingEnable(
		bool enable
	);

	// 弹出提示窗口
	static void info(
		const String& text,					/* 内容 */
		const String& title = L"Infomation"	/* 窗口标题 */
	);

	// 弹出警告窗口
	static void warning(
		const String& text,					/* 内容 */
		const String& title = L"Warning"	/* 窗口标题 */
	);

	// 弹出错误窗口
	static void error(
		const String& text,					/* 内容 */
		const String& title = L"Error"		/* 窗口标题 */
	);

private:
	// 初始化窗口
	static bool __init(
		const String& title,
		int width,
		int height
	);

	// 重置窗口属性
	static void __uninit();

	// 处理窗口消息
	static void __poll();

	// 更新指针
	static void __updateCursor();

	// Win32 窗口消息回调程序
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// 时间控制
class Time
{
	friend class Game;

public:
	// 获取上一帧与当前帧的时间间隔（秒）
	static float getDeltaTime();

	// 获取上一帧与当前帧的时间间隔（毫秒）
	static unsigned int getDeltaTimeMilliseconds();

	// 获取游戏总时长（秒）
	static float getTotalTime();

	// 获取游戏总时长（毫秒）
	static unsigned int getTotalTimeMilliseconds();

private:
	// 初始化计时操作
	static bool __init();

	// 是否达到更新时间
	static bool __isReady();

	// 更新当前时间
	static void __updateNow();

	// 更新时间信息
	static void __updateLast();

	// 重置时间信息
	static void __reset();

	// 挂起线程
	static void __sleep();
};


class Listener;

// 输入控制
class Input
{
	friend class Game;

public:
	// 检测键盘某按键是否正被按下
	static bool isDown(
		KeyCode::Value key
	);

	// 检测键盘某按键是否被点击
	static bool isPress(
		KeyCode::Value key
	);

	// 检测键盘某按键是否正在松开
	static bool isRelease(
		KeyCode::Value key
	);

	// 检测鼠标按键是否正被按下
	static bool isDown(
		MouseCode::Value code
	);

	// 检测鼠标按键是否被点击
	static bool isPress(
		MouseCode::Value code
	);

	// 检测鼠标按键是否正在松开
	static bool isRelease(
		MouseCode::Value code
	);

	// 获得鼠标X轴坐标值
	static float getMouseX();

	// 获得鼠标Y轴坐标值
	static float getMouseY();

	// 获得鼠标坐标值
	static Point getMousePos();

	// 获得鼠标X轴坐标增量
	static float getMouseDeltaX();

	// 获得鼠标Y轴坐标增量
	static float getMouseDeltaY();

	// 获得鼠标Z轴（鼠标滚轮）坐标增量
	static float getMouseDeltaZ();

private:
	// 初始化 DirectInput 以及键盘鼠标设备
	static bool __init();

	// 刷新输入信息
	static void __update();

	// 卸载 DirectInput
	static void __uninit();
};


// 渲染器
class Renderer
{
	friend class Game;
	friend class Window;

public:
	// 获取背景色
	static Color getBackgroundColor();

	// 修改背景色
	static void setBackgroundColor(
		Color color
	);

	// 显示 FPS
	static void showFps(
		bool show = true
	);

	// 获取系统 DPI 缩放
	static float getDpiScaleX();

	// 获取系统 DPI 缩放
	static float getDpiScaleY();

	// 获取 ID2D1Factory 对象
	static ID2D1Factory * getID2D1Factory();

	// 获取 ID2D1HwndRenderTarget 对象
	static ID2D1HwndRenderTarget * getRenderTarget();

	// 获取 ID2D1SolidColorBrush 对象
	static ID2D1SolidColorBrush * getSolidColorBrush();

	// 获取 IWICImagingFactory 对象
	static IWICImagingFactory * getIWICImagingFactory();

	// 获取 IDWriteFactory 对象
	static IDWriteFactory * getIDWriteFactory();

	// 设置文字渲染样式
	static void SetTextStyle(
		const Color& fillColor,
		bool hasOutline,
		const Color& outlineColor,
		float outlineWidth,
		LineJoin outlineJoin
	);

	// 渲染文字布局
	static void DrawTextLayout(
		IDWriteTextLayout* layout
	);

	// 获取 Miter 样式的 ID2D1StrokeStyle
	static ID2D1StrokeStyle * getMiterID2D1StrokeStyle();

	// 获取 Bevel 样式的 ID2D1StrokeStyle
	static ID2D1StrokeStyle * getBevelID2D1StrokeStyle();

	// 获取 Round 样式的 ID2D1StrokeStyle
	static ID2D1StrokeStyle * getRoundID2D1StrokeStyle();

private:
	// 渲染游戏画面
	static void __render();

	// 创建设备无关资源
	static bool __createDeviceIndependentResources();

	// 创建设备相关资源
	static bool __createDeviceResources();

	// 删除设备相关资源
	static void __discardDeviceResources();

	// 删除所有渲染相关资源
	static void __discardResources();
};


// 日志
class Logger
{
public:
	// 启用日志记录
	static void enable();

	// 关闭日志记录
	static void disable();

	// 输出日志消息
	static void messageln(String format, ...);

	// 输出警告
	static void warningln(String format, ...);

	// 输出错误
	static void errorln(String format, ...);

	// 打开/关闭控制台
	static void showConsole(bool show = true);
};


// 垃圾回收装置
class GC
{
public:
	// 将对象放入 GC 池
	static void trace(
		Object* pObject
	);

	// 清理对象
	static void clear();

	// 检查对象是否在 GC 池中
	static bool isInPool(Object* pObject);

	// GC 池状态
	static bool isClearing();

	// 保留对象
	template <typename Type>
	static inline void retain(Type*& p)
	{
		if (p != nullptr)
		{
			p->retain();
		}
	}

	// 释放对象
	template <typename Type>
	static inline void release(Type*& p)
	{
		if (p != nullptr)
		{
			p->release();
			p = nullptr;
		}
	}
};


//
// GC macros
//

namespace __gc_helper
{
	class GCNewHelper
	{
	public:
		template <typename _Ty>
		inline _Ty* operator<< (_Ty* newObj) const
		{
			if (newObj)
			{
				newObj->autorelease();
			}
			return newObj;
		}

		// 使用 static 变量而不是 static inline 函数
		// 让 IntelliSense 正常工作
		static GCNewHelper instance;
	};
}

#ifndef gcnew
#	define gcnew __gc_helper::GCNewHelper::instance << new (std::nothrow)
#endif


//
// Log macros
//

#ifndef E2D_LOG
#	ifdef E2D_DEBUG
#		define E2D_LOG(FORMAT, ...) easy2d::Logger::messageln(FORMAT, __VA_ARGS__)
#	else
#		define E2D_LOG __noop
#	endif
#endif

#ifndef E2D_WARNING
#	define E2D_WARNING(FORMAT, ...) easy2d::Logger::warningln(FORMAT, __VA_ARGS__)
#endif

#ifndef E2D_ERROR
#	define E2D_ERROR(FORMAT, ...) easy2d::Logger::errorln(FORMAT, __VA_ARGS__)
#endif

#ifndef E2D_ERROR_IF_FAILED
#	define E2D_ERROR_IF_FAILED(HR, FORMAT, ...) do { if (FAILED(HR)) { E2D_ERROR(FORMAT, __VA_ARGS__); } } while (0)
#endif

}