/******************************************************
* Easy2D Game Engine (v1.0.4)
* http://www.easy2d.cn
* 
* Depends on EasyX (Ver:20170827(beta))
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


// String macros

#ifdef UNICODE
	#define tstring std::wstring
#else
	#define tstring std::string
#endif


// Safe macros

#define SAFE_DELETE(p)			{ delete (p); (p) = nullptr; }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) p->release(); }

#pragma warning (disable: 4099)

#include <windows.h>
#include <tchar.h>
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


// Class Declare

namespace easy2d {

// ������
class App;
class Scene;
class KeyMsg;
class MouseMsg;
class FreePool;
// ��ʽ��
class Color;
class FontStyle;
// ������
class Timer;
class MusicUtils;
class FileUtils;
// ����
class Object;
class Node;
class Layer;
class BatchNode;
class MouseNode;
class Image;
class Text;
class Shape;
class Rect;
class Circle;
class Button;
class TextButton;
class ImageButton;


typedef unsigned int				VK_KEY;
typedef std::function<void()>		CLICK_CALLBACK;
typedef std::function<void()>		TIMER_CALLBACK;
typedef std::function<void(VK_KEY)>	KEY_CALLBACK;
typedef std::function<void()>		MOUSE_CALLBACK;


class App
{
public:
	App();
	~App();

	// ���ڿ�ѡģʽ
	enum { SHOW_CONSOLE = 1, NO_CLOSE = 2, NO_MINI_MIZE = 4 };

	// ��ȡ����ʵ��
	static App * get();
	// ��������ԭ��
	static void setOrigin(int originX, int originY);
	// ��ȡ����ԭ������������
	static int getOriginX();
	// ��ȡ����ԭ�������������
	static int getOriginY();
	// ��������
	int run();
	// ��ֹ����
	void quit();
	// ��ֹ����
	void end();
	// �����ͼ����
	void createWindow(int width, int height, int mode = 0);
	// �����ͼ����
	void createWindow(tstring title, int width, int height, int mode = 0);
	// �رմ���
	void close();
	// �޸Ĵ��ڴ�С
	void setWindowSize(int width, int height);
	// ���ô��ڱ���
	void setWindowTitle(tstring title);
	// ��ȡ���ڱ���
	tstring getWindowTitle();
	// ��ȡ���ڿ��
	int getWidth() const;
	// ��ȡ���ڸ߶�
	int getHeight() const;
	// �л�����
	void enterScene(Scene *scene, bool save = true);
	// ������һ����
	void backScene();
	// ���֮ǰ��������г���
	void clearScene();
	// ���� AppName
	void setAppName(tstring appname);
	// ��ȡ AppName
	tstring getAppName() const;
	// �޸Ĵ��ڱ���ɫ
	void setBkColor(COLORREF color);
	// ����֡��
	void setFPS(DWORD fps);
	// ���û�ͼ��ʽΪĬ��ֵ
	void reset();
	// �ͷ������ڴ���Դ
	void free();
	// ���ٸö���
	void destory();
	// ��ȡ��ǰ����
	Scene * getCurrentScene();
	// ��ȡ Easy2D �汾��
	LPCTSTR getVersion();

protected:
	tstring				m_sTitle;
	tstring				m_sAppName;
	Scene*				m_currentScene;
	Scene*				m_nextScene;
	std::stack<Scene*>	m_sceneStack;
	LARGE_INTEGER		m_nAnimationInterval;
	int					m_nWidth;
	int					m_nHeight;
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
	friend class App;
	friend class Object;

private:
	// ˢ���ڴ��
	static void __flush();
	// ��һ���ڵ�����ͷų�
	static void __add(Object * nptr);
};

class Scene
{
	friend class App;
	friend class MouseMsg;

public:
	Scene();
	~Scene();

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


class MouseMsg
{
	friend class App;

public:
	MouseMsg();
	MouseMsg(tstring name, const MOUSE_CALLBACK& callback);
	~MouseMsg();

	// ��Ӽ��̼���
	static void addListener(tstring name, const MOUSE_CALLBACK& callback);
	// ɾ�����̼���
	static bool delListener(tstring name);
	// ɾ�����м��̼���
	static void clearAllListeners();
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
	// ��ȡ������ֵ
	static int getWheel();
	// ����ƶ���Ϣ
	static bool isOnMouseMoved();
	// ���˫����Ϣ
	static bool isOnLButtonDBClicked();
	// �Ҽ�������Ϣ
	static bool isOnLButtonDown();
	// ���������Ϣ
	static bool isOnLButtonUp();
	// �Ҽ�˫����Ϣ
	static bool isOnRButtonDBClicked();
	// �Ҽ�������Ϣ
	static bool isOnRButtonDown();
	// �Ҽ�������Ϣ
	static bool isOnRButtonUp();
	// �м�˫����Ϣ
	static bool isOnMButtonDBClicked();
	// �м�������Ϣ
	static bool isOnMButtonDown();
	// �м�������Ϣ
	static bool isOnMButtonUp();
	// �����ֲ�����Ϣ
	static bool isOnWheel();
	// ��������Ϣ
	static void resetMouseMsg();

private:
	static void __exec();

protected:
	tstring m_sName;
	MOUSE_CALLBACK m_callback;

protected:
	// ִ�лص�����
	void onMouseMsg();
};


class KeyMsg
{
	friend class App;

public:
	KeyMsg(tstring name, const KEY_CALLBACK& callback);
	~KeyMsg();

	// ִ�лص�����
	void onKbHit(VK_KEY key);

	// ��Ӽ��̼���
	static void addListener(tstring name, const KEY_CALLBACK& callback);
	// ɾ�����̼���
	static bool delListener(tstring name);
	// ɾ�����м��̼���
	static void clearAllListeners();
	// �жϼ��Ƿ񱻰��£����·���true
	static bool isKeyDown(VK_KEY key);

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
	tstring m_sName;
	KEY_CALLBACK m_callback;
};


class FileUtils
{
public:
	// ��ȡϵͳ�� AppData\Local ·��
	static tstring getLocalAppDataPath();
	// ��ȡĬ�ϵı���·��
	static tstring getDefaultSavePath();
	// ���� int �͵�ֵ
	static void saveInt(LPCTSTR key, int value);
	// ���� double �͵�ֵ
	static void saveDouble(LPCTSTR key, double value);
	// ���� �ַ��� �͵�ֵ����Ҫ�� Unicode �ַ����±��������ַ���
	static void saveString(LPCTSTR key, tstring value);
	// ��ȡ int �͵�ֵ�����������򷵻� default ������ֵ��
	static int getInt(LPCTSTR key, int default);
	// ��ȡ double �͵�ֵ�����������򷵻� default ������ֵ��
	static double getDouble(LPCTSTR key, double default);
	// ��ȡ �ַ��� �͵�ֵ�����������򷵻� default ������ֵ��
	static tstring getString(LPCTSTR key, tstring default);
	// �õ��ļ���չ����Сд��
	static tstring getFileExtension(const tstring& filePath);
	/**
	*  �򿪱����ļ��Ի��򣬵õ���Ч����·������ true
	*  �����������ļ�·�����ַ��������ڱ��⣬������չ�����ˣ�����Ĭ����չ��
	*/
	static bool getSaveFilePath(tstring& path, LPCTSTR title = _T("���浽"), LPCTSTR defExt = NULL);
};


class MusicUtils
{
public:
	// ���ű�������
	static void playBackgroundMusic(tstring pszFilePath, bool bLoop = true);
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
	static unsigned int playMusic(tstring pszFilePath, bool loop = false);
	// ֹͣ��Ч
	static void stopMusic(unsigned int nSoundId);
	// Ԥ������Ч
	static void preloadMusic(tstring pszFilePath);
	// ��ͣ��Ч
	static void pauseMusic(unsigned int nSoundId);
	// ����������Ч
	static void resumeMusic(unsigned int nSoundId);
	// ж����Ч
	static void unloadMusic(LPCTSTR pszFilePath);
	// �����ض����ֵ�������0 ~ 1.0f
	static void setVolume(tstring pszFilePath, float volume);

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
	friend class App;

public:
	Timer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
	~Timer();

	// ������ʱ��
	void start();
	// ֹͣ��ʱ��
	void stop();
	// ��ʱ���Ƿ���������
	bool isRunning();
	// ���ü��ʱ��
	void setInterval(UINT ms);
	// ���ûص�����
	void setCallback(const TIMER_CALLBACK& callback);
	// ���ö�ʱ������
	void setName(tstring name);
	// ��ȡ��ʱ�����ʱ��
	UINT getInterval() const;
	// ��ȡ��ʱ������
	tstring getName() const;

	// ��Ӷ�ʱ��
	static void addTimer(Timer * timer);
	// ��Ӷ�ʱ��
	static void addTimer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
	// �������ƻ�ȡ��ʱ��
	static Timer * getTimer(tstring name);
	// �����ض���ʱ��
	static bool startTimer(tstring name);
	// ֹͣ�ض���ʱ��
	static bool stopTimer(tstring name);
	// ɾ���ض���ʱ��
	static bool delTimer(tstring name);
	// ɾ�����ж�ʱ��
	static void clearAllTimers();

protected:
	bool			m_bRunning;
	tstring			m_sName;
	TIMER_CALLBACK	m_callback;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;
	UINT			m_nMilliSeconds;

private:
	static void __exec();
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


class Object
{
	friend class FreePool;

public:
	Object();
	virtual ~Object();

	void retain();
	void release();

protected:
	int m_nRef;
};


class FontStyle :
	public virtual Object
{
	friend class Text;

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


class Node :
	public virtual Object
{
	friend class Scene;
	friend class BatchNode;

public:
	Node();
	Node(int x, int y);
	virtual ~Node();

	// ��ȡ�ڵ������
	const int getX() const;
	// ��ȡ�ڵ�������
	const int getY() const;
	// ���ýڵ������
	virtual void setX(int x);
	// ���ýڵ�������
	virtual void setY(int y);
	// ���ýڵ��������
	virtual void setPos(int x, int y);
	// �ƶ��ڵ�
	virtual void move(int x, int y);
	// �ڵ��Ƿ���ʾ
	bool display() const;
	// ���ýڵ��Ƿ���ʾ
	void setDisplay(bool value);
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
	int		m_nX;
	int		m_nY;

protected:
	virtual bool _exec(bool active);
	virtual void _onDraw() = 0;
	void setParentScene(Scene * scene);
};


class BatchNode :
	public virtual Node
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
	public virtual BatchNode
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


class Image :
	public virtual Node
{
	friend class ImageButton;

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

	// ��ȡͼ����
	int getWidth() const;
	// ��ȡͼ��߶�
	int getHeight() const;
	// ��ȡ�����������
	float getScaleX() const;
	// ��ȡ�����������
	float getScaleY() const;

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
	void scale(float scaleX = 1.0f, float scaleY = 1.0f);
	// ����ͼƬλ��
	void setPos(int x, int y) override;
	// �ƶ�ͼƬ
	void move(int x, int y) override;
	// ����ͼƬ������
	void setX(int x) override;
	// ����ͼƬ������
	void setY(int y) override;
	// ����͸��ɫ
	void setTransparentColor(COLORREF value);
	// ������Ϸ��ͼ
	static void saveScreenshot();

protected:
	CImage m_Image;
	CRect m_rDest;
	CRect m_rSrc;
	float m_fScaleX;
	float m_fScaleY;

protected:
	virtual void _onDraw() override;
};


class Text :
	public virtual Node
{
	friend class TextButton;

public:
	Text();
	// �����ַ�������ɫ�����崴������
	Text(tstring text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
	// ���ݺ������ꡢ�ַ�������ɫ�����崴������
	Text(int x, int y, tstring text, COLORREF color = Color::white, FontStyle * font = FontStyle::getDefault());
	virtual ~Text();

	// ��ȡ��ǰ��ɫ
	COLORREF getColor() const;
	// ��ȡ��ǰ����
	tstring getText() const;
	// ��ȡ��ǰ����
	FontStyle * getFontStyle();
	// ��ȡ���ֿ��
	int getWidth();
	// ��ȡ���ָ߶�
	int getHeight();
	// �ı��Ƿ�Ϊ��
	bool isEmpty() const;

	// ��������
	void setText(tstring text);
	// ����������ɫ
	void setColor(COLORREF color);
	// ��������
	void setFontStyle(FontStyle * style);

protected:
	tstring		m_sText;
	COLORREF	m_color;
	FontStyle *	m_pFontStyle;

protected:
	virtual void _onDraw() override;
};


class MouseNode :
	public virtual Node
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

private:
	bool m_bTarget;
	bool m_bBlock;
	enum Status { NORMAL, MOUSEIN, SELECTED } m_eStatus;
	CLICK_CALLBACK m_OnMouseInCallback;
	CLICK_CALLBACK m_OnMouseOutCallback;
	CLICK_CALLBACK m_OnSelectCallback;
	CLICK_CALLBACK m_OnUnselectCallback;
	CLICK_CALLBACK m_ClickCallback;

protected:
	int m_nWidth;
	int m_nHeight;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

	// ��д������������Զ��尴ť���жϷ���
	virtual bool _judge();
	// �л�״̬
	void _setStatus(Status status);
	// ����״̬
	virtual void _onNormal() = 0;
	// �������ʱ
	virtual void _onMouseIn() = 0;
	// ���ѡ��ʱ
	virtual void _onSelected() = 0;
};


class Button :
	public virtual MouseNode
{
public:
	Button();
	virtual ~Button();

	// ��ť�Ƿ�����
	virtual bool isEnable();
	// �����Ƿ�����
	virtual void setEnable(bool enable);

protected:
	bool m_bEnable;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

	virtual void _onNormal() = 0;
	virtual void _onMouseIn() = 0;
	virtual void _onSelected() = 0;
	virtual void _onDisable() = 0;
};



class TextButton :
	public virtual Button
{
public:
	TextButton();
	TextButton(tstring text);
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

	// ���ð�ť������
	virtual void setX(int x) override;
	// ���ð�ť������
	virtual void setY(int y) override;
	// ���ð�ť��������
	virtual void setPos(int x, int y) override;

protected:
	Text * m_pNormalText;
	Text * m_pMouseInText;
	Text * m_pSelectedText;
	Text * m_pUnableText;

protected:
	// ��������λ�ã�������ʾ��
	void resetTextPosition();

	virtual void _onNormal() override;
	virtual void _onMouseIn() override;
	virtual void _onSelected() override;
	virtual void _onDisable() override;
};



class ImageButton :
	public virtual Button
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

	// ���ð�ť������
	virtual void setX(int x) override;
	// ���ð�ť������
	virtual void setY(int y) override;
	// ���ð�ť��������
	virtual void setPos(int x, int y) override;

protected:
	Image * m_pNormalImage;
	Image * m_pMouseInImage;
	Image * m_pSelectedImage;
	Image * m_pUnableImage;

protected:
	// ����ͼƬλ�ã�������ʾ��
	void resetImagePosition();

	virtual void _onNormal() override;
	virtual void _onMouseIn() override;
	virtual void _onSelected() override;
	virtual void _onDisable() override;
};


class Shape :
	public virtual Node
{
public:
	Shape();
	virtual ~Shape();

	enum STYLE { ROUND, SOLID, FILL } m_eStyle;	// ��״�����ʽ

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
	COLORREF	fillColor;
	COLORREF	lineColor;

protected:
	virtual void _onDraw() override;
	virtual void solidShape() = 0;
	virtual void fillShape() = 0;
	virtual void roundShape() = 0;
};


class Rect :
	public virtual Shape
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
	int m_nWidth;
	int m_nHeight;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;
};


class Circle :
	public virtual Shape
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

}	// End of easy2d namespace


// Ĭ��ʹ�� easy2d �����ռ�
using namespace easy2d;