#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{
class Game;
class Canvas;


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
struct Brush
{
	// 绘图模式
	enum class Mode
	{
		Solid,		/* 填充 */
		Round,		/* 轮廓 */
		Fill,		/* 轮廓 + 填充 */
	};

	// 纯色无描边画刷
	Brush(
		Color fillColor
	);

	// 画刷
	Brush(
		Mode mode = Mode::Fill,
		Color fillColor = Color::White,
		Color strokeColor = Color::Transparent,
		float strokeWidth = 2.0f,
		LineJoin lineJoin = LineJoin::None
	);

	Mode	 mode;			// 绘图模式
	Color	 fillColor;		// 填充色
	Color	 strokeColor;	// 描边颜色
	float	 strokeWidth;	// 描边宽度
	LineJoin lineJoin;		// 线条相交样式
};


// 可绘制对象
class Drawable
{
public:
	virtual void onDraw() = 0;

	// 获取可见状态
	bool isVisible() const;

	// 设置可见
	void setVisible(
		bool value
	);

	// 获取绘制宽度
	float getWidth() const;

	// 获取绘制高度
	float getHeight() const;

	// 获取绘制大小
	Size getSize() const;

	// 修改绘制宽度
	void setWidth(
		float width
	);

	// 修改绘制高度
	void setHeight(
		float height
	);

	// 修改绘制大小
	void setSize(
		const Size& size
	);

	// 获取透明度
	float getOpacity() const;

	// 设置透明度
	// 默认为 1.0f, 范围 [0, 1]
	void setOpacity(
		float opacity
	);

	// 获取锚点
	Point getAnchor() const;

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	void setAnchor(
		const Point& anchor
	);

	// 修改默认锚点位置
	static void setDefaultAnchor(
		const Point& defaultAnchor
	);

protected:
	bool _visible;
	float _opacity;
	Size _size;
	Point _anchor;
};


// 图片
class Image :
	public Object,
	public Drawable
{
	friend Game;
	friend Canvas;

public:
	// 预加载图片文件
	static void preload(
		const String& filePath	// 图片文件路径
	);

	// 预加载图片资源
	static void preload(
		const Resource& res		// 图片资源
	);

	Image(
		const String& filePath,			// 图片文件路径
		const Rect& cropRect = Rect()	// 裁剪矩形
	);

	Image(
		const Resource& res,			// 图片资源
		const Rect& cropRect = Rect()	// 裁剪矩形
	);

	virtual ~Image();

	void onDraw() override;

	// 获取裁剪矩形
	Rect getCropRect() const;

	// 设置裁剪矩形
	void setCropRect(const Rect& cropRect);

	// 获取宽度
	float getWidth() const;

	// 获取高度
	float getHeight() const;

	// 获取大小
	Size getSize() const;

	// 重载缓存
	static void reloadCache();

protected:
	Image(ID2D1Bitmap* bitmap);

	// 获取 ID2D1Bitmap 对象
	ID2D1Bitmap* getBitmap();

	void resetBitmap(ID2D1Bitmap* bitmap);

	// 清空缓存
	static void clearCache();

protected:
	ID2D1Bitmap* _bitmap;
	Rect _cropRect;
};

}
