#include <e2dbase.h>
#include <e2dmanager.h>
#include <e2dtool.h>
#include <e2dcollider.h>


// 控制游戏终止
static bool s_bEndGame = true;
// 控制游戏暂停
static bool s_bPaused = false;
// 是否进行过初始化
static bool s_bInitialized = false;
// 游戏名称
static easy2d::String s_sGameName;


bool easy2d::Game::init(const String& name, const String& mutexName)
{
	if (s_bInitialized)
	{
		E2D_WARNING(L"The game has been initialized!");
		return false;
	}

	if (!mutexName.empty())
	{
		// 创建进程互斥体
		String fullMutexName = L"Easy2DApp-" + mutexName;
		HANDLE hMutex = ::CreateMutex(nullptr, TRUE, fullMutexName.c_str());

		if (hMutex == nullptr)
		{
			E2D_WARNING(L"CreateMutex Failed!");
		}
		else if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// 如果程序已经存在并且正在运行，弹窗提示
			Window::error(L"游戏已在其他窗口中打开！", L"提示");
			// 关闭进程互斥体
			::CloseHandle(hMutex);
			return false;
		}
	}

	// 初始化 COM 组件
	CoInitialize(nullptr);

	// 创建设备无关资源
	if (!Renderer::__createDeviceIndependentResources())
	{
		E2D_ERROR(L"渲染器设备无关资源创建失败");
		return false;
	}

	// 初始化窗口
	if (!Window::__init())
	{
		E2D_ERROR(L"初始化窗口失败");
		return false;
	}

	// 创建设备相关资源
	if (!Renderer::__createDeviceResources())
	{
		E2D_ERROR(L"渲染器设备相关资源创建失败");
		return false;
	}

	// 初始化 DirectInput
	if (!Input::__init())
	{
		E2D_ERROR(L"初始化 DirectInput 失败");
		return false;
	}

	// 初始化播放器
	if (!Music::__init())
	{
		E2D_ERROR(L"初始化 XAudio2 失败");
		return false;
	}

	// 初始化路径
	if (!Path::__init(name))
	{
		E2D_WARNING(L"Path::__init failed!");
	}

	// 保存游戏名称
	s_sGameName = name;

	// 初始化成功
	s_bInitialized = true;

	return s_bInitialized;
}

void easy2d::Game::start()
{
	if (!s_bInitialized)
	{
		E2D_WARNING(L"开始游戏前未进行初始化");
		return;
	}

	// 初始化场景管理器
	SceneManager::__init();
	// 显示窗口
	::ShowWindow(Window::getHWnd(), SW_SHOWNORMAL);
	// 刷新窗口内容
	::UpdateWindow(Window::getHWnd());
	// 处理窗口消息
	Window::__poll();
	// 初始化计时
	Time::__init();

	s_bEndGame = false;

	while (!s_bEndGame)
	{
		// 处理窗口消息
		Window::__poll();
		// 刷新时间
		Time::__updateNow();

		// 判断是否达到了刷新状态
		if (Time::__isReady())
		{
			Input::__update();			// 获取用户输入
			Timer::__update();			// 更新定时器
			ActionManager::__update();	// 更新动作管理器
			SceneManager::__update();	// 更新场景内容
			Renderer::__render();		// 渲染游戏画面

			Time::__updateLast();		// 刷新时间信息
		}
		else
		{
			Time::__sleep();			// 挂起线程
			GC::flush();				// 刷新内存池
		}
	}

	s_bEndGame = true;
}

void easy2d::Game::pause()
{
	s_bPaused = true;
}

void easy2d::Game::resume()
{
	if (s_bInitialized && s_bPaused)
	{
		Game::reset();
	}
	s_bPaused = false;
}

void easy2d::Game::reset()
{
	if (s_bInitialized && !s_bEndGame)
	{
		Time::__reset();
		ActionManager::__resetAll();
		Timer::__resetAll();
	}
}

bool easy2d::Game::isPaused()
{
	return s_bPaused;
}

void easy2d::Game::quit()
{
	s_bEndGame = true;	// 这个变量将控制游戏是否结束
}

void easy2d::Game::destroy()
{
	if (!s_bInitialized)
		return;

	// 删除所有场景
	SceneManager::__uninit();
	// 删除输入监听器
	Input::__clearListeners();
	// 删除碰撞监听器
	Collision::__clearListeners();
	// 删除动作
	ActionManager::__uninit();
	// 回收音乐播放器资源
	Player::__uninit();
	// 删除所有对象
	GC::clear();
	// 清空图片缓存
	Image::clearCache();
	// 回收音乐相关资源
	Music::__uninit();
	// 清空定时器
	Timer::__uninit();
	// 关闭输入
	Input::__uninit();
	// 回收渲染相关资源
	Renderer::__discardResources();
	// 销毁窗口
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

easy2d::String easy2d::Game::getName()
{
	return s_sGameName;
}
