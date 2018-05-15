#pragma once
#include "e2dmacros.h"
#include "e2dcustom.h"

namespace e2d
{


class Size;

// ��ʾ����Ľṹ��
class Point
{
public:
	double x;	// X ����
	double y;	// Y ����

public:
	Point();

	Point(double x, double y);

	Point operator + (Point const & p) const;
	Point operator - (Point const & p) const;
	Point operator * (double const & value) const;
	Point operator / (double const & value) const;
	Point operator - () const;

	operator e2d::Size() const;
};

// ��ά����
typedef Point Vector;

// ��ʾ��С�Ľṹ��
class Size
{
public:
	double width;	// ���
	double height;	// �߶�

public:
	Size();

	Size(double width, double height);

	Size operator + (Size const & size) const;
	Size operator - (Size const & size) const;
	Size operator * (double const & value) const;
	Size operator / (double const & value) const;
	Size operator - () const;

	operator e2d::Point() const;
};


// �ַ���
class String
{
public:
	String();
	String(const String &);
	String(const char *);
	String(const wchar_t *);
	String(String &&);

	~String();

	// �ж��ַ����Ƿ�Ϊ��
	bool isEmpty() const;

	// ��ȡ�ַ�������
	int getLength() const;

	// ��ȡ���ַ�����ɢ��ֵ
	unsigned int getHashCode() const;

	// ��ȡ Unicode �ַ���
	std::wstring getWString() const;

	// ��ȡ ANSI �ַ���
	std::string getCString() const;

	// �Ƚ��ַ���
	int compare(
		const String & str
	) const;

	// ��ȡ�ַ���
	e2d::String subtract(
		int offset,		/* ƫ���� */
		int count = -1	/* ��ȡ�ַ����� */
	) const;

	// �����ַ���
	void insert(
		const String & str,
		int pos
	);

	// �滻�ַ����е�ָ������
	void replace(
		const String & from,	/* ���滻���� */
		const String & to		/* �滻������ */
	);

	// ɾ���ַ����е�ָ������
	void erase(
		int offset,		/* ƫ���� */
		int count		/* ɾ���ַ����� */
	);

	// �����ַ���
	int find(
		const String & str,	/* �������� */
		int offset = 0		/* ƫ���� */
	) const;

	// ����ַ���
	void clear();

	// ��ȡ��д�ַ���
	String toUpper() const;

	// ��ȡСд�ַ���
	String toLower() const;

	// ���ַ���ת��Ϊ int ��
	int toInt() const;

	// ���ַ���ת��Ϊ double ��
	double toDouble() const;

	// ���ַ���ת��Ϊ bool ��
	bool toBool() const;

	// ��������ת�ַ���
	static String parse(int value);
	static String parse(unsigned int value);
	static String parse(float value);
	static String parse(double value);

	// ��ʽ���ַ���
	static String format(const char * format, ...);
	static String format(const wchar_t * format, ...);

	// �������ַ���
	static void swap(String &str1, String &str2);

	// ��ֵ�����
	String& operator= (const String &);
	String& operator= (const char *);
	String& operator= (const wchar_t *);

	// �����
	String& operator+= (const String &);
	String& operator+= (const char *);
	String& operator+= (const wchar_t *);
	String operator+ (const String &);
	String operator+ (const char *);
	String operator+ (const wchar_t *);

	// ��Ԫ�����
	friend String operator+ (const char *, const String &);
	friend String operator+ (const wchar_t*, const String &);

	// ����ת��������
	operator const wchar_t* () const;
	operator wchar_t* () const;

	// �Ƚ������
	bool operator== (const String &);
	bool operator== (const char *);
	bool operator== (const wchar_t *);
	bool operator!= (const String &);
	bool operator!= (const char *);
	bool operator!= (const wchar_t *);
	bool operator> (const String &) const;
	bool operator>= (const String &) const;
	bool operator< (const String &) const;
	bool operator<= (const String &) const;

	// << �����������ַ�����
	String& operator<< (const String &);
	String& operator<< (const char *);
	String& operator<< (char *);
	String& operator<< (const wchar_t *);
	String& operator<< (wchar_t *);
	String& operator<< (int value);
	String& operator<< (unsigned int value);
	String& operator<< (float value);
	String& operator<< (double value);

	// ���������
	wchar_t& operator[] (int);

	friend std::ostream& operator<< (std::ostream &, const String &);
	friend std::wostream& operator<< (std::wostream &, const String &);

	friend std::istream& operator>> (std::istream &, String &);
	friend std::wistream& operator>> (std::wistream &, String &);

private:
	std::wstring _str;
};


// ��ɫ
class Color
{
public:
	Color();

	Color(
		double r,
		double g,
		double b
	);

	Color(
		double r,
		double g,
		double b,
		double alpha
	);

	Color(
		UINT32 rgb
	);

	Color(
		UINT32 rgb,
		double alpha
	);

	D2D1_COLOR_F toColorF() const;

public:
	enum RGB_VALUE : UINT32
	{
		ALICE_BLUE = 0xF0F8FF,
		AQUA = 0x00FFFF,
		AZURE = 0xF0FFFF,
		BEIGE = 0xF5F5DC,
		BLACK = 0x000000,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		CHOCOLATE = 0xD2691E,
		CYAN = 0x00FFFF,
		DARK_BLUE = 0x00008B,
		DARK_CYAN = 0x008B8B,
		DARK_GOLDENROD = 0xB8860B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_ORANGE = 0xFF8C00,
		DARK_RED = 0x8B0000,
		DARK_SEA_GREEN = 0x8FBC8F,
		DARK_VIOLET = 0x9400D3,
		DEEP_PINK = 0xFF1493,
		DEEP_SKY_BLUE = 0x00BFFF,
		FOREST_GREEN = 0x228B22,
		GOLD = 0xFFD700,
		GOLDENROD = 0xDAA520,
		GRAY = 0x808080,
		GREEN = 0x008000,
		GREEN_YELLOW = 0xADFF2F,
		LIGHT_BLUE = 0xADD8E6,
		LIGHT_CYAN = 0xE0FFFF,
		LIGHT_GOLDENROD_YELLOW = 0xFAFAD2,
		LIGHT_GREEN = 0x90EE90,
		LIGHT_GRAY = 0xD3D3D3,
		LIGHT_PINK = 0xFFB6C1,
		LIGHT_SEA_GREEN = 0x20B2AA,
		LIGHT_SKY_BLUE = 0x87CEFA,
		LIGHT_SLATE_GRAY = 0x778899,
		LIGHT_YELLOW = 0xFFFFE0,
		MEDIUM_BLUE = 0x0000CD,
		MEDIUM_PURPLE = 0x9370DB,
		MEDIUM_SEA_GREEN = 0x3CB371,
		MEDIUM_SPRING_GREEN = 0x00FA9A,
		MEDUIM_VIOLET_RED = 0xC71585,
		MIDNIGHT_BLUE = 0x191970,
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		PINK = 0xFFC0CB,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		SEA_GREEN = 0x2E8B57,
		SEA_SHELL = 0xFFF5EE,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SNOW = 0xFFFAFA,
		SPRING_GREEN = 0x00FF7F,
		TOMATO = 0xFF6347,
		VIOLET = 0xEE82EE,
		WHEAT = 0xF5DEB3,
		WHITE = 0xFFFFFF,
		WHITE_SMOKE = 0xF5F5F5,
		WOOD = 0xDEB887,
		YELLOW = 0xFFFF00,
		YELLOW_GREEN = 0x9ACD32
	};

protected:
	void _init(
		UINT32 rgb,
		double alpha
	);

public:
	float r;
	float g;
	float b;
	float a;
};


// �����ϸֵ
class FontWeight
{
public:
	enum : UINT32
	{
		THIN = 100,
		EXTRA_LIGHT = 200,
		ULTRA_LIGHT = 200,
		LIGHT = 300,
		SEMI_LIGHT = 350,
		NORMAL = 400,
		REGULAR = 400,
		MEDIUM = 500,
		DEMI_BOLD = 600,
		SEMI_BOLD = 600,
		BOLD = 700,
		EXTRA_BOLD = 800,
		ULTRA_BOLD = 800,
		BLACK = 900,
		HEAVY = 900,
		EXTRA_BLACK = 950,
		ULTRA_BLACK = 950
	};
};


// �ı����뷽ʽ
enum class TextAlign : int
{
	LEFT,		/* ����� */
	RIGHT,		/* �Ҷ��� */
	CENTER		/* ���ж��� */
};


// ��ֵ����
enum class KeyCode : int
{
	UP = 0xC8,
	LEFT = 0xCB,
	RIGHT = 0xCD,
	DOWN = 0xD0,
	ENTER = 0x1C,
	SPACE = 0x39,
	ESC = 0x01,
	BACK = 0x0E,
	TAB = 0x0F,
	PAUSE = 0xC5,
	Q = 0x10,
	W = 0x11,
	E = 0x12,
	R = 0x13,
	T = 0x14,
	Y = 0x15,
	U = 0x16,
	I = 0x17,
	O = 0x18,
	P = 0x19,
	A = 0x1E,
	S = 0x1F,
	D = 0x20,
	F = 0x21,
	G = 0x22,
	H = 0x23,
	J = 0x24,
	K = 0x25,
	L = 0x26,
	Z = 0x2C,
	X = 0x2D,
	C = 0x2E,
	V = 0x2F,
	B = 0x30,
	N = 0x31,
	M = 0x32,
	NUM1 = 0x02,
	NUM2 = 0x03,
	NUM3 = 0x04,
	NUM4 = 0x05,
	NUM5 = 0x06,
	NUM6 = 0x07,
	NUM7 = 0x08,
	NUM8 = 0x09,
	NUM9 = 0x0A,
	NUM0 = 0x0B,
	NUMPAD7 = 0x47,
	NUMPAD8 = 0x48,
	NUMPAD9 = 0x49,
	NUMPAD4 = 0x4B,
	NUMPAD5 = 0x4C,
	NUMPAD6 = 0x4D,
	NUMPAD1 = 0x4F,
	NUMPAD2 = 0x50,
	NUMPAD3 = 0x51,
	NUMPAD0 = 0x52
};


// ���ָ����ʽ
enum class Cursor : int
{
	NORMAL,		/* Ĭ��ָ����ʽ */
	HAND,		/* ��״ָ�� */
	NO,			/* ��ָֹ�� */
	WAIT,		/* ɳ©ָ�� */
	ARROW_WAIT	/* Ĭ��ָ���Сɳ© */
};


// ����
enum class Direction : int
{
	UP,			/* �� */
	DOWN,		/* �� */
	LEFT,		/* �� */
	RIGHT		/* �� */
};


// ���彻����ϵ
enum class Relation : int
{
	UNKNOWN = 0,		/* ��ϵ��ȷ�� */
	DISJOINT = 1,		/* û�н��� */
	IS_CONTAINED = 2,	/* ��ȫ������ */
	CONTAINS = 3,		/* ��ȫ���� */
	OVERLAP = 4			/* �����ص� */
};


// �����ཻ��ʽ
enum class LineJoin : int
{
	MITER = 0,
	BEVEL = 1,
	ROUND = 2
};


// ��״��ʽ
enum class ShapeStyle : int
{
	SOLID,		/* ��� */
	ROUND,		/* ���� */
	FILL,		/* ���� + ��� */
};


// ��ײ�����
enum class ColliderType : int
{
	RECT,		/* ���� */
	CIRCLE,		/* Բ�� */
	ELLIPSE		/* ��Բ�� */
};


// �ı���ʽ
class TextStyle
{
public:
	String		fontFamily;			// ����
	double		fontSize;			// �ֺ�
	Color		color;				// ��ɫ
	UINT32		fontWeight;			// ��ϸֵ
	bool		italic;				// б��
	TextAlign	alignment;			// ���뷽ʽ
	bool		wrapping;			// ���Զ�����
	double		wrappingWidth;		// �Զ����п��
	double		lineSpacing;		// �м��
	bool		hasUnderline;		// �»���
	bool		hasStrikethrough;	// ɾ����
	bool		hasOutline;			// ��ʾ���
	Color		outlineColor;		// �����ɫ
	double		outlineWidth;		// ����߿�
	LineJoin	outlineJoin;		// ������ཻ��ʽ

public:
	TextStyle();

	TextStyle(
		const String& fontFamily,
		double fontSize = 22,
		Color color = Color::WHITE,
		UINT32 fontWeight = FontWeight::NORMAL,
		bool italic = false,
		TextAlign alignment = TextAlign::LEFT,
		bool wrapping = false,
		double wrappingWidth = 0.0,
		double lineSpacing = 0.0,
		bool hasUnderline = false,
		bool hasStrikethrough = false,
		bool hasOutline = true,
		Color outlineColor = Color(Color::BLACK, 0.5),
		double outlineWidth = 1.0,
		LineJoin outlineJoin = LineJoin::ROUND
	);
};


// �ڵ�����
struct NodeProperty
{
	bool visable;		// �ɼ���
	double posX;		// X ����
	double posY;		// Y ����
	double width;		// ���
	double height;		// �߶�
	double opacity;		// ͸����
	double pivotX;		// ���ĵ� X ����
	double pivotY;		// ���ĵ� Y ����
	double scaleX;		// ��������
	double scaleY;		// ��������
	double rotation;	// ��ת�Ƕ�
	double skewAngleX;	// ������б�Ƕ�
	double skewAngleY;	// ������б�Ƕ�
};


// ��������
class Function
{
public:
	Function();

	Function(
		std::nullptr_t
	);

	Function(
		std::function<void()> func
	);

	template<typename Func>
	Function(Func func) : _func(func) {}

	template<typename Func, typename Object>
	Function(
		Func&& func,	/* ����ĳ�Ա���� */
		Object&& obj	/* ����ָ�� */
	)
	{
		_func = std::bind(func, obj);
	}

	void operator() (void) const;

	operator bool() const;

protected:
	std::function<void()> _func;
};


// ��������
class Object
{
public:
	Object();

	virtual ~Object();

	// �Զ��ͷ�
	void autorelease();

	// ���ü�����һ
	void retain();

	// ���ü�����һ
	void release();

	// ��ȡ���ü���
	int getRefCount() const;

	// ���ٶ���
	virtual void onDestroy() {}

private:
	int _refCount;
};


// ͼƬ
class Image :
	public Object
{
public:
	// ����һ���յ�ͼƬ����
	Image();

	// ����ͼƬ�ļ�
	Image(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	// ����ͼƬ��Դ
	Image(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	// ����λͼ
	Image(
		ID2D1Bitmap * bitmap	/* λͼ��Դ */
	);

	// ����ͼƬ�ļ����ü�
	Image(
		const String& filePath,	/* ͼƬ�ļ�·�� */
		double cropX,			/* �ü�λ�� X ���� */
		double cropY,			/* �ü�λ�� Y ���� */
		double cropWidth,		/* �ü���� */
		double cropHeight		/* �ü��߶� */
	);

	// ����ͼƬ��Դ���ü�
	Image(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType,	/* ͼƬ��Դ���� */
		double cropX,			/* �ü�λ�� X ���� */
		double cropY,			/* �ü�λ�� Y ���� */
		double cropWidth,		/* �ü���� */
		double cropHeight		/* �ü��߶� */
	);

	virtual ~Image();

	// ����ͼƬ�ļ�
	bool open(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	// ����ͼƬ��Դ
	bool open(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	// ����λͼ
	bool open(
		ID2D1Bitmap * bitmap	/* λͼ��Դ */
	);

	// ��ͼƬ�ü�Ϊ����
	void crop(
		double cropX,			/* �ü�λ�� X ���� */
		double cropY,			/* �ü�λ�� Y ���� */
		double cropWidth,		/* �ü���� */
		double cropHeight		/* �ü��߶� */
	);

	// ��ȡ���
	virtual double getWidth() const;

	// ��ȡ�߶�
	virtual double getHeight() const;

	// ��ȡ��С
	virtual Size getSize() const;

	// ��ȡԴͼƬ���
	virtual double getSourceWidth() const;

	// ��ȡԴͼƬ�߶�
	virtual double getSourceHeight() const;

	// ��ȡԴͼƬ��С
	virtual Size getSourceSize() const;
	
	// ��ȡ�ü�λ�� X ����
	virtual double getCropX() const;

	// ��ȡ�ü�λ�� Y ����
	virtual double getCropY() const;

	// ��ȡ�ü�λ��
	virtual Point getCropPos() const;

	// ��ȡ ID2D1Bitmap ����
	ID2D1Bitmap * getBitmap();

	// Ԥ����ͼƬ�ļ�
	static bool preload(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	// Ԥ����ͼƬ��Դ
	static bool preload(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	// ��ջ���
	static void clearCache();

protected:
	// ���� Bitmap
	void _setBitmap(
		ID2D1Bitmap * bitmap
	);

protected:
	double	_cropX;
	double	_cropY;
	double	_cropWidth;
	double	_cropHeight;
	ID2D1Bitmap * _bitmap;
};


// ֡����
class Animation :
	public Object
{
public:
	// ����֡����
	Animation();

	// ����֡����
	Animation(
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	// �����ض�֡�����֡����
	Animation(
		double interval						/* ֡������룩 */
	);

	// �����ض�֡�����֡����
	Animation(
		double interval,					/* ֡������룩 */
		const std::vector<Image*>& frames	/* �ؼ�֡���� */
	);

	virtual ~Animation();

	// ��ӹؼ�֡
	void add(
		Image * frame	/* �ؼ�֡ */
	);

	// ��Ӷ���ؼ�֡
	void add(
		const std::vector<Image*>& frames	/* �ؼ�֡�б� */
	);

	// ��ȡ֡���
	double getInterval() const;

	// ��ȡ�ؼ�֡
	const std::vector<Image*>& getFrames() const;

	// ����ÿһ֡��ʱ����
	void setInterval(
		double interval		/* ֡������룩 */
	);

	// ��ȡ�����Ŀ�������
	virtual Animation * clone() const;

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	double	_interval;
	std::vector<Image*> _frames;
};


class Node;
class SceneManager;
class Transition;

// ����
class Scene :
	public Object
{
	friend SceneManager;
	friend Transition;

public:
	Scene();

	virtual ~Scene();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter() {}

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit() {}

	// ��д�����������������ײ����ʱ�Զ�ִ��
	virtual void onCollide(
		Node* pActiveNode,	/* ��ײ����ʱ�������� */
		Node* pPassiveNode	/* ��ײ����ʱ�ı����� */
	) {}

	// ��д��������������ڹرմ���ʱִ�У����� false ����ֹ���ڹرգ�
	virtual bool onCloseWindow() { return true; }

	// ��д���������������ÿһ֡����ˢ��ʱִ��
	virtual void onUpdate() {}

	// ��������� onUpdate ����
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// ��ӽڵ㵽����
	void add(
		Node * child,	/* Ҫ��ӵĽڵ� */
		int zOrder = 0	/* ��Ⱦ˳�� */
	);

	// ��Ӷ���ڵ㵽����
	virtual void add(
		const std::vector<Node*>& nodes,	/* �ڵ����� */
		int order = 0						/* ��Ⱦ˳�� */
	);

	// ɾ���ӽڵ�
	bool remove(
		Node * child
	);

	// ��ȡ����������ͬ���ӽڵ�
	std::vector<Node*> get(
		const String& name
	) const;

	// ��ȡ������ͬ���ӽڵ�
	Node* getOne(
		const String& name
	) const;

	// ��ȡ�����ӽڵ�
	const std::vector<Node*>& getAll() const;

	// ��ȡ���ڵ�
	Node * getRoot() const;

	// ������رսڵ�������Ⱦ
	void showCollider(
		bool visiable = true
	);

	// ���ٶ���
	virtual void onDestroy() override;

protected:
	// ��Ⱦ��������
	void _render();

	// ���³�������
	void _update();

protected:
	bool _autoUpdate;
	bool _colliderVisiable;
	Node * _root;
};


template <class Type>
inline void SafeRelease(Type*& p)
{ 
	if (p != nullptr)
	{ 
		p->release(); 
		p = nullptr; 
	}
}


}