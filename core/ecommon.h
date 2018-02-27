#pragma once
#include "emacros.h"
#include <vector>
#include <functional>
#include <sstream>

namespace e2d
{


// ��ʾ����Ľṹ��
struct Point
{
	double x;
	double y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	Point operator + (Point const & p)
	{
		return Point(x + p.x, y + p.y);
	}

	Point operator - (Point const & p)
	{
		return Point(x - p.x, y - p.y);
	}

	Point operator * (double const & value)
	{
		return Point(x * value, y * value);
	}

	Point operator / (double const & value)
	{
		return Point(x / value, y / value);
	}
};

// ��ʾ��С�Ľṹ��
struct Size
{
	double width;
	double height;

	Size()
	{
		width = 0;
		height = 0;
	}

	Size(double width, double height)
	{
		this->width = width;
		this->height = height;
	}

	Size operator + (Size const & size)
	{
		return Size(width + size.width, height + size.height);
	}

	Size operator - (Size const & size)
	{
		return Size(width - size.width, height - size.height);
	}

	Size operator * (double const & value)
	{
		return Size(width * value, height * value);
	}

	Size operator / (double const & value)
	{
		return Size(width / value, height / value);
	}
};


// �ַ���
class String
{
public:
	String();
	String(const wchar_t *);
	String(const String &);
	String(const std::wstring &);
	String(String &&);

	~String();

	String& operator= (const wchar_t *);
	String& operator= (const String &);
	String& operator= (const std::wstring &);

	bool operator== (const wchar_t *);
	bool operator== (const String &);
	bool operator== (const std::wstring &);

	bool operator!= (const wchar_t *);
	bool operator!= (const String &);
	bool operator!= (const std::wstring &);

	wchar_t &operator[] (int);

	String operator+ (const wchar_t);
	String operator+ (const wchar_t *);
	String operator+ (const String &);
	String operator+ (const std::wstring &);

	friend String operator+ (const wchar_t, const String &);
	friend String operator+ (const wchar_t*, const String &);
	friend String operator+ (const std::wstring &, const String &);

	String& operator+= (const wchar_t);
	String& operator+= (const wchar_t *);
	String& operator+= (const String &);
	String& operator+= (const std::wstring &);

	template<typename T>
	String& operator<< (const T value)
	{
		std::wostringstream ss;
		ss << value;
		return (*this) += ss.str();
	}

	operator const wchar_t* () const;
	operator bool () const;

	friend std::wistream& operator>> (std::wistream &, String &);

	// �ж��ַ����Ƿ�Ϊ��
	bool isEmpty() const;

	// ��ȡ�ַ�������
	int getLength() const;

	// ��ȡ���ַ�����ɢ��ֵ
	unsigned int getHash() const;

	// ����ַ�
	String &append(const wchar_t ch);

	// ����ַ���
	String &append(const wchar_t *str);

	// ����ַ���
	String &append(const String &str);

	// ����ַ���
	template<typename T>
	String &append(const T &value)
	{
		return (*this) += value;
	}

	// ��ȡ�ü��ַ���
	String subtract(int offset, int count = -1) const;

	// ��ȡ�ַ����е�һ���ض��ַ����±�
	int findFirstOf(const wchar_t ch) const;

	// ��ȡ�ַ��������һ���ض��ַ����±�
	int findLastOf(const wchar_t ch) const;

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

	// ����������ת��Ϊ�ַ���
	template<typename T>
	static String toString(const T value)
	{
		std::wostringstream ss;
		ss << value;
		return ss.str();
	}

private:
	std::wstring m_str;
};


// ��ɫ
class Color
{
public:
	enum COMMON_VALUE
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
};

// �����ϸֵ
class FontWeight
{
public:
	enum COMMON_VALUE
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


// ��ֵ����
class KeyCode
{
public:
	enum VALUE
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
		NUMPAD0 = 0x52,
		F1 = 0x3B,
		F2 = 0x3C,
		F3 = 0x3D,
		F4 = 0x3E,
		F5 = 0x3F,
		F6 = 0x40,
		F7 = 0x41,
		F8 = 0x42,
		F9 = 0x43,
		F10 = 0x44
	};
};


// ��״������ϵ
class Relation
{
public:
	enum VALUE
	{
		UNKNOWN = 0,		/* ��ϵ��ȷ�� */
		DISJOINT = 1,		/* û�н��� */
		IS_CONTAINED = 2,	/* ��ȫ������ */
		CONTAINS = 3,		/* ��ȫ���� */
		OVERLAP = 4			/* �����ص� */
	};
};


class ObjectManager;

// ��������
class Obj
{
	friend ObjectManager;

public:
	Obj();

	virtual ~Obj();

	// ���ü�����һ
	void retain();

	// ���ü�����һ
	void release();

private:
	int m_nRefCount;
	bool m_bManaged;
};


class Text;

class Font :
	public Obj
{
	friend Text;

public:
	Font();

	Font(
		String fontFamily,
		double fontSize = 22,
		UINT32 color = Color::WHITE,
		UINT32 fontWeight = FontWeight::REGULAR,
		bool italic = false
	);

	virtual ~Font();

	// ��ȡ��ǰ�ֺ�
	double getFontSize() const;

	// ��ȡ��ǰ�����ϸֵ
	UINT32 getFontWeight() const;

	// ��ȡ������ɫ
	UINT32 getColor() const;

	// �Ƿ���б��
	bool isItalic() const;

	// ��������
	void setFamily(
		const String & fontFamily
	);

	// �����ֺ�
	void setSize(
		double fontSize
	);

	// ���������ϸֵ
	void setWeight(
		UINT32 fontWeight
	);

	// ����������ɫ
	void setColor(
		UINT32 color
	);

	// ��������б��
	void setItalic(
		bool value
	);

protected:
	// �������ָ�ʽ
	void _initTextFormat();

	// ��ȡ���ָ�ʽ
	IDWriteTextFormat * _getTextFormat();

protected:
	String		m_sFontFamily;
	float		m_fFontSize;
	UINT32		m_FontWeight;
	UINT32		m_Color;
	bool		m_bItalic;
	bool		m_bRecreateNeeded;
	IDWriteTextFormat * m_pTextFormat;
};


// ͼƬ
class Image :
	public Obj
{
public:
	// ����һ���յ�ͼƬ
	Image();

	// �ӱ����ļ��ж�ȡ��Դ
	Image(
		LPCTSTR strFilePath	/* ͼƬ�ļ�·�� */
	);

	// �ӱ����ļ��ж�ȡ��Դ
	Image(
		LPCTSTR strFilePath,/* ͼƬ�ļ�·�� */
		double nClipX,		/* �ü�λ�� X ���� */
		double nClipY,		/* �ü�λ�� Y ���� */
		double nClipWidth,	/* �ü���� */
		double nClipHeight	/* �ü��߶� */
	);

	virtual ~Image();

	// �ü�ͼƬ
	void clip(
		double nClipX,		/* �ü�λ�� X ���� */
		double nClipY,		/* �ü�λ�� Y ���� */
		double nClipWidth,	/* �ü���� */
		double nClipHeight	/* �ü��߶� */
	);

	// �ӱ����ļ��ж�ȡͼƬ
	void loadFrom(
		const String & strFilePath
	);

	// �ӱ����ļ��ж�ȡͼƬ���ü�
	void loadFrom(
		const String & strFilePath,/* ͼƬ�ļ�·�� */
		double nClipX,				/* �ü�λ�� X ���� */
		double nClipY,				/* �ü�λ�� Y ���� */
		double nClipWidth,			/* �ü���� */
		double nClipHeight			/* �ü��߶� */
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
	virtual double getClipX() const;

	// ��ȡ�ü�λ�� Y ����
	virtual double getClipY() const;

	// ��ȡ�ü�λ��
	virtual Point getClipPos() const;

	// ��ȡ ID2D1Bitmap ����
	ID2D1Bitmap * getBitmap();

	// Ԥ������Դ
	static bool preload(
		const String & sFileName	/* ͼƬ�ļ�·�� */
	);

	// ��ջ���
	static void clearCache();

protected:
	double	m_fSourceClipX;
	double	m_fSourceClipY;
	double	m_fSourceClipWidth;
	double	m_fSourceClipHeight;
	ID2D1Bitmap * m_pBitmap;
};


class SceneManager;
class Node;
class Action;

// ����
class Scene :
	public Obj
{
	friend SceneManager;

public:
	Scene();

	virtual ~Scene();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter() {}

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit() {}

	// ��д�����������������ײ����ʱ�Զ�ִ��
	virtual void onCollide(
		Node * pActiveNode,	/* ����������ײ�Ľڵ� */
		Node * pPassiveNode	/* ����������ײ�Ľڵ� */
	) {}

	// ��д��������������ڹرմ���ʱִ��
	virtual bool onCloseWindow() { return true; }

	// ��д���������������ÿһ֡����ˢ��ʱִ��
	virtual void onUpdate() {}

	// ��д�����������������Ϸ��ͣʱִ��
	virtual void onPause() {}

	// ��������� onUpdate ����
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// ��ӽڵ㵽����
	void add(
		Node * child,
		int zOrder = 0
	);

	// ɾ���ӽڵ�
	bool remove(
		Node * child
	);

	// ��ȡ���ڵ�
	Node * getRoot() const;

	// ��������ͼ�ε���Ⱦ
	void setShapeVisiable(
		bool visiable
	);

protected:
	// ��Ⱦ��������
	void _render();

	// ���³�������
	void _update();

protected:
	bool m_bAutoUpdate;
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bShapeVisiable;
	Node * m_pRoot;
};


// ��ά����
typedef Point Vector;

// ����ֵ�Ͳ����б�Ϊ�յĺ���
typedef std::function<void(void)> VoidFunction;

// ��ʱ���ص�����
typedef VoidFunction TimerCallback;

// ��ť����ص�����
typedef VoidFunction ButtonCallback;

template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }

template<typename Obj>
inline void SafeRelease(Obj** p) { if (*p) { (*p)->release(); *p = nullptr; } }

template<class Interface>
inline void SafeReleaseInterface(Interface **pp) { if (*pp != nullptr) { (*pp)->Release(); (*pp) = nullptr; } }

}