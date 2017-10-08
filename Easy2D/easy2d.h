/******************************************************
* Easy2D Game Engine
* 
* Website: http://www.easy2d.cn
* Github:  https://github.com/Nomango/Easy2D
* Gitee:   https://gitee.com/werelone/Easy2D
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


#include <Windows.h>
#include <tchar.h>
#include <atltypes.h>
#include <atlimage.h>
#include <vector>
#include <stack>
#include <functional>


#if defined(UNICODE) && (_DEBUG)
	#pragma comment(lib,"Easy2Ddw.lib")
#elif !defined(UNICODE) && (_DEBUG)
	#pragma comment(lib,"Easy2Dd.lib")
#elif defined(UNICODE)
	#pragma comment(lib,"Easy2Dw.lib")
#elif !defined(UNICODE)
	#pragma comment(lib,"Easy2D.lib")
#endif


// Type Declare

typedef CPoint						CVector;
typedef unsigned int				VK_KEY;
typedef std::function<void()>		CLICK_CALLBACK;
typedef std::function<void()>		TIMER_CALLBACK;
typedef std::function<void(VK_KEY)>	KEY_CALLBACK;
typedef std::function<void()>		MOUSE_CALLBACK;

#ifdef UNICODE
	typedef std::wstring TString;
#else
	typedef std::string TString;
#endif

// Classes Declare

namespace easy2d
{
	class App;
	class Scene;

	class Object;
	class Node;
	class FontStyle;
	class Color;
	class RectNode;
	class Text;
	class Image;
	class BatchNode;
	class MouseNode;
	class Button;
	class TextButton;
	class ImageButton;
	class Layer;
	class Sprite;
	class BatchSprite;
	class Shape;
	class Circle;
	class Rectangle;

	class Action;
	class Animation;
	class ActionMoveTo;
	class ActionMoveBy;
	class ActionScaleTo;
	class ActionScaleBy;
	class ActionOpacityTo;
	class ActionOpacityBy;
	class ActionFadeIn;
	class ActionFadeOut;
	class ActionFrames;
	class ActionDelay;
	class ActionCallback;
	class ActionTwo;
	class ActionSequence;
	class ActionNeverStop;

	class MouseMsg;
	class KeyMsg;

	class FreePool;
	class FileUtils;
	class MusicUtils;
	class ActionManager;
}


// Classes

namespace easy2d
{

class App
{
	friend Scene;
public:
	App();
	~App();

	// ���ڿ�ѡģʽ
	enum MODE { SHOW_CONSOLE = 1, NO_CLOSE = 2, NO_MINI_MIZE = 4 };

	// �����ͼ����
	void createWindow(int width, int height, int mode = 0);
	// �����ͼ����
	void createWindow(CSize size, int mode = 0);
	// �����ͼ����
	void createWindow(TString title, int width, int height, int mode = 0);
	// �����ͼ����
	void createWindow(TString title, CSize size, int mode = 0);
	// ��������
	int run();
	// �ͷ������ڴ���Դ
	void free();

	// ��ȡ����ʵ��
	static App * get();
	// ��������ԭ��
	static void setOrigin(int originX, int originY);
	// ��ȡ����ԭ������������
	static int getOriginX();
	// ��ȡ����ԭ�������������
	static int getOriginY();
	// ��ֹ����
	static void quit();
	// ��ֹ����
	static void end();
	// �޸Ĵ��ڴ�С
	static void setWindowSize(int width, int height);
	// �޸Ĵ��ڴ�С
	static void setWindowSize(CSize size);
	// �رմ���
	static void close();
	// ���ô��ڱ���
	static void setWindowTitle(TString title);
	// ��ȡ���ڱ���
	static TString getWindowTitle();
	// ��ȡ���ڿ��
	static int getWidth();
	// ��ȡ���ڸ߶�
	static int getHeight();
	// �л�����
	static void enterScene(Scene *scene, bool save = true);
	// ������һ����
	static void backScene();
	// ���֮ǰ��������г���
	static void clearScene();
	// ���� AppName
	static void setAppName(TString appname);
	// ��ȡ AppName
	static TString getAppName();
	// �޸Ĵ��ڱ���ɫ
	static void setBkColor(COLORREF color);
	// ����֡��
	static void setFPS(DWORD fps);
	// ���û�ͼ��ʽΪĬ��ֵ
	static void reset();
	// ��ȡ��ǰ����
	static Scene * getCurrentScene();
	// ��ȡ�����ڼ����еĳ���
	static Scene * getLoadingScene();

protected:
	TString				m_sTitle;
	TString				m_sAppName;
	Scene*				m_pCurrentScene;
	Scene*				m_pNextScene;
	Scene*				m_pLoadingScene;
	std::stack<Scene*>	m_SceneStack;
	LARGE_INTEGER		m_nAnimationInterval;
	CSize				m_Size;
	int					m_nWindowMode;
	bool				m_bRunning;
	bool				m_bSaveScene;

protected:
	void _initGraph();
	void _mainLoop();
	void _enterNextScene();
};

class FreePool
{
	friend App;
	friend Object;
private:
	// ˢ���ڴ��
	static void __flush();
	// ��һ���ڵ�����ͷų�
	static void __add(Object * nptr);
	// ɾ�����нڵ�
	static void __clearAllObjects();
};

class Scene
{
	friend App;
	friend MouseMsg;
public:
	Scene();
	~Scene();

	// ��д�����������ʼ���������
	virtual void init() = 0;
	// ��д��������������ڽ����������ʱ�Զ�ִ��
	virtual void onEnter();
	// ��д����������������뿪�������ʱ�Զ�ִ��
	virtual void onExit();
	// ����ӳ�Ա������
	void add(Node * child, int zOrder = 0);
	// ɾ���ӳ�Ա
	bool del(Node * child);
	// ��������ӳ�Ա
	void clearAllChildren();

protected:
	std::vector<Node*> m_vChildren;

protected:
	void _exec();
	void _onDraw();
};

class Object
{
	friend FreePool;
public:
	Object();
	virtual ~Object();

	// �����������
	void retain();
	// �ͷ��������
	void release();
	// �������Զ��ͷ��������
	void autoRelease();

protected:
	int m_nRefCount;
	bool m_bAutoRelease;
};

class MouseMsg
{
	friend App;
public:
	MouseMsg();
	MouseMsg(TString name, const MOUSE_CALLBACK& callback);
	~MouseMsg();

	enum MESSAGE 
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

	// ��������
	void start();
	// ֹͣ����
	void stop();
	// ����ȴ�״̬
	void wait();
	// ����
	void notify();

	// ����Ƿ���
	static bool isLButtonDown();
	// �Ҽ��Ƿ���
	static bool isRButtonDown();
	// �м��Ƿ���
	static bool isMButtonDown();
	// ��ȡ���X����
	static int getX();
	// ��ȡ���Y����
	static int getY();
	// ��ȡ�������
	static CPoint getPos();
	// ��ȡ������ֵ
	static int getWheel();
	// ��ȡ��ǰ�����Ϣ
	static MESSAGE getMsg();

	// ��������Ϣ����
	static void addListener(TString name, const MOUSE_CALLBACK& callback);
	// ���������Ϣ����
	static void startListener(TString name);
	// ֹͣ�����Ϣ����
	static void stopListener(TString name);
	// ɾ�������Ϣ����
	static void delListener(TString name);
	// ɾ�����������Ϣ����
	static void clearAllListeners();
	// �������ڳ����ϵ����м�����
	static void notifyAllSceneListeners(Scene* scene);
	// ֹͣ���ڳ����ϵ����м�����
	static void waitAllSceneListeners(Scene* scene);
	// ������ڳ����ϵ����м�����
	static void clearAllSceneListeners(Scene* scene);

private:
	static void __exec();

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	TString			m_sName;
	MOUSE_CALLBACK	m_callback;
	Scene *			m_pParentScene;

protected:
	// ִ�лص�����
	void onMouseMsg();
};

class KeyMsg
{
	friend App;
public:
	KeyMsg(TString name, const KEY_CALLBACK& callback);
	~KeyMsg();

	// ִ�лص�����
	void onKbHit(VK_KEY key);
	// ��������
	void start();
	// ֹͣ����
	void stop();
	// ����ȴ�״̬
	void wait();
	// ����
	void notify();

	// �жϼ��Ƿ񱻰��£����·���true
	static bool isKeyDown(VK_KEY key);
	// ��Ӱ�������
	static void addListener(TString name, const KEY_CALLBACK& callback);
	// ������������
	static void startListener(TString name);
	// ֹͣ��������
	static void stopListener(TString name);
	// ɾ����������
	static void KeyMsg::delListener(TString name);
	// �������ڳ����ϵ����м�����
	static void notifyAllSceneListeners(Scene* scene);
	// ֹͣ���ڳ����ϵ����м�����
	static void waitAllSceneListeners(Scene* scene);
	// ֹͣ���ڳ����ϵ����ж�ʱ��
	static void clearAllSceneListeners(Scene* scene);
	// ɾ�����а�������
	static void clearAllListeners();

public:
	// ��ĸ��ֵ
	static const VK_KEY A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
	// ���ּ�ֵ
	static const VK_KEY NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0;
	// С���ּ���ֵ
	static const VK_KEY NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, NUMPAD_0;
	// ���Ƽ�ֵ
	static const VK_KEY Enter, Space, Up, Down, Left, Right, Esc, Shift, LShift, RShift, Ctrl, LCtrl, RCtrl;
	// F ��ֵ
	static const VK_KEY F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

private:
	static void __exec();

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	TString			m_sName;
	KEY_CALLBACK	m_callback;
	Scene *			m_pParentScene;
};

class FontStyle :
	public Object
{
	friend Text;
public:
	FontStyle();
	/**
	*  ʹ�� [�������ơ��ֺš���ϸ���ֿ�б�塢�»��ߡ�ɾ���ߡ��ַ�����д�Ƕȡ�
	*  ÿ���ַ���д�Ƕȡ������] ���Դ���������ʽ
	*/
	FontStyle(LPCTSTR fontfamily, LONG height = 18, LONG weight = 0, LONG width = 0,
		bool italic = 0, bool underline = 0, bool strikeout = 0, LONG escapement = 0,
		LONG orientation = 0, bool quality = true);
	virtual ~FontStyle();

	// ��ȡĬ������
	static FontStyle * getDefault();
	// �����ַ�ƽ���߶�
	void setHeight(LONG value);
	// �����ַ�ƽ����ȣ�0��ʾ����Ӧ��
	void setWidth(LONG value);
	// ��������
	void setFontFamily(LPCTSTR value);
	// �����ַ��ʻ���ϸ����Χ0~1000��Ĭ��Ϊ0
	void setWeight(LONG value);
	// ����б��
	void setItalic(bool value);
	// �����»���
	void setUnderline(bool value);
	// ����ɾ����
	void setStrikeOut(bool value);
	// �����ַ�������д�Ƕȣ���λ0.1�ȣ�Ĭ��Ϊ0
	void setEscapement(LONG value);
	// ����ÿ���ַ�����д�Ƕȣ���λ0.1�ȣ�Ĭ��Ϊ0
	void setOrientation(LONG value);
	// �������忹��ݣ�Ĭ��Ϊtrue
	void setQuality(bool value);

protected:
	LOGFONT m_font;
};

class FontWeight
{
public:
	static const LONG dontcare;		// ��ϸֵ 0
	static const LONG thin;			// ��ϸֵ 100
	static const LONG extraLight;	// ��ϸֵ 200
	static const LONG light;		// ��ϸֵ 300
	static const LONG normal;		// ��ϸֵ 400
	static const LONG regular;		// ��ϸֵ 400
	static const LONG medium;		// ��ϸֵ 500
	static const LONG demiBlod;		// ��ϸֵ 600
	static const LONG blod;			// ��ϸֵ 700
	static const LONG extraBold;	// ��ϸֵ 800
	static const LONG black;		// ��ϸֵ 900
	static const LONG heavy;		// ��ϸֵ 900
};

class Color
{
public:
	static const COLORREF black;		// ��ɫ
	static const COLORREF blue;			// ��ɫ
	static const COLORREF green;		// ��ɫ
	static const COLORREF cyan;			// ��ɫ
	static const COLORREF red;			// ��ɫ
	static const COLORREF magenta;		// ��ɫ
	static const COLORREF brown;		// ��ɫ
	static const COLORREF lightgray;	// ����ɫ
	static const COLORREF darkgray;		// ���ɫ
	static const COLORREF lightblue;	// ����ɫ
	static const COLORREF lightgreen;	// ����ɫ
	static const COLORREF lightcyan;	// ����ɫ
	static const COLORREF lightred;		// ����ɫ
	static const COLORREF lightmagenta;	// ����ɫ
	static const COLORREF yellow;		// ����ɫ
	static const COLORREF white;		// ��ɫ

	// ͨ���졢�̡�����ɫ�����ϳ���ɫ
	static COLORREF getFromRGB(BYTE r, BYTE g, BYTE b);
	// ͨ��ɫ�ࡢ���Ͷȡ����Ⱥϳ���ɫ
	static COLORREF getFromHSL(float H, float S, float L);
	// ͨ��ɫ�ࡢ���Ͷȡ����Ⱥϳ���ɫ
	static COLORREF getFromHSV(float H, float S, float V);

	// ����ָ����ɫ�еĺ�ɫֵ
	static BYTE getRValue(COLORREF color);
	// ����ָ����ɫ�е���ɫֵ
	static BYTE getGValue(COLORREF color);
	// ����ָ����ɫ�е���ɫֵ
	static BYTE getBValue(COLORREF color);
	// ������ָ����ɫ��Ӧ�ĻҶ�ֵ��ɫ
	static COLORREF getGray(COLORREF color);
};

class Node :
	public Object
{
	friend Scene;
	friend BatchNode;
public:
	Node();
	Node(CPoint p);
	Node(int x, int y);
	virtual ~Node();

	// ��ȡ�ڵ������
	virtual int getX() const;
	// ��ȡ�ڵ�������
	virtual int getY() const;
	// ��ȡ�ڵ�����
	virtual CPoint getPos() const;
	// ���ýڵ������
	virtual void setX(int x);
	// ���ýڵ�������
	virtual void setY(int y);
	// ���ýڵ��������
	virtual void setPos(int x, int y);
	// ���ýڵ��������
	virtual void setPos(CPoint p);
	// �ƶ��ڵ�
	virtual void move(int x, int y);
	// ���ýڵ��������
	virtual void move(CVector v);
	// �ڵ��Ƿ���ʾ
	virtual bool display() const;
	// ���ýڵ��Ƿ���ʾ
	virtual void setDisplay(bool value);
	// ��ȡ�ڵ��ͼ˳��
	virtual int getZOrder() const;
	// ���ýڵ��ͼ˳��0Ϊ���Ȼ��ƣ���ʾ����ײ㣩
	virtual void setZOrder(int z);
	// ��ȡ�ڵ����ڳ���
	Scene * getParentScene();

protected:
	int		m_nZOrder;
	bool	m_bDisplay;
	Scene*	m_pScene;
	CPoint	m_Pos;

protected:
	virtual bool _exec(bool active);
	virtual void _onDraw() = 0;
	void setParentScene(Scene * scene);
};

class BatchNode :
	public Node
{
public:
	BatchNode();
	virtual ~BatchNode();

	// ����ӽڵ�
	void add(Node *child, int z_Order = 0);
	// ɾ���ӽڵ�
	bool del(Node * child);
	// ��������ӽڵ�
	void clearAllChildren();

protected:
	std::vector<Node*> m_vChildren;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;
};

class Layer :
	public BatchNode
{
public:
	Layer();
	virtual ~Layer();

	// ͼ���Ƿ�������Ϣ
	int getBlock() const;
	// ����ͼ���Ƿ�������Ϣ
	void setBlock(bool block);

protected:
	bool m_bBlock;

protected:
	virtual bool _exec(bool active) override;
};

class RectNode :
	public Node
{
public:
	RectNode();
	~RectNode();

	virtual bool isCollisionWith(RectNode * rectNode) const;
	virtual bool isPointIn(CPoint p) const;

	virtual void setWindowCenterX();
	virtual void setWindowCenterY();
	virtual void setWindowCenter();

	virtual int getX() const override;
	virtual int getY() const override;
	virtual CPoint getPos() const override;
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual CSize getSize() const;
	virtual CRect getRect() const;

	virtual void setX(int x) override;
	virtual void setY(int y) override;
	virtual void setPos(int x, int y) override;
	virtual void setPos(CPoint p) override;
	virtual void move(int x, int y) override;
	virtual void move(CVector v) override;
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setSize(int width, int height);
	virtual void setSize(CSize size);
	virtual void setRect(int x1, int y1, int x2, int y2);
	virtual void setRect(CPoint leftTop, CPoint rightBottom);
	virtual void setRect(CRect rect);

protected:
	CRect m_Rect;
};

class Text :
	public RectNode
{
	friend TextButton;
public:
	Text();
	// �����ַ�������ɫ�����崴������
	Text(TString text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
	// ���ݺ������ꡢ�ַ�������ɫ�����崴������
	Text(int x, int y, TString text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
	virtual ~Text();

	// ��ȡ��ǰ��ɫ
	COLORREF getColor() const;
	// ��ȡ��ǰ����
	TString getText() const;
	// ��ȡ��ǰ����
	FontStyle * getFontStyle();
	// �ı��Ƿ�Ϊ��
	bool isEmpty() const;

	// ��������
	void setText(TString text);
	// ����������ɫ
	void setColor(COLORREF color);
	// ��������
	void setFontStyle(FontStyle * style);

protected:
	TString		m_sText;
	COLORREF	m_color;
	FontStyle *	m_pFontStyle;

protected:
	virtual void _onDraw() override;
};

class Image :
	public RectNode
{
	friend Sprite;
	friend ImageButton;
public:
	Image();
	// ��ͼƬ�ļ���ȡͼ��
	Image(LPCTSTR ImageFile);
	/**
	*  ��ͼƬ�ļ���ȡͼ��
	*  ������ͼƬ�ļ�����ͼƬ�ü����꣬ͼƬ�ü���Ⱥ͸߶�
	*/
	Image(LPCTSTR ImageFile, int x, int y, int width, int height);
	virtual ~Image();

	// ��ȡ�����������
	float getScaleX() const;
	// ��ȡ�����������
	float getScaleY() const;
	// ��ȡ͸����
	float getOpacity() const;

	/**
	*  ��ͼƬ�ļ���ȡͼ��
	*  ����ֵ��ͼƬ�����Ƿ�ɹ�
	*/
	bool setImage(LPCTSTR ImageFile);
	/**
	*  ��ͼƬ�ļ���ȡͼ��
	*  ������ͼƬ�ļ�����ͼƬ�ü���ʼ���꣬ͼƬ�ü���Ⱥ͸߶�
	*  ����ֵ��ͼƬ�����Ƿ�ɹ�
	*/
	bool setImage(LPCTSTR ImageFile, int x, int y, int width, int height);
	/**
	*  ����Դ�ļ���ȡͼ�񣬲�֧�� png
	*  ����ֵ��ͼƬ�����Ƿ�ɹ�
	*/
	bool setImageFromRes(LPCTSTR pResName);
	/**
	*  ����Դ�ļ���ȡͼ�񣬲�֧�� png
	*  ��������Դ���ƣ�ͼƬ�ü����꣬ͼƬ�ü���Ⱥ͸߶�
	*  ����ֵ��ͼƬ�����Ƿ�ɹ�
	*/
	bool setImageFromRes(LPCTSTR pResName, int x, int y, int width, int height);
	// �ü�ͼƬ���ü����ָ� stretch ���죩
	void crop(int x, int y, int width, int height);
	// ��ͼƬ���쵽�̶����
	void stretch(int width, int height);
	// ����������ͼƬ
	void setScale(float scaleX, float scaleY);
	// ����͸���ȣ���Χ 0~1.0f��ֻ�� png ͼƬ��Ч��
	void setOpacity(float value);
	// ����͸��ɫ
	void setTransparentColor(COLORREF value);
	// ����ͼƬ����
	void reset();

	// Ԥ����ͼƬ
	static bool preload(LPCTSTR fileName, bool fromRes = false);
	// ������Ϸ��ͼ
	static void saveScreenshot();

protected:
	CImage*	m_pCImage;
	CRect	m_SrcRect;
	BYTE	m_nAlpha;
	float	m_fScaleX;
	float	m_fScaleY;

protected:
	virtual void _onDraw() override;
};

class Sprite :
	public RectNode
{
	friend BatchSprite;
public:
	Sprite();
	Sprite(Image * image);
	Sprite(LPCTSTR imageFileName);
	virtual ~Sprite();

	// �ж��Ƿ����һ��������ײ
	bool isCollisionWith(Sprite * sprite);
	// �޸ľ���ͼƬ
	virtual void setImage(Image * image);
	// ��Ӷ���
	virtual void addAction(Action * action);
	// ��������
	virtual void resumeAction(Action * action);
	// ��ͣ����
	virtual void pauseAction(Action * action);
	// ֹͣ����
	virtual void stopAction(Action * action);
	// ��ͣ���ж���
	virtual void pauseAllActions();
	// �������ж���
	virtual void resumeAllActions();
	// ֹͣ���ж���
	virtual void stopAllActions();

	virtual float getScaleX() const;
	virtual float getScaleY() const;
	virtual float getOpacity() const;

	virtual void setScale(float scaleX, float scaleY);
	virtual void setOpacity(float opacity);

protected:
	float	m_fScaleX;
	float	m_fScaleY;
	BYTE	m_nAlpha;
	Image *	m_pImage;

protected:
	bool _exec(bool active) override;
	void _onDraw() override;
};

class BatchSprite :
	public Sprite
{
public:
	BatchSprite();
	virtual ~BatchSprite();

	// ��Ӿ���
	void addSprite(Sprite * sprite, int z_Order = 0);
	// ɾ������
	bool delSprite(Sprite * child);
	// ɾ�����о���
	void clearAllSprites();
	// �ж��Ƿ��о��������ײ
	// ����ֵ��������ײ�����ص�һ��������ײ�ľ��飬���򷵻ؿ�ָ��
	Sprite * isCollisionWith(Sprite * sprite);
	// �жϵ��Ƿ��ھ����ڲ�
	// ����ֵ���������������һ�������ڲ�������������飬���򷵻ؿ�ָ��
	Sprite * isPointIn(CPoint point);
	// ���о���ͬʱִ��һ�ζ���
	virtual void addAction(Action * action) override;
	// ͬʱ�޸����о����ͼƬ
	virtual void setImage(Image * image) override;

	virtual float getScaleX() const override;
	virtual float getScaleY() const override;
	virtual float getOpacity() const override;

	virtual void setScale(float scaleX, float scaleY) override;
	virtual void setOpacity(float opacity) override;

protected:
	std::vector<Sprite*> m_vSprites;

protected:
	bool _exec(bool active) override;
	void _onDraw() override;
};

class MouseNode :
	public RectNode
{
public:
	MouseNode();
	virtual ~MouseNode();

	// ����Ƿ�����
	virtual bool isMouseIn();
	// ����Ƿ�ѡ��
	virtual bool isSelected();
	// ���ûص�����
	virtual void setClickedCallback(const CLICK_CALLBACK & callback);
	// ���ûص�����
	virtual void setMouseInCallback(const CLICK_CALLBACK & callback);
	// ���ûص�����
	virtual void setMouseOutCallback(const CLICK_CALLBACK & callback);
	// ���ûص�����
	virtual void setSelectCallback(const CLICK_CALLBACK & callback);
	// ���ûص�����
	virtual void setUnselectCallback(const CLICK_CALLBACK & callback);
	// ����״̬
	virtual void reset();
	// ���ýڵ��Ƿ����������Ϣ
	void setBlock(bool block);

protected:
	bool m_bTarget;
	bool m_bBlock;
	enum Status { NORMAL, MOUSEIN, SELECTED } m_eStatus;
	CLICK_CALLBACK m_OnMouseInCallback;
	CLICK_CALLBACK m_OnMouseOutCallback;
	CLICK_CALLBACK m_OnSelectCallback;
	CLICK_CALLBACK m_OnUnselectCallback;
	CLICK_CALLBACK m_ClickCallback;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

	// ��д������������Զ��尴ť���жϷ���
	virtual bool _isMouseIn();
	// �л�״̬
	virtual void _setStatus(Status status);
	// ����״̬
	virtual void _onNormal() = 0;
	// �������ʱ
	virtual void _onMouseIn() = 0;
	// ���ѡ��ʱ
	virtual void _onSelected() = 0;
};

class Button :
	public MouseNode
{
public:
	Button();
	virtual ~Button();

	// ��ť�Ƿ�����
	virtual bool isEnable();
	// �����Ƿ�����
	virtual void setEnable(bool enable);

	// ���ð�ť������
	virtual void setX(int x) override;
	// ���ð�ť������
	virtual void setY(int y) override;
	// ���ð�ť����
	virtual void setPos(int x, int y) override;
	// ���ð�ť����
	virtual void setPos(CPoint p) override;
	// �ƶ���ť
	virtual void move(int x, int y) override;
	// �ƶ���ť
	virtual void move(CVector v) override;

protected:
	bool m_bEnable;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;
	virtual void _resetPosition() = 0;

	virtual void _onNormal() = 0;
	virtual void _onMouseIn() = 0;
	virtual void _onSelected() = 0;
	virtual void _onDisable() = 0;
};

class TextButton :
	public Button
{
public:
	TextButton();
	TextButton(TString text);
	TextButton(Text * text);
	virtual ~TextButton();

	// ���ð�ť����
	void setNormal(Text * text);
	// �����������ʱ�İ�ť����
	void setMouseIn(Text * text);
	// �������ѡ��ʱ�İ�ť����
	void setSelected(Text * text);
	// ���ð�ť����ʱ�İ�ť����
	void setUnable(Text * text);

protected:
	Text * m_pNormalText;
	Text * m_pMouseInText;
	Text * m_pSelectedText;
	Text * m_pUnableText;

protected:
	virtual void _resetPosition() override;

	virtual void _setStatus(Status status) override;
	virtual void _onNormal() override;
	virtual void _onMouseIn() override;
	virtual void _onSelected() override;
	virtual void _onDisable() override;
};

class ImageButton :
	public Button
{
public:
	ImageButton();
	ImageButton(LPCTSTR image);
	ImageButton(Image * image);
	virtual ~ImageButton();

	// ���ð�ťͼƬ
	void setNormal(Image * image);
	// �����������ʱ�İ�ťͼƬ
	void setMouseIn(Image * image);
	// �������ѡ��ʱ�İ�ťͼƬ
	void setSelected(Image * image);
	// ���ð�ť����ʱ�İ�ťͼƬ
	void setUnable(Image * image);

protected:
	Image * m_pNormalImage;
	Image * m_pMouseInImage;
	Image * m_pSelectedImage;
	Image * m_pUnableImage;

protected:
	virtual void _resetPosition() override;
	virtual void _setStatus(Status status) override;
	virtual void _onNormal() override;
	virtual void _onMouseIn() override;
	virtual void _onSelected() override;
	virtual void _onDisable() override;
};

class Shape :
	public Node
{
public:
	Shape();
	virtual ~Shape();

	// ��״�����ʽ
	enum STYLE { ROUND, SOLID, FILL } m_eStyle;

	// ��ȡ��״�������ɫ
	COLORREF getFillColor() const;
	// ��ȡ��״��������ɫ
	COLORREF getLineColor() const;
	// ���������ɫ
	void setFillColor(COLORREF color);
	// ����������ɫ
	void setLineColor(COLORREF color);
	// ���������ʽ
	void setStyle(STYLE style);

protected:
	COLORREF fillColor;
	COLORREF lineColor;

protected:
	virtual void _onDraw() override;
	virtual void solidShape() = 0;
	virtual void fillShape() = 0;
	virtual void roundShape() = 0;
};

class Rect :
	public Shape
{
public:
	Rect();
	Rect(int x, int y, int width, int height);
	virtual ~Rect();

	// ��ȡ���ο��
	int getWidth() const;
	// ��ȡ���θ߶�
	int getHeight() const;
	// ���þ��ο��
	void setWidth(int width);
	// ���þ��θ߶�
	void setHeight(int height);
	// ���þ��δ�С
	void setSize(int width, int height);

protected:
	CSize m_Size;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;
};

class Circle :
	public Shape
{
public:
	Circle();
	Circle(int x, int y, int radius);
	virtual ~Circle();

	// ��ȡԲ�ΰ뾶
	int getRadius() const;
	// ����Բ�ΰ뾶
	void setRadius(int m_nRadius);

protected:
	int m_nRadius;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;
};

class Action :
	public Object
{
	friend Sprite;
	friend ActionManager;
	friend ActionTwo;
	friend ActionNeverStop;
	friend ActionSequence;
public:
	Action();
	virtual ~Action();

	// ��ȡ��������״̬
	virtual bool isRunning();
	// ��ȡ��������״̬
	virtual bool isEnding();
	// ��������
	virtual void start();
	// ��������
	virtual void resume();
	// ��ͣ����
	virtual void pause();
	// ֹͣ����
	virtual void stop();
	// ����ȴ�״̬
	virtual void wait();
	// ����
	virtual void notify();
	// ���ö���ÿһ֡ʱ����
	virtual void setInterval(UINT ms);
	// ��ȡһ���µĿ�������
	virtual Action * copy() const = 0;
	// ��ȡһ���µ�������
	virtual Action * reverse() const;
	// ��ȡִ�иö�����Ŀ��
	virtual Sprite * getTarget();

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	bool			m_bEnding;
	bool			m_bInit;
	Sprite *		m_pTargetSprite;
	Scene *			m_pParentScene;
	UINT			m_nMilliSeconds;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;

protected:
	virtual void _init();
	virtual void _exec(LARGE_INTEGER nNow) = 0;
	virtual void _reset();
};

class Animation :
	public Action
{
public:
	Animation(float duration);
	virtual ~Animation();

protected:
	UINT	m_nDuration;
	UINT	m_nTotalDuration;

protected:
	bool _isEnd() const;
	bool _isDelayEnough(LARGE_INTEGER nNow);
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionMoveBy :
	public Animation
{
public:
	ActionMoveBy(float duration, CVector vec);
	virtual ~ActionMoveBy();

	virtual ActionMoveBy * copy() const override;
	virtual ActionMoveBy * reverse() const override;

protected:
	CPoint	m_BeginPos;
	CVector	m_MoveVector;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionMoveTo :
	public ActionMoveBy
{
public:
	ActionMoveTo(float duration, CPoint pos);
	virtual ~ActionMoveTo();

	virtual ActionMoveTo * copy() const override;

protected:
	CPoint m_EndPos;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionScaleBy :
	public Animation
{
public:
	ActionScaleBy(float duration, float scaleX, float scaleY);
	virtual ~ActionScaleBy();

	virtual ActionScaleBy * copy() const override;
	virtual ActionScaleBy * reverse() const override;

protected:
	float	m_nBeginScaleX;
	float	m_nBeginScaleY;
	float	m_nVariationX;
	float	m_nVariationY;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionScaleTo :
	public ActionScaleBy
{
public:
	ActionScaleTo(float duration, float scaleX, float scaleY);
	virtual ~ActionScaleTo();

	virtual ActionScaleTo * copy() const override;

protected:
	float	m_nEndScaleX;
	float	m_nEndScaleY;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionOpacityBy :
	public Animation
{
public:
	ActionOpacityBy(float duration, float opacity);
	virtual ~ActionOpacityBy();

	virtual ActionOpacityBy * copy() const override;
	virtual ActionOpacityBy * reverse() const override;

protected:
	float m_nBeginVal;
	float m_nVariation;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionOpacityTo :
	public ActionOpacityBy
{
public:
	ActionOpacityTo(float duration, float opacity);
	virtual ~ActionOpacityTo();

	virtual ActionOpacityTo * copy() const override;

protected:
	float m_nEndVal;

protected:
	virtual void _init() override;
	virtual void _reset() override;
};

class ActionFadeIn :
	public ActionOpacityTo
{
public:
	ActionFadeIn(float duration) : ActionOpacityTo(duration, 1) {}
};

class ActionFadeOut :
	public ActionOpacityTo
{
public:
	ActionFadeOut(float duration) : ActionOpacityTo(duration, 0) {}
};

class ActionTwo :
	public Action
{
public:
	ActionTwo(Action * actionFirst, Action * actionSecond);
	virtual ~ActionTwo();

	virtual ActionTwo * copy() const override;
	virtual ActionTwo * reverse(bool actionReverse = true) const;

protected:
	Action *	m_FirstAction;
	Action *	m_SecondAction;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionSequence :
	public Action
{
public:
	ActionSequence();
	ActionSequence(int number, Action * action1, ...);
	virtual ~ActionSequence();

	void addAction(Action * action);
	virtual ActionSequence * copy() const override;
	virtual ActionSequence * reverse(bool actionReverse = true) const;

protected:
	UINT					m_nActionIndex;
	std::vector<Action*>	m_vActions;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionDelay :
	public Action
{
public:
	ActionDelay(float duration);
	virtual ~ActionDelay();

	virtual ActionDelay * copy() const override;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionNeverStop :
	public Action
{
public:
	ActionNeverStop(Action * action);
	virtual ~ActionNeverStop();

	virtual ActionNeverStop * copy() const override;

protected:
	Action * m_Action;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionFrames :
	public Action
{
public:
	ActionFrames();
	ActionFrames(UINT frameDelay);
	~ActionFrames();

	void addFrame(Image * frame);
	virtual ActionFrames * copy() const override;
	virtual ActionFrames * reverse() const override;

protected:
	UINT				m_nFrameIndex;
	std::vector<Image*> m_vFrames;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class ActionCallback :
	public Action
{
public:
	ActionCallback(const std::function<void()>& callback);
	~ActionCallback();

	virtual ActionCallback * copy() const override;

protected:
	std::function<void()> m_Callback;

protected:
	virtual void _init() override;
	virtual void _exec(LARGE_INTEGER nNow) override;
	virtual void _reset() override;
};

class FileUtils
{
public:
	// ��ȡϵͳ�� AppData\Local ·��
	static TString getLocalAppDataPath();
	// ��ȡĬ�ϵı���·��
	static TString getDefaultSavePath();
	// ���� int �͵�ֵ
	static void saveInt(LPCTSTR key, int value);
	// ���� double �͵�ֵ
	static void saveDouble(LPCTSTR key, double value);
	// ���� �ַ��� �͵�ֵ����Ҫ�� Unicode �ַ����±��������ַ���
	static void saveString(LPCTSTR key, TString value);
	// ��ȡ int �͵�ֵ�����������򷵻� default ������ֵ��
	static int getInt(LPCTSTR key, int default);
	// ��ȡ double �͵�ֵ�����������򷵻� default ������ֵ��
	static double getDouble(LPCTSTR key, double default);
	// ��ȡ �ַ��� �͵�ֵ�����������򷵻� default ������ֵ��
	static TString geTString(LPCTSTR key, TString default);
	// �õ��ļ���չ����Сд��
	static TString getFileExtension(const TString& filePath);
	/**
	*  �򿪱����ļ��Ի��򣬵õ���Ч����·������ true
	*  �����������ļ�·�����ַ��������ڱ��⣬������չ�����ˣ�����Ĭ����չ��
	*/
	static bool getSaveFilePath(TString& path, LPCTSTR title = _T("���浽"), LPCTSTR defExt = NULL);
};

class MusicUtils
{
public:
	// ���ű�������
	static void playBackgroundMusic(TString pszFilePath, bool bLoop = true);
	// ֹͣ��������
	static void stopBackgroundMusic(bool bReleaseData = false);
	// ��ͣ��������
	static void pauseBackgroundMusic();
	// �������ű�������
	static void resumeBackgroundMusic();
	// ��ͷ���ű�������
	static void rewindBackgroundMusic();
	// ���������Ƿ����ڲ���
	static bool isBackgroundMusicPlaying();
	// ���ñ�������������0 ~ 1.0f
	static void setBackgroundMusicVolume(float volume);

	// ������Ч
	static unsigned int playMusic(TString pszFilePath, bool loop = false);
	// ֹͣ��Ч
	static void stopMusic(unsigned int nSoundId);
	// Ԥ������Ч
	static void preloadMusic(TString pszFilePath);
	// ��ͣ��Ч
	static void pauseMusic(unsigned int nSoundId);
	// ����������Ч
	static void resumeMusic(unsigned int nSoundId);
	// ж����Ч
	static void unloadMusic(LPCTSTR pszFilePath);
	// �����ض����ֵ�������0 ~ 1.0f
	static void setVolume(TString pszFilePath, float volume);

	// ��ͣ��������
	static void pauseAllMusics();
	// ����������������
	static void resumeAllMusics();
	// ֹͣ��������
	static void stopAllMusics();
	// ֹͣ�������֣����ͷ��ڴ�
	static void end();
	// ������������0 ~ 1.0f
	static void setVolume(float volume);
};

class Timer
{
	friend App;
public:
	Timer(TString name, UINT ms, const TIMER_CALLBACK & callback);
	~Timer();

	// ������ʱ��
	void start();
	// ֹͣ��ʱ��
	void stop();
	// ����ȴ�״̬
	void wait();
	// ����
	void notify();
	// ��ʱ���Ƿ���������
	bool isRunning();
	// ���ü��ʱ��
	void setInterval(UINT ms);
	// ���ûص�����
	void setCallback(const TIMER_CALLBACK& callback);
	// ���ö�ʱ������
	void setName(TString name);
	// ��ȡ��ʱ�����ʱ��
	UINT getInterval() const;
	// ��ȡ��ʱ������
	TString getName() const;

	// ��Ӷ�ʱ��
	static void addTimer(Timer * timer);
	// ��Ӷ�ʱ��
	static void addTimer(TString name, UINT ms, const TIMER_CALLBACK & callback);
	// �����ض���ʱ��
	static void startTimer(TString name);
	// ֹͣ�ض���ʱ��
	static void stopTimer(TString name);
	// ɾ���ض���ʱ��
	static void delTimer(TString name);
	// ɾ�����ж�ʱ��
	static void clearAllTimers();

	// �������ڳ����ϵ����ж�ʱ��
	static void notifyAllSceneTimers(Scene* scene);
	// ֹͣ���ڳ����ϵ����ж�ʱ��
	static void waitAllSceneTimers(Scene* scene);
	// ������ڳ����ϵ����ж�ʱ��
	static void clearAllSceneTimers(Scene* scene);

protected:
	bool			m_bRunning;
	bool			m_bWaiting;
	TString			m_sName;
	TIMER_CALLBACK	m_callback;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;
	UINT			m_nMilliSeconds;
	Scene *			m_pParentScene;

private:
	static void __exec();
};

class ActionManager
{
	friend App;
	friend Sprite;
public:
	// ����һ���ض��Ķ���
	static void startAction(Action * action);
	// ����һ���ض��Ķ���
	static void resumeAction(Action * action);
	// ��ͣһ���ض��Ķ���
	static void pauseAction(Action * action);
	// ֹͣһ���ض��Ķ���
	static void stopAction(Action * action);

	// ����һ�� Sprite �����ж���
	static void startSpriteAllActions(Sprite * sprite);
	// ����һ�� Sprite �����ж���
	static void resumeSpriteAllActions(Sprite * sprite);
	// ��ͣһ�� Sprite �����ж���
	static void pauseSpriteAllActions(Sprite * sprite);
	// ֹͣһ�� Sprite �����ж���
	static void stopSpriteAllActions(Sprite * sprite);

	// ������ǰ���ڵ����ж���
	static void startAllActions();
	// ������ǰ���ڵ����ж���
	static void resumeAllActions();
	// ��ͣ��ǰ���ڵ����ж���
	static void pauseAllActions();
	// ֹͣ��ǰ���ڵ����ж���
	static void stopAllActions();
	// ɾ����ǰ���ڵ����ж���
	static void clearAllActions();

	// �������ڳ����ϵĶ���
	static void notifyAllSceneActions(Scene* scene);
	// ��ͣ���ڳ����ϵĶ���
	static void waitAllSceneActions(Scene* scene);
	// ֹͣ���ڳ����ϵĶ���
	static void stopAllSceneActions(Scene* scene);

private:
	static void __exec();
	// ��һ��������ӽ�����������
	static void addAction(Action * action);
};

}	// End of easy2d namespace


// Functions Declare

inline void SafeRelease(easy2d::Object * p) { if (p) p->release(); }
inline void SafeDelete(void * p) { if (p) delete p; }

using namespace easy2d;