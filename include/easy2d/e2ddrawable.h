#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{
class Game;
class Canvas;


// �����ཻ��ʽ
enum class LineJoin : int
{
	None  = -1,
	Miter = 0,	/* б�� */
	Bevel = 1,	/* б�� */
	Round = 2	/* Բ�� */
};


// ��ֵģʽ
// ָ��λͼ�����ź���תʱ������ɫ�ļ��㷽ʽ
enum class InterpolationMode
{
    Linear,  // ˫���Բ�ֵ������Χ�ĸ����ؽ����������Բ�ֵ���㣬��ͼ��Ŵ�ʱ���ܻ�ģ��
    Nearest  // ���ڽ���ֵ��ȡ���ڽ������ص����ɫֵ
};


// ��ɫ
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

	// ͸��ɫ
	static const Color Transparent;

public:
	float r;
	float g;
	float b;
	float a;
};


// ��ͼ��ʽ
struct Brush
{
	// ��ͼģʽ
	enum class Mode
	{
		Solid,		/* ��� */
		Round,		/* ���� */
		Fill,		/* ���� + ��� */
	};

	// ��ɫ����߻�ˢ
	Brush(
		Color fillColor
	);

	// ��ˢ
	Brush(
		Mode mode = Mode::Fill,
		Color fillColor = Color::White,
		Color strokeColor = Color::Transparent,
		float strokeWidth = 2.0f,
		LineJoin lineJoin = LineJoin::None
	);

	Mode	 mode;			// ��ͼģʽ
	Color	 fillColor;		// ���ɫ
	Color	 strokeColor;	// �����ɫ
	float	 strokeWidth;	// ��߿��
	LineJoin lineJoin;		// �����ཻ��ʽ
};


// �ɻ��ƶ���
class Drawable
{
public:
	virtual void onDraw() = 0;

	// ��ȡ�ɼ�״̬
	bool isVisible() const;

	// ���ÿɼ�
	void setVisible(
		bool value
	);

	// ��ȡ���ƿ��
	float getWidth() const;

	// ��ȡ���Ƹ߶�
	float getHeight() const;

	// ��ȡ���ƴ�С
	Size getSize() const;

	// �޸Ļ��ƿ��
	void setWidth(
		float width
	);

	// �޸Ļ��Ƹ߶�
	void setHeight(
		float height
	);

	// �޸Ļ��ƴ�С
	void setSize(
		const Size& size
	);

	// ��ȡ͸����
	float getOpacity() const;

	// ����͸����
	// Ĭ��Ϊ 1.0f, ��Χ [0, 1]
	void setOpacity(
		float opacity
	);

	// ��ȡê��
	Point getAnchor() const;

	// ����ê��λ��
	// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
	void setAnchor(
		const Point& anchor
	);

	// �޸�Ĭ��ê��λ��
	static void setDefaultAnchor(
		const Point& defaultAnchor
	);

protected:
	bool _visible;
	float _opacity;
	Size _size;
	Point _anchor;
};


// ͼƬ
class Image :
	public Object,
	public Drawable
{
	friend Game;
	friend Canvas;

public:
	// Ԥ����ͼƬ�ļ�
	static void preload(
		const String& filePath	// ͼƬ�ļ�·��
	);

	// Ԥ����ͼƬ��Դ
	static void preload(
		const Resource& res		// ͼƬ��Դ
	);

	Image(
		const String& filePath,			// ͼƬ�ļ�·��
		const Rect& cropRect = Rect()	// �ü�����
	);

	Image(
		const Resource& res,			// ͼƬ��Դ
		const Rect& cropRect = Rect()	// �ü�����
	);

	virtual ~Image();

	void onDraw() override;

	// ��ȡ�ü�����
	Rect getCropRect() const;

	// ���òü�����
	void setCropRect(const Rect& cropRect);

	// ��ȡ���
	float getWidth() const;

	// ��ȡ�߶�
	float getHeight() const;

	// ��ȡ��С
	Size getSize() const;

	// ���ػ���
	static void reloadCache();

protected:
	Image(ID2D1Bitmap* bitmap);

	// ��ȡ ID2D1Bitmap ����
	ID2D1Bitmap* getBitmap();

	void resetBitmap(ID2D1Bitmap* bitmap);

	// ��ջ���
	static void clearCache();

protected:
	ID2D1Bitmap* _bitmap;
	Rect _cropRect;
};

}
