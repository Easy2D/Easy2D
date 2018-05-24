#pragma once
#include "e2dmacros.h"

namespace e2d
{


// ����
enum class Direction : int
{
	Up,			/* �� */
	Down,		/* �� */
	Left,		/* �� */
	Right		/* �� */
};


// �����ཻ��ʽ
enum class LineJoin : int
{
	Miter = 0,	/* б�� */
	Bevel = 1,	/* б�� */
	Round = 2	/* Բ�� */
};


class Size;

// ����
class Point
{
public:
	double x;	// X ����
	double y;	// Y ����

public:
	Point();

	Point(double x, double y);

	Point(const Point& other);

	Point operator + (Point const & point) const;
	Point operator - (Point const & point) const;
	Point operator * (double const & point) const;
	Point operator / (double const & point) const;
	Point operator - () const;
	bool operator== (const Point& point) const;

	operator e2d::Size() const;
};


// ��ά����
typedef Point Vector;


// ��С
class Size
{
public:
	double width;	// ���
	double height;	// �߶�

public:
	Size();

	Size(double width, double height);

	Size(const Size& other);

	Size operator + (Size const & size) const;
	Size operator - (Size const & size) const;
	Size operator * (double const & size) const;
	Size operator / (double const & size) const;
	Size operator - () const;
	bool operator== (const Size& size) const;

	operator e2d::Point() const;
};


// ����
class Rect
{
public:
	Point origin;	// ԭ������
	Size  size;		// ��Ⱥ͸߶�

public:
	Rect();
	
	Rect(double x, double y, double width, double height);
	
	Rect(const Point& pos, const Size& size);
	
	Rect(const Rect& other);
	
	Rect& operator= (const Rect& other);

	bool operator== (const Rect& rect) const;
	
	// ���þ���
	void setRect(
		double x, 
		double y, 
		double width, 
		double height
	);
	
	// �жϵ��Ƿ��ھ�����
	bool containsPoint(
		const Point& point
	) const;
	
	// �ж��������Ƿ��ཻ
	bool intersects(
		const Rect& rect
	) const;
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

	// ��ȡָ��λ���ַ�
	wchar_t at(
		int index
	) const;

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
	friend class String operator+ (const char *, const String &);
	friend class String operator+ (const wchar_t*, const String &);

	// ����ת��������
	explicit operator const wchar_t* () const;
	explicit operator wchar_t* () const;

	// �Ƚ������
	bool operator== (const String &) const;
	bool operator== (const char *) const;
	bool operator== (const wchar_t *) const;
	bool operator!= (const String &) const;
	bool operator!= (const char *) const;
	bool operator!= (const wchar_t *) const;
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
		UINT rgb
	);

	Color(
		UINT rgb,
		double alpha
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
		double alpha
	);

private:
	float r;
	float g;
	float b;
	float a;
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


// ����
class Font
{
public:
	String	family;			// ������
	double	size;			// �ֺ�
	UINT	weight;			// ��ϸֵ
	bool	italic;			// б��

public:
	// �����ϸֵ
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
		double size = 22,
		UINT weight = Font::Weight::Normal,
		bool italic = false
	);
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
	Image();

	explicit Image(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	explicit Image(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	explicit Image(
		const String& filePath,	/* ͼƬ�ļ�·�� */
		const Rect& cropRect	/* �ü����� */
	);

	explicit Image(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType,	/* ͼƬ��Դ���� */
		const Rect& cropRect	/* �ü����� */
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

	// ��ͼƬ�ü�Ϊ����
	void crop(
		const Rect& cropRect	/* �ü����� */
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
	Rect _cropRect;
	ID2D1Bitmap * _bitmap;
};


class Node;
class SceneManager;
class Transition;

// ����
class Scene :
	public Object
{
	friend class SceneManager;
	friend class Transition;

public:
	Scene();

	virtual ~Scene();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter() {}

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit() {}

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


#if _MSC_VER > 1700

// �������Զ������ڴ�Ķ���
template <typename Type, typename... Args>
inline Type * Create(Args&&... args)
{
	auto newObj = new (std::nothrow) Type(std::forward<Args>(args)...);
	if (newObj)
	{
		newObj->autorelease();
		return newObj;
	}
	return nullptr;
}

#else

template <typename Type>
inline Type * Create()
{
	auto newObj = new (std::nothrow) Type();
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

template <typename Type, typename Arg1>
inline Type * Create(Arg1&& arg1)
{
	auto newObj = new (std::nothrow) Type(std::forward<Arg1>(arg1));
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

template <typename Type,
	typename Arg1,
	typename Arg2>
	inline Type * Create(
		Arg1&& arg1, 
		Arg2&& arg2
	)
{
	auto newObj = new (std::nothrow) Type(
		std::forward<Arg1>(arg1),
		std::forward<Arg2>(arg2)
	);
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

template <typename Type,
	typename Arg1,
	typename Arg2,
	typename Arg3>
	inline Type * Create(
		Arg1&& arg1,
		Arg2&& arg2,
		Arg3&& arg3
	)
{
	auto newObj = new (std::nothrow) Type(
		std::forward<Arg1>(arg1),
		std::forward<Arg2>(arg2),
		std::forward<Arg3>(arg3)
	);
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

template <typename Type,
	typename Arg1,
	typename Arg2,
	typename Arg3,
	typename Arg4>
	inline Type * Create(
		Arg1&& arg1,
		Arg2&& arg2,
		Arg3&& arg3,
		Arg4&& arg4
	)
{
	auto newObj = new (std::nothrow) Type(
		std::forward<Arg1>(arg1),
		std::forward<Arg2>(arg2),
		std::forward<Arg3>(arg3),
		std::forward<Arg4>(arg4)
	);
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

template <typename Type,
	typename Arg1,
	typename Arg2,
	typename Arg3,
	typename Arg4,
	typename Arg5>
	inline Type * Create(
		Arg1&& arg1,
		Arg2&& arg2,
		Arg3&& arg3,
		Arg4&& arg4,
		Arg5&& arg5
	)
{
	auto newObj = new (std::nothrow) Type(
		std::forward<Arg1>(arg1),
		std::forward<Arg2>(arg2),
		std::forward<Arg3>(arg3),
		std::forward<Arg4>(arg4),
		std::forward<Arg5>(arg5)
	);
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

template <typename Type,
	typename Arg1,
	typename Arg2,
	typename Arg3,
	typename Arg4,
	typename Arg5,
	typename Arg6>
	inline Type * Create(
		Arg1&& arg1,
		Arg2&& arg2,
		Arg3&& arg3,
		Arg4&& arg4,
		Arg5&& arg5,
		Arg6&& arg6
	)
{
	auto newObj = new (std::nothrow) Type(
		std::forward<Arg1>(arg1),
		std::forward<Arg2>(arg2),
		std::forward<Arg3>(arg3),
		std::forward<Arg4>(arg4),
		std::forward<Arg5>(arg5),
		std::forward<Arg6>(arg6)
	);
	if (newObj) { newObj->autorelease(); return newObj; }
	return nullptr;
}

#endif


}