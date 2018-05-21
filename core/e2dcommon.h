#pragma once
#include "e2dmacros.h"
#include "e2dcustom.h"

namespace e2d
{



// ����
enum class Direction : int
{
	UP,			/* �� */
	DOWN,		/* �� */
	LEFT,		/* �� */
	RIGHT		/* �� */
};


// �����ཻ��ʽ
enum class LineJoin : int
{
	MITER = 0,	/* б�� */
	BEVEL = 1,	/* б�� */
	ROUND = 2	/* Բ�� */
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

	Point operator + (Point const & p) const;
	Point operator - (Point const & p) const;
	Point operator * (double const & value) const;
	Point operator / (double const & value) const;
	Point operator - () const;

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
		UINT rgb
	);

	Color(
		UINT rgb,
		double alpha
	);

	D2D1_COLOR_F toD2DColorF() const;

public:
	enum RGB_VALUE : UINT
	{
		BLACK = 0x000000,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		CHOCOLATE = 0xD2691E,
		DARK_BLUE = 0x00008B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_ORANGE = 0xFF8C00,
		DARK_RED = 0x8B0000,
		DARK_VIOLET = 0x9400D3,
		FOREST_GREEN = 0x228B22,
		GOLD = 0xFFD700,
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
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		PINK = 0xFFC0CB,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SNOW = 0xFFFAFA,
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
		UINT rgb,
		double alpha
	);

public:
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

	Image(
		const String& filePath	/* ͼƬ�ļ�·�� */
	);

	Image(
		int resNameId,			/* ͼƬ��Դ���� */
		const String& resType	/* ͼƬ��Դ���� */
	);

	Image(
		const String& filePath,	/* ͼƬ�ļ�·�� */
		double cropX,			/* �ü�λ�� X ���� */
		double cropY,			/* �ü�λ�� Y ���� */
		double cropWidth,		/* �ü���� */
		double cropHeight		/* �ü��߶� */
	);

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


class Input;
class Collision;

// ������
class Listener
{
	friend Input;
	friend Collision;

public:
	Listener(
		const Function& func,
		const String& name,
		bool paused
	);

	// ���¼�����״̬
	virtual void update();

	// ��ȡ����������״̬
	bool isRunning() const;

	// ��ȡ����
	String getName() const;

	// ��������
	void setName(
		const String& name
	);

protected:
	bool _running;
	bool _stopped;
	String _name;
	Function _callback;
};


}