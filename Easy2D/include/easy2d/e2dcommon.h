#pragma once
#include <ostream>
#include <easy2d/e2dmacros.h>
#include <easy2d/e2dmath.h>

namespace easy2d
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


// �ַ���
using String = std::wstring;

// խ�ַ���
using ByteString = std::string;


// ��������
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


// ��ʽ���ַ���
ByteString FormatString(const char* format, ...);

// ��ʽ���ַ���
String FormatString(const wchar_t* format, ...);

// ���ַ���תխ�ַ���
ByteString WideToNarrow(const String& str);

// խ�ַ���ת���ַ���
String NarrowToWide(const ByteString& str);


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
		YellowGreen = 0x9ACD32
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


// ����
class Font
{
public:
	String	family;			// ������
	float	size;			// �ֺ�
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
		float size = 22,
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
	virtual float getWidth() const;

	// ��ȡ�߶�
	virtual float getHeight() const;

	// ��ȡ��С
	virtual Size getSize() const;

	// ��ȡԴͼƬ���
	virtual float getSourceWidth() const;

	// ��ȡԴͼƬ�߶�
	virtual float getSourceHeight() const;

	// ��ȡԴͼƬ��С
	virtual Size getSourceSize() const;
	
	// ��ȡ�ü�λ�� X ����
	virtual float getCropX() const;

	// ��ȡ�ü�λ�� Y ����
	virtual float getCropY() const;

	// ��ȡ�ü�λ��
	virtual Point getCropPos() const;

	// ��ָ��λ����ȾͼƬ
	void draw(const Rect& destRect, float opacity) const;

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


// ����ֵ
struct MouseCode
{
	enum Value : int
	{
		Left,		/* ������ */
		Right,		/* ����Ҽ� */
		Middle		/* ����м� */
	};
};


// ���̼�ֵ
struct KeyCode
{
	enum Value : int
	{
		Unknown = 0,
		Up = VK_UP,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		Enter = VK_RETURN,
		Space = VK_SPACE,
		Esc = VK_ESCAPE,
		Ctrl = VK_CONTROL,
		Shift = VK_SHIFT,
		Alt = VK_MENU,
		Tab = VK_TAB,
		Delete = VK_DELETE,
		Back = VK_BACK,

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

		Num0 = 0x30,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		Numpad0 = VK_NUMPAD0,
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


// �¼�
struct Event
{
	enum Type : UINT
	{
		Unknown = 0,

		MouseMove,		// ����ƶ�
		MouseDown,		// ��갴��
		MouseUp,		// ���̧��
		MouseWheel,		// �����ֻ���
		KeyDown,		// ��������
		KeyUp,			// ����̧��

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


// �¼�������
class Listener
	: public Object
{
	friend class Input;

public:
	using Callback = Function<void(Event*)>;

	Listener();

	Listener(
		const Callback& func,
		const String& name,
		bool paused
	);

	// ��������
	void start();

	// ֹͣ����
	void stop();

	// ��ȡ����������״̬
	bool isRunning() const;

	// ��ȡ����
	String getName() const;

	// ��������
	void setName(
		const String& name
	);

	// ���ü����ص�����
	void setCallback(
		const Callback& func
	);

	// �����¼�
	virtual void handle(Event* evt);

	void done();

	bool isDone();

protected:
	bool _running;
	bool _done;
	String _name;
	Callback _callback;
};

// ��Դ
class Resource
{
public:
	// ��Դ�Ķ���������
	struct Data
	{
		void* buffer;	// ��Դ����
		int size;	// ��Դ���ݴ�С

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

	Resource();

	Resource(
		int id,				/* ��Դ ID */
		const String& type	/* ��Դ���� */
	);

	// ������Դ�Ķ���������
	Resource::Data loadData() const;

	// ��ȡ��Դ ID
	int getId() const;

	// ��ȡ��Դ����
	String getType() const;

private:
	int		_id;
	String	_type;

	mutable Resource::Data _data;
};


}