#pragma once
#include "emacros.h"
#include <vector>
#include <functional>
#include <sstream>

namespace e2d
{


// ��ʾ����Ľṹ��
struct EPoint
{
	float x;
	float y;

	EPoint()
	{
		x = 0;
		y = 0;
	}

	EPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	EPoint operator + (EPoint const & p)
	{
		return EPoint(x + p.x, y + p.y);
	}

	EPoint operator - (EPoint const & p)
	{
		return EPoint(x - p.x, y - p.y);
	}

	EPoint operator * (float const & value)
	{
		return EPoint(x * value, y * value);
	}

	EPoint operator / (float const & value)
	{
		return EPoint(x / value, y / value);
	}
};

// ��ʾ��С�Ľṹ��
struct ESize
{
	float width;
	float height;

	ESize()
	{
		width = 0;
		height = 0;
	}

	ESize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	ESize operator + (ESize const & size)
	{
		return ESize(width + size.width, height + size.height);
	}

	ESize operator - (ESize const & size)
	{
		return ESize(width - size.width, height - size.height);
	}

	ESize operator * (float const & value)
	{
		return ESize(width * value, height * value);
	}

	ESize operator / (float const & value)
	{
		return ESize(width / value, height / value);
	}
};

// �ַ���
class EString
{
public:
	EString();
	EString(const wchar_t);
	EString(const wchar_t *);
	EString(const EString &);
	EString(const std::wstring &);
	EString(EString &&);

	~EString();

	EString& operator=(const wchar_t *);
	EString& operator=(const EString &);
	EString& operator=(const std::wstring &);

	bool operator==(const wchar_t *);
	bool operator==(const EString &);
	bool operator==(const std::wstring &);

	bool operator!=(const wchar_t *);
	bool operator!=(const EString &);
	bool operator!=(const std::wstring &);

	wchar_t &operator[](int);

	EString operator+(const wchar_t);
	EString operator+(const wchar_t *);
	EString operator+(const EString &);
	EString operator+(const std::wstring &);

	template<typename T>
	EString operator+(const T value)
	{
		EString str_temp(*this);

		str_temp += value;
		return std::move(str_temp);
	}

	EString &operator +=(const wchar_t);
	EString &operator +=(const wchar_t *);
	EString &operator +=(const EString &);
	EString &operator +=(const std::wstring &);

	template<typename T>
	EString &operator +=(const T value)
	{
		std::wostringstream ss;
		ss << value;
		return (*this) += ss.str();
	}

	bool operator < (EString const&) const;
	bool operator <= (EString const&) const;
	bool operator > (EString const&) const;
	bool operator >= (EString const&) const;

	operator wchar_t*() const { return _string; }
	operator bool() const { return _size != 0; }

	friend EString operator+(const wchar_t, const EString &);
	friend EString operator+(const wchar_t*, const EString &);
	friend EString operator+(const EString &, const EString &);
	friend EString operator+(const std::wstring &, const EString &);
	template<typename T>
	friend EString operator+(const T &value, const EString &str)
	{
		return std::move((EString::parse(value) + str2));
	}

	friend std::wistream &operator>>(std::wistream &, EString &);

	// �ж��ַ����Ƿ�Ϊ��
	bool isEmpty() const { return _size == 0; }

	// ��ȡ�ַ�������
	int length() const { return _size; }

	// ��ȡ��д�ַ���
	EString upper() const;

	// ��ȡСд�ַ���
	EString lower() const;

	// ��ȡ�ü��ַ���
	EString sub(int offset, int count = -1) const;

	// ��ȡ�ַ����е�һ���ض��ַ����±�
	int findFirstOf(const wchar_t ch) const;

	// ��ȡ�ַ��������һ���ض��ַ����±�
	int findLastOf(const wchar_t ch) const;

	// ����ַ�
	EString &append(const wchar_t ch);

	// ����ַ���
	EString &append(const wchar_t *str);

	// ����ַ���
	EString &append(const EString &str);

	// ����ַ���
	template<typename T>
	EString &append(const T &value)
	{
		return (*this) += value;
	}

	// ��ȡ���ַ�����ɢ��ֵ
	unsigned int hash() const;

	// ��ģ������ת��Ϊ�ַ���
	template<typename T>
	static EString parse(const T value)
	{
		EString str;

		std::wostringstream ss;
		ss << value;
		str += ss.str();

		return std::move(str);
	}

private:
	wchar_t *_string;
	int _size;
};


// ��ά����
typedef EPoint EVector2;

// ��ʱ���ص�����������Ϊ�ö�ʱ�������õĴ������� 0 ��ʼ��
typedef std::function<void(int)> TimerCallback;

// ��ť����ص�����
typedef std::function<void()> BtnClkCallback;

// ����������Ϣ�������ص�����
typedef std::function<void()> PhysLsnrCallback;

// ��ײ��Ϣ�������ص�����
typedef PhysLsnrCallback  ClsLsnrCallback;

template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }

template<typename Obj>
inline void SafeRelease(Obj** p) { if (*p) { (*p)->release(); *p = nullptr; } }

template<class Interface>
inline void SafeReleaseInterface(Interface **pp) { if (*pp != nullptr) { (*pp)->Release(); (*pp) = nullptr; } }


// ��ɫ
class EColor
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
class EFontWeight
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
class EKeyCode
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


class EGeometry;

// ������Ϣ
class EPhysicsEvent
{
public:
	enum INTERSECT_RELATION
	{
		UNKNOWN = 0,		/* ��ϵ��ȷ�� */
		DISJOINT = 1,		/* û�н��� */
		IS_CONTAINED = 2,	/* ��ȫ������ */
		CONTAINS = 3,		/* ��ȫ���� */
		OVERLAP = 4			/* �����ص� */
	};

	// ��ȡ��ǰ������ײ��Ϣ����
	static INTERSECT_RELATION getMsg();

	// ��ȡ������
	static EGeometry * getActiveGeometry();

	// ��ȡ������
	static EGeometry * getPassiveGeometry();

public:
	static INTERSECT_RELATION s_nRelation;
	static EGeometry * s_pActiveGeometry;
	static EGeometry * s_pPassiveGeometry;
};


class EObjectManager;

class EObject
{
	friend EObjectManager;

public:
	EObject();

	virtual ~EObject();

	// ���ü�����һ
	void retain();

	// ���ü�����һ
	void release();

private:
	int m_nRefCount;
	bool m_bManaged;
};


class EText;

class EFont :
	public EObject
{
	friend EText;

public:
	EFont();

	EFont(
		EString fontFamily,
		float fontSize = 22,
		UINT32 color = EColor::WHITE,
		UINT32 fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EFont();

	// ��ȡ��ǰ�ֺ�
	float getFontSize() const;

	// ��ȡ��ǰ�����ϸֵ
	UINT32 getFontWeight() const;

	// ��ȡ������ɫ
	UINT32 getColor() const;

	// �Ƿ���б��
	bool isItalic() const;

	// ��������
	void setFamily(
		const EString & fontFamily
	);

	// �����ֺ�
	void setSize(
		float fontSize
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
	EString		m_sFontFamily;
	float		m_fFontSize;
	UINT32		m_FontWeight;
	UINT32		m_Color;
	bool		m_bItalic;
	bool		m_bRecreateNeeded;
	IDWriteTextFormat * m_pTextFormat;
};


class ESprite;

class EImage :
	public EObject
{
	friend ESprite;

public:
	// ����һ���յ�ͼƬ
	EImage();

	// �ӱ����ļ��ж�ȡ��Դ
	EImage(
		LPCTSTR fileName
	);

	// ��ȡ������Դ
	EImage(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	virtual ~EImage();

	// �ӱ����ļ��ж�ȡ��Դ
	void loadFromFile(
		const EString & fileName
	);

	// ��ȡ������Դ
	void loadFromResource(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// ��ȡԴͼƬ���
	virtual float getSourceWidth() const;

	// ��ȡԴͼƬ�߶�
	virtual float getSourceHeight() const;

	// ��ȡԴͼƬ��С
	virtual ESize getSourceSize() const;

	// Ԥ������Դ
	static bool preload(
		const EString & fileName
	);

	// Ԥ������Դ
	static bool preload(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// ��ջ���
	static void clearCache();

protected:
	ID2D1Bitmap * _getBitmap();

protected:
	ID2D1Bitmap * m_pBitmap;
};


class EKeyframe :
	public EObject
{
	friend ESprite;

public:
	// �����յĹؼ�֡
	EKeyframe();

	// �����յĹؼ�֡
	EKeyframe(
		EImage * texture
	);

	// �����յĹؼ�֡
	EKeyframe(
		const EString & imageFileName
	);

	// �����յĹؼ�֡
	EKeyframe(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// �����յĹؼ�֡
	EKeyframe(
		EImage * texture,
		float x,
		float y,
		float width,
		float height
	);

	// �����յĹؼ�֡
	EKeyframe(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// �����յĹؼ�֡
	EKeyframe(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~EKeyframe();

	// ��ȡ���
	float getWidth() const;

	// ��ȡ�߶�
	float getHeight() const;

	// ��ȡͼƬ
	EImage * getImage() const;

protected:
	// �޸�ͼƬ
	void _setImage(
		EImage * texture
	);

	// �ü�ͼƬ
	void _clipTexture(
		float x,
		float y,
		float width,
		float height
	);

protected:
	float	m_fSourceClipX;
	float	m_fSourceClipY;
	float	m_fSourceClipWidth;
	float	m_fSourceClipHeight;
	EImage * m_pImage;
};


class ESceneManager;
class ENode;
class EAction;

// ����
class EScene :
	public EObject
{
	friend ESceneManager;

public:
	EScene();

	virtual ~EScene();

	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter() {}

	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit() {}

	// ��д��������������ڴ��ڼ���ʱִ��
	virtual bool onActivate() { return false; }

	// ��д��������������ڴ��ڷǼ���ʱִ��
	virtual bool onInactive() { return false; }

	// ��д��������������ڹرմ���ʱִ��
	virtual bool onCloseWindow() { return true; }

	// ��д���������������ÿһ֡����ˢ��ʱִ��
	virtual void onUpdate() {}

	// ����ӽڵ㵽����
	void add(
		ENode * child,
		int zOrder = 0
	);

	// ɾ���ӽڵ�
	bool remove(
		ENode * child
	);

	// ɾ����ͬ���Ƶ��ӽڵ�
	void remove(
		const EString &childName
	);

	// ��ȡ�����ӽڵ�
	std::vector<e2d::ENode*> getChildren();

	// ��ȡ�ӽڵ�����
	size_t getChildrenCount() const;

	// �������ƻ�ȡ�ӽڵ�
	ENode * getChild(
		const EString &childName
	);

	// ��ȡ���ڵ�
	ENode * getRoot() const;

	// ��������ӳ�Ա
	void clearAllChildren();

	// ִ�ж���
	void runAction(
		EAction * action
	);

	// ��������ͼ�ε���Ⱦ
	void setGeometryVisiable(
		bool visiable
	);

protected:
	// ��Ⱦ��������
	void _render();

	// ���³�������
	void _update();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bGeometryVisiable;
	ENode * m_pRoot;
};

}