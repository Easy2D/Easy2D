#pragma once
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dmath.h>
#include <functional> // std::function, std::hash
#include <ostream> // std::basic_ostream
#include <string>

namespace easy2d
{
class Game;
class Canvas;

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
	None  = -1,
	Miter = 0,	/* 斜切 */
	Bevel = 1,	/* 斜角 */
	Round = 2	/* 圆角 */
};


// 插值模式
// 指定位图在缩放和旋转时像素颜色的计算方式
enum class InterpolationMode
{
    Linear,  // 双线性插值，对周围四个像素进行两次线性插值计算，在图像放大时可能会模糊
    Nearest  // 最邻近插值，取最邻近的像素点的颜色值
};


// 窄字符串
using ByteString = std::string;

// 宽字符串
using WideString = std::wstring;

// 字符串
using String = ByteString;


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
WideString FormatString(const wchar_t* format, ...);

// 宽字符串转窄字符串
ByteString WideToNarrow(const WideString& str);

// 窄字符串转宽字符串
WideString NarrowToWide(const ByteString& str);


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
		YellowGreen = 0x9ACD32
	};

	// 透明色
	static const Color Transparent;

public:
	float r;
	float g;
	float b;
	float a;
};


// 绘图样式
struct DrawingStyle
{
	// 绘图模式
	enum class Mode
	{
		Solid,		/* 填充 */
		Round,		/* 轮廓 */
		Fill,		/* 轮廓 + 填充 */
	};

	// 绘图样式
	DrawingStyle(
		Mode mode = Mode::Fill,
		Color fillColor = Color::White,
		Color strokeColor = Color::Transparent,
		float strokeWidth = 2.0f,
		LineJoin lineJoin = LineJoin::None
	);

	// 纯色无描边样式
	DrawingStyle(
		Color fillColor
	);

	Mode	 mode;			// 绘图模式
	Color	 fillColor;		// 填充色
	Color	 strokeColor;	// 描边颜色
	float	 strokeWidth;	// 描边宽度
	LineJoin lineJoin;		// 线条相交样式
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

	Resource(
		int id,				/* 资源 ID */
		const String& type	/* 资源类型 */
	);

	// 加载资源的二进制数据
	Data loadData() const;

	// 获取资源 ID
	int getId() const;

	// 获取资源类型
	String getType() const;

	bool operator==(const Resource& other) const { return _id == other._id && _type == other._type; }

	bool operator<(const Resource& other) const { return _id < other._id || _type < other._type; }

private:
	int		_id;
	String	_type;
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

	// 获取名称
	String getName() const;

	// 设置名称
	void setName(
		const String& name
	);

	// 名称是否相同
	bool isName(
		const String& name,
		size_t hashName = 0
	) const;

private:
	int _refCount;
	String* _name;
	size_t _hashName;
};


// 图片
class Image :
	public Object
{
	friend Game;
	friend Canvas;

public:
	// 加载图片文件
	static Image* load(
		const String& filePath	/* 图片文件路径 */
	);

	// 加载图片资源
	static Image* load(
		const Resource& res		/* 图片资源 */
	);

	// 加载图片资源
	static Image* load(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	virtual ~Image();

	// 获取宽度
	float getWidth() const;

	// 获取高度
	float getHeight() const;

	// 获取大小
	Size getSize() const;

	// 获取 ID2D1Bitmap 对象
	ID2D1Bitmap* getBitmap();

	// 重载缓存
	static void reloadCache();

protected:
	Image(ID2D1Bitmap* bitmap);

	void resetBitmap(ID2D1Bitmap* bitmap);

	// 清空缓存
	static void clearCache();

protected:
	ID2D1Bitmap* _bitmap;
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
		Up = VK_UP,				// 方向键 - 上
		Left = VK_LEFT,			// 方向键 - 左
		Right = VK_RIGHT,		// 方向键 - 右
		Down = VK_DOWN,			// 方向键 - 下
		Enter = VK_RETURN,		// 回车键
		Space = VK_SPACE,		// 空格键
		Esc = VK_ESCAPE,		// Esc 键
		Ctrl = VK_CONTROL,		// 任意 ctrl 键
		LCtrl = VK_LCONTROL,	// 左 ctrl 键
		RCtrl = VK_RCONTROL,	// 右 ctrl 键
		Shift = VK_SHIFT,		// 任意 shift 键
		LShift = VK_LSHIFT,		// 左 shift 键
		RShift = VK_RSHIFT,		// 右 shift 键
		Alt = VK_MENU,			// 任意 alt 键
		LAlt = VK_LMENU,		// 左 alt 键
		RAlt = VK_RMENU,		// 右 alt 键
		Tab = VK_TAB,			// Tab 键
		Delete = VK_DELETE,		// 删除键
		Back = VK_BACK,			// 退格键

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

		Num0 = 0x30,			// 数字键
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,	// 小键盘数字键
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
	enum Type : UINT
	{
		Unknown = 0,

		MouseMove,		// 鼠标移动
		MouseDown,		// 鼠标按下
		MouseUp,		// 鼠标抬起
		MouseWheel,		// 鼠标滚轮滑动
		KeyDown,		// 按键按下
		KeyUp,			// 按键抬起

		Last = KeyUp,
	};

	const UINT type;
	Object* target;

	Event(UINT type);

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

}

namespace std
{
	template<>
	struct hash<easy2d::Resource>
	{
		size_t operator()(const easy2d::Resource& res) const
		{
			return static_cast<size_t>(res.getId());
		}
	};
}
