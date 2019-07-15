#pragma once
#include <e2dmacros.h>
#include <e2dmath.h>

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


// ��������
template <typename _Fty>
using Function = std::function<_Fty>;


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