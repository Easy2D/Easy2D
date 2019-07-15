#pragma once
#include <e2dmacros.h>
#include <e2dmath.h>

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


// 函数对象
template <typename _Fty>
using Function = std::function<_Fty>;


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


class Node;
class SceneManager;
class Transition;

// 场景
class Scene :
	public Object
{
	friend class SceneManager;
	friend class Transition;

public:
	Scene();

	virtual ~Scene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

	// 重写这个函数，它将在关闭窗口时执行（返回 false 将阻止窗口关闭）
	virtual bool onCloseWindow() { return true; }

	// 重写这个函数，它将在每一帧画面刷新时执行
	virtual void onUpdate() {}

	// 开启或禁用 onUpdate 函数
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// 添加节点到场景
	void add(
		Node * child,	/* 要添加的节点 */
		int zOrder = 0	/* 渲染顺序 */
	);

	// 添加多个节点到场景
	virtual void add(
		const std::vector<Node*>& nodes,	/* 节点数组 */
		int order = 0						/* 渲染顺序 */
	);

	// 删除子节点
	bool remove(
		Node * child
	);

	// 获取所有名称相同的子节点
	std::vector<Node*> get(
		const String& name
	) const;

	// 获取名称相同的子节点
	Node* getOne(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& getAll() const;

	// 获取根节点
	Node * getRoot() const;

	// 开启或关闭节点轮廓渲染
	void showCollider(
		bool visiable = true
	);

protected:
	// 渲染场景画面
	void _render();

	// 更新场景内容
	void _update();

protected:
	bool _autoUpdate;
	bool _colliderVisiable;
	Node * _root;
};


template<class Interface>
inline void SafeRelease(Interface*& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}


}