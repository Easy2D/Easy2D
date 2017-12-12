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

// ��ʾ��ά�����Ľṹ��
typedef EPoint EVec;

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

// ��ʾ������ʽ�Ľṹ��
struct EWindowStyle
{
	LPCTSTR m_pIconID;	/* ����ͼ�� ID */
	bool m_bNoClose;	/* ���ùرհ�ť */
	bool m_bNoMiniSize;	/* ������С����ť */
	bool m_bTopMost;	/* �����ö� */

	EWindowStyle()
	{
		m_pIconID = 0;
		m_bNoClose = false;
		m_bNoMiniSize = false;
		m_bTopMost = false;
	}

	EWindowStyle(
		LPCTSTR pIconID
	)
	{
		m_pIconID = pIconID;
		m_bNoClose = false;
		m_bNoMiniSize = false;
		m_bTopMost = false;
	}

	EWindowStyle(
		LPCTSTR pIconID,
		bool bNoClose,
		bool bNoMiniSize,
		bool bTopMost
	)
	{
		m_pIconID = pIconID;
		m_bNoClose = bNoClose;
		m_bNoMiniSize = bNoMiniSize;
		m_bTopMost = bTopMost;
	}
};

// �ַ���
class EString
{
public:
	EString();
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

	friend std::wistream &operator>>(std::wistream &, EString &);

	operator const wchar_t*() const { return _string; }
	operator bool() const { return _size != 0; }

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

// ��ɫ
class EColor
{
public:
	enum COMMON_VALUE
	{
		ALICE_BLUE = 0xF0F8FF,
		ANTIQUE_WHITE = 0xFAEBD7,
		AQUA = 0x00FFFF,
		AQUAMARINE = 0x7FFFD4,
		AZURE = 0xF0FFFF,
		BEIGE = 0xF5F5DC,
		BISQUE = 0xFFE4C4,
		BLACK = 0x000000,
		BLANCHED_ALMOND = 0xFFEBCD,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		BURLY_WOOD = 0xDEB887,
		CADET_BLUE = 0x5F9EA0,
		CHARTREUSE = 0x7FFF00,
		CHOCOLATE = 0xD2691E,
		CORAL = 0xFF7F50,
		CORNFLOWER_BLUE = 0x6495ED,
		CORNSILK = 0xFFF8DC,
		CRIMSON = 0xDC143C,
		CYAN = 0x00FFFF,
		DARK_BLUE = 0x00008B,
		DARK_CYAN = 0x008B8B,
		DARK_GOLDENROD = 0xB8860B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_KHAKI = 0xBDB76B,
		DARK_MAGENTA = 0x8B008B,
		DARK_OLIVE_GREEN = 0x556B2F,
		DARK_ORANGE = 0xFF8C00,
		DARK_ORCHID = 0x9932CC,
		DARK_RED = 0x8B0000,
		DARK_SALMON = 0xE9967A,
		DARK_SEA_GREEN = 0x8FBC8F,
		DARK_SLATE_BLUE = 0x483D8B,
		DARK_SLATE_GRAY = 0x2F4F4F,
		DARK_TURQUOISE = 0x00CED1,
		DARK_VIOLET = 0x9400D3,
		DEEP_PINK = 0xFF1493,
		DEEP_SKY_BLUE = 0x00BFFF,
		DIM_GRAY = 0x696969,
		DODGER_BLUE = 0x1E90FF,
		FIREBRICK = 0xB22222,
		FLORAL_WHITE = 0xFFFAF0,
		FOREST_GREEN = 0x228B22,
		FUCHSIA = 0xFF00FF,
		GAINSCORO = 0xDCDCDC,
		GHOST_WHITE = 0xF8F8FF,
		GOLD = 0xFFD700,
		GOLDENROD = 0xDAA520,
		GRAY = 0x808080,
		GREEN = 0x008000,
		GREEN_YELLOW = 0xADFF2F,
		HONEYDEW = 0xF0FFF0,
		HOT_PINK = 0xFF69B4,
		INDIAN_RED = 0xCD5C5C,
		INDIGO = 0x4B0082,
		IVORY = 0xFFFFF0,
		KHAKI = 0xF0E68C,
		LAVENDER = 0xE6E6FA,
		LAVENDER_BLUSH = 0xFFF0F5,
		LAWN_GREEN = 0x7CFC00,
		LEMON_CHIFFON = 0xFFFACD,
		LIGHT_BLUE = 0xADD8E6,
		LIGHT_CORAL = 0xF08080,
		LIGHT_CYAN = 0xE0FFFF,
		LIGHT_GOLDENROD_YELLOW = 0xFAFAD2,
		LIGHT_GREEN = 0x90EE90,
		LIGHT_GRAY = 0xD3D3D3,
		LIGHT_PINK = 0xFFB6C1,
		LIGHT_SALMON = 0xFFA07A,
		LIGHT_SEA_GREEN = 0x20B2AA,
		LIGHT_SKY_BLUE = 0x87CEFA,
		LIGHT_SLATE_GRAY = 0x778899,
		LIGHT_STEEL_BLUE = 0xB0C4DE,
		LIGHT_YELLOW = 0xFFFFE0,
		LIME = 0x00FF00,
		LIME_GREEN = 0x32CD32,
		LINEN = 0xFAF0E6,
		MAGENTA = 0xFF00FF,
		MAROON = 0x800000,
		MEDIUM_AQUAMARINE = 0x66CDAA,
		MEDIUM_BLUE = 0x0000CD,
		MEDIUM_ORCHID = 0xBA55D3,
		MEDIUM_PURPLE = 0x9370DB,
		MEDIUM_SEA_GREEN = 0x3CB371,
		MEDIUM_SLATE_BLUE = 0x7B68EE,
		MEDIUM_SPRING_GREEN = 0x00FA9A,
		MEDIUM_TURQUOISE = 0x48D1CC,
		MEDUIM_VIOLET_RED = 0xC71585,
		MIDNIGHT_BLUE = 0x191970,
		MINT_CREAM = 0xF5FFFA,
		MISTY_ROSE = 0xFFE4E1,
		MOCCASIN = 0xFFE4B5,
		NAVAJO_WHITE = 0xFFDEAD,
		NAVY = 0x000080,
		OLD_LACE = 0xFDF5E6,
		OLIVE = 0x808000,
		OLIVE_DRAB = 0x6B8E23,
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		ORCHID = 0xDA70D6,
		PALE_GOLDENROD = 0xEEE8AA,
		PALE_GREEN = 0x98FB98,
		PALE_TURQUOISE = 0xAFEEEE,
		PALE_VIOLET_RED = 0xDB7093,
		PAPAYA_WHIP = 0xFFEFD5,
		PEACH_PUFF = 0xFFDAB9,
		PERU = 0xCD853F,
		PINK = 0xFFC0CB,
		PLUM = 0xDDA0DD,
		POWDER_BLUE = 0xB0E0E6,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		ROSY_BROWN = 0xBC8F8F,
		ROYAL_BLUE = 0x4169E1,
		SADDLE_BROWN = 0x8B4513,
		SALMON = 0xFA8072,
		SANDY_BROWN = 0xF4A460,
		SEA_GREEN = 0x2E8B57,
		SEA_SHELL = 0xFFF5EE,
		SIENNA = 0xA0522D,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SLATE_BLUE = 0x6A5ACD,
		SLATE_GRAY = 0x708090,
		SNOW = 0xFFFAFA,
		SPRING_GREEN = 0x00FF7F,
		STEEL_BLUE = 0x4682B4,
		TAN = 0xD2B48C,
		TEAL = 0x008080,
		THISTLE = 0xD8BFD8,
		TOMATO = 0xFF6347,
		TURQUOISE = 0x40E0D0,
		VIOLET = 0xEE82EE,
		WHEAT = 0xF5DEB3,
		WHITE = 0xFFFFFF,
		WHITE_SMOKE = 0xF5F5F5,
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


// �����Ϣ
class EMouseMsg
{
public:
	// �����Ϣ����
	enum MOUSE_MSG
	{
		MOVE = 0x0200,	// ����ƶ�
		LBUTTON_DOWN,	// ����������
		LBUTTON_UP,		// ������̧��
		LBUTTON_DBLCLK,	// ������˫��
		RBUTTON_DOWN,	// ����Ҽ�����
		RBUTTON_UP,		// ����Ҽ�̧��
		RBUTTON_DBLCLK,	// ����Ҽ�˫��
		MBUTTON_DOWN,	// ����м�����
		MBUTTON_UP,		// ����м�̧��
		MBUTTON_DBLCLK,	// ����м�˫��
		WHEEL			// ��������
	};

	// ��ȡ��������
	static DWORD getPosX();

	// ��ȡ���������
	static DWORD getPosY();

	// ��ȡ�������
	static EPoint getPos();

	// ��ȡ����������״̬
	static bool isLButtonDown();

	// ��ȡ����м�����״̬
	static bool isMButtonDown();

	// ��ȡ����Ҽ�����״̬
	static bool isRButtonDown();

	// ��ȡ Shift ����״̬
	static bool isShiftDown();

	// ��ȡ Ctrl ����״̬
	static bool isCtrlDown();

	// ��ȡ������ֵ
	static DWORD getWheelDelta();

	// ��ȡ��ǰ�����Ϣ����
	static MOUSE_MSG getMsg();

public:
	static UINT s_nMsg;
	static WPARAM s_wParam;
	static LPARAM s_lParam;
};


// ������Ϣ
class EKeyboardMsg
{
public:
	// ������Ϣ���ͼ���
	enum KEYBOARD_MSG
	{
		KEY_DOWN = 0x0100,	// ����
		KEY_UP				// ̧��
	};

	// ������ֵ����
	enum class KEY
	{
		A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,					// ��ĸ��ֵ
		NUM0 = '0', NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,									// ���ּ�ֵ
		NUMPAD0 = 0x60, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,	// ����С���̼�ֵ
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,											// F����ֵ
		MULTIPLY,		// �˺ż���ֵ
		ADD,			// �Ӻż���ֵ
		SEPARATOR,		// �ָ����ֵ
		SUBTRACT,		// ���ż���ֵ
		DECIMAL,		// С�������ֵ
		DIVIDE,			// ���ż���ֵ
		TAB = 0x09,		// TAB ����ֵ
		ENTER = 0x0D,	// �س�����ֵ
		SHIFT, CTRL, 	// SHIFT ����ֵ
		ESC = 0x1B, 	// ESCAPE ����ֵ
		SPACE = 0x20, 	// �ո����ֵ
		PAGE_UP, 		// PageUp ����ֵ
		PAGE_DOWN, 		// PageDown ����ֵ
		END, 			// End ����ֵ
		HOME, 			// Home ����ֵ
		LEFT, 			// �����ֵ
		UP, 			// �ϼ���ֵ
		RIGHT, 			// �Ҽ���ֵ
		DOWN			// �¼���ֵ
	};

	// ��ȡ������Ϣ����
	static KEYBOARD_MSG getMsg();

	// ��ȡ��ֵ
	static KEY getKeyValue();

	// ��ȡ������Ϣ�ļ���
	static DWORD getCount();

	// ��ȡ�ض�������״̬
	static bool isKeyDown(
		KEY key
	);

	// ��ȡ��Сд����״̬
	static bool isCapitalLockOn();

	// ��ȡ����С��������״̬
	static bool isNumpadLockOn();

	// ��ȡ��������״̬
	static bool isScrollLockOn();

public:
	static UINT s_nMsg;
	static WPARAM s_wParam;
	static LPARAM s_lParam;
};


class EGeometry;

// ������Ϣ
class EPhysicsMsg
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

class ETexture :
	public EObject
{
	friend ESprite;

public:
	// ����һ���յ�����
	ETexture();

	// �ӱ����ļ��ж�ȡ��Դ
	ETexture(
		const EString & fileName
	);

	// ��ȡ������Դ
	ETexture(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	virtual ~ETexture();

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


class ESpriteFrame :
	public EObject
{
	friend ESprite;

public:
	// �����յľ���֡
	ESpriteFrame();

	// �����յľ���֡
	ESpriteFrame(
		ETexture * texture
	);

	// �����յľ���֡
	ESpriteFrame(
		const EString & imageFileName
	);

	// �����յľ���֡
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// �����յľ���֡
	ESpriteFrame(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// �����յľ���֡
	ESpriteFrame(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// �����յľ���֡
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESpriteFrame();

	// ��ȡ���
	float getWidth() const;

	// ��ȡ�߶�
	float getHeight() const;

	// ��ȡ����
	ETexture * getTexture() const;

protected:
	// ��ȡ����
	void _setTexture(
		ETexture * texture
	);

	// �ü�����
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
	ETexture * m_pTexture;
};

class ENode;

// ��ʱ���ص�����������Ϊ�ö�ʱ�������õĴ������� 0 ��ʼ��
typedef std::function<void(int)> TIMER_CALLBACK;

// ��ť����ص�����
typedef std::function<void()> BUTTON_CLICK_CALLBACK;

// ������Ϣ�����ص�����
typedef std::function<void()> KEY_LISTENER_CALLBACK;

// �����Ϣ�����ص�����
typedef std::function<void()> MOUSE_LISTENER_CALLBACK;

// �������Ϣ�����ص�����������Ϊ���λ�ã�
typedef std::function<void(EPoint mousePos)> MOUSE_CLICK_LISTENER_CALLBACK;

// ��갴����Ϣ�����ص�����������Ϊ����λ�ã�
typedef MOUSE_CLICK_LISTENER_CALLBACK  MOUSE_PRESS_LISTENER_CALLBACK;

// ���˫����Ϣ�����ص�����������Ϊ˫��λ�ã�
typedef MOUSE_CLICK_LISTENER_CALLBACK  MOUSE_DBLCLK_LISTENER_CALLBACK;

// ����϶���Ϣ�����ص�����������Ϊ�϶�ǰλ�ú��϶���λ�ã�
typedef std::function<void(EPoint begin, EPoint end)> MOUSE_DRAG_LISTENER_CALLBACK;

// ����������Ϣ�������ص�����
typedef std::function<void()> PHYSICS_LISTENER_CALLBACK;

// ��ײ��Ϣ�������ص�����
typedef PHYSICS_LISTENER_CALLBACK  COLLISION_LISTENER_CALLBACK;

}