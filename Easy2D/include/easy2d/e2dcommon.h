#pragma once
#include <ostream>
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dmath.h>

namespace easy2d
{


// 方向
enum class Direction : int
{
	Up,			/* 上 */
	Down,		/* 下 */
	Left,		/* 左 */
	Right		/* 右 */
};


// 线条相交样式
enum class LineJoin : int
{
	Miter = 0,	/* 斜切 */
	Bevel = 1,	/* 斜角 */
	Round = 2	/* 圆角 */
};


// 字符串
using String = std::wstring;

// 窄字符串
using ByteString = std::string;


// 函数对象
template <typename _Fty>
using Function = std::function<_Fty>;


template<class Interface>
inline void SafeRelease(Interface*& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}


// 格式化字符串
ByteString FormatString(const char* format, ...);

// 格式化字符串
String FormatString(const wchar_t* format, ...);

// 宽字符串转窄字符串
ByteString WideToNarrow(const String& str);

// 窄字符串转宽字符串
String NarrowToWide(const ByteString& str);


// 颜色
class Color
{
public:
	Color();

	Color(
		float r,
		float g,
		float b
	);

	Color(
		float r,
		float g,
		float b,
		float alpha
	);

	Color(
		UINT rgb
	);

	Color(
		UINT rgb,
		float alpha
	);

	D2D1_COLOR_F toD2DColorF() const;

public:
	enum Value : UINT
	{
		Black = 0x000000,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		Chocolate = 0xD2691E,
		DarkBlue = 0x00008B,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkOrange = 0xFF8C00,
		DarkRed = 0x8B0000,
		DarkViolet = 0x9400D3,
		ForestGreen = 0x228B22,
		Gold = 0xFFD700,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		LightBlue = 0xADD8E6,
		LightCyan = 0xE0FFFF,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightYellow = 0xFFFFE0,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Pink = 0xFFC0CB,
		Purple = 0x800080,
		Red = 0xFF0000,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		Snow = 0xFFFAFA,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Wood = 0xDEB887,
		Yellow = 0xFFFF00,
		Yellow_Green = 0x9ACD32
	};

private:
	void _init(
		UINT rgb,
		float alpha
	);

private:
	float r;
	float g;
	float b;
	float a;
};


// 字体
class Font
{
public:
	String	family;			// 字体族
	float	size;			// 字号
	UINT	weight;			// 粗细值
	bool	italic;			// 斜体

public:
	// 字体粗细值
	enum Weight : UINT
	{
		Thin = 100,
		ExtraLight = 200,
		Light = 300,
		Normal = 400,
		Medium = 500,
		Bold = 700,
		ExtraBold = 800,
		Black = 900,
		ExtraBlack = 950
	};

public:
	Font();

	explicit Font(
		const String& family,
		float size = 22,
		UINT weight = Font::Weight::Normal,
		bool italic = false
	);
};


// 基础对象
class Object
{
public:
	Object();

	virtual ~Object();

	// 自动释放
	void autorelease();

	// 引用计数加一
	void retain();

	// 引用计数减一
	void release();

	// 获取引用计数
	int getRefCount() const;

private:
	int _refCount;
};


// 图片
class Image :
	public Object
{
public:
	Image();

	explicit Image(
		const String& filePath	/* 图片文件路径 */
	);

	explicit Image(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	explicit Image(
		const String& filePath,	/* 图片文件路径 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	explicit Image(
		int resNameId,			/* 图片资源名称 */
		const String& resType,	/* 图片资源类型 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	virtual ~Image();

	// 加载图片文件
	bool open(
		const String& filePath	/* 图片文件路径 */
	);

	// 加载图片资源
	bool open(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 将图片裁剪为矩形
	void crop(
		const Rect& cropRect	/* 裁剪矩形 */
	);

	// 获取宽度
	virtual float getWidth() const;

	// 获取高度
	virtual float getHeight() const;

	// 获取大小
	virtual Size getSize() const;

	// 获取源图片宽度
	virtual float getSourceWidth() const;

	// 获取源图片高度
	virtual float getSourceHeight() const;

	// 获取源图片大小
	virtual Size getSourceSize() const;
	
	// 获取裁剪位置 X 坐标
	virtual float getCropX() const;

	// 获取裁剪位置 Y 坐标
	virtual float getCropY() const;

	// 获取裁剪位置
	virtual Point getCropPos() const;

	// 获取 ID2D1Bitmap 对象
	ID2D1Bitmap * getBitmap();

	// 预加载图片文件
	static bool preload(
		const String& filePath	/* 图片文件路径 */
	);

	// 预加载图片资源
	static bool preload(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 清空缓存
	static void clearCache();

protected:
	// 设置 Bitmap
	void _setBitmap(
		ID2D1Bitmap * bitmap
	);

protected:
	Rect _cropRect;
	ID2D1Bitmap * _bitmap;
};


// 鼠标键值
struct MouseCode
{
	enum Value : int
	{
		Left,		/* 鼠标左键 */
		Right,		/* 鼠标右键 */
		Middle		/* 鼠标中键 */
	};
};


// 键盘键值
struct KeyCode
{
	enum Value : int
	{
		Unknown = 0,
		Up = VK_UP,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		Enter = VK_RETURN,
		Space = VK_SPACE,
		Esc = VK_ESCAPE,
		Ctrl = VK_CONTROL,
		Shift = VK_SHIFT,
		Alt = VK_MENU,
		Tab = VK_TAB,
		Delete = VK_DELETE,
		Back = VK_BACK,

		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Num0 = 0x30,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,

		F1 = VK_F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
	};
};


// 事件
struct Event
{
	enum class Type
	{
		MouseMove,
		MouseDown,
		MouseUp,
		MouseWheel,
		KeyDown,
		KeyUp
	} const type;
	Object* target;

	Event(Type type);

	virtual ~Event();
};

struct MouseMoveEvent
	: public Event
{
	MouseMoveEvent(float x, float y);

	float x, y;
};

struct MouseDownEvent
	: public Event
{
	MouseDownEvent(float x, float y, MouseCode::Value btn);

	float x, y;
	MouseCode::Value button;
};

struct MouseUpEvent
	: public Event
{
	MouseUpEvent(float x, float y, MouseCode::Value btn);

	float x, y;
	MouseCode::Value button;
};

struct MouseWheelEvent
	: public Event
{
	MouseWheelEvent(float x, float y, float delta);

	float x, y;
	float delta;
};

struct KeyDownEvent
	: public Event
{
	KeyDownEvent(KeyCode::Value key, int count);

	KeyCode::Value key;
	int count;
};

struct KeyUpEvent
	: public Event
{
	KeyUpEvent(KeyCode::Value key, int count);

	KeyCode::Value key;
	int count;
};


// 事件监听器
class Listener
	: public Object
{
	friend class Input;

public:
	using Callback = Function<void(Event*)>;

	Listener();

	Listener(
		const Callback& func,
		const String& name,
		bool paused
	);

	// 启动监听
	void start();

	// 停止监听
	void stop();

	// 获取监听器运行状态
	bool isRunning() const;

	// 获取名称
	String getName() const;

	// 设置名称
	void setName(
		const String& name
	);

	// 设置监听回调函数
	void setCallback(
		const Callback& func
	);

	// 处理事件
	virtual void handle(Event* evt);

	void done();

	bool isDone();

protected:
	bool _running;
	bool _done;
	String _name;
	Callback _callback;
};

// 资源
class Resource
{
public:
	// 资源的二进制数据
	struct Data
	{
		void* buffer;	// 资源数据
		int size;	// 资源数据大小

		Data();

		bool isValid() const;

		template <typename Elem>
		friend std::basic_ostream<Elem>& operator<<(std::basic_ostream<Elem>& out, const Resource::Data& data)
		{
			using OStreamType = std::basic_ostream<Elem>;

			typename OStreamType::iostate state = OStreamType::goodbit;
			const typename OStreamType::sentry ok(out);
			if (!ok)
			{
				state |= OStreamType::badbit;
			}
			else
			{
				if (data.buffer && data.size)
				{
					out.write(reinterpret_cast<const Elem*>(data.buffer), static_cast<std::streamsize>(data.size));
				}
				else
				{
					state |= OStreamType::badbit;
				}
			}
			out.setstate(state);
			return out;
		}
	};

	Resource();

	Resource(
		int id,				/* 资源 ID */
		const String& type	/* 资源类型 */
	);

	// 加载资源的二进制数据
	Resource::Data loadData() const;

	// 获取资源 ID
	int getId() const;

	// 获取资源类型
	String getType() const;

private:
	int		_id;
	String	_type;

	mutable Resource::Data _data;
};


}