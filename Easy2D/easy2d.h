/******************************************************
* Easy2D Game Engine (v1.0.0)
* http://www.easy2d.cn
* 
* Depends on EasyX (Ver:20170827(beta))
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
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
class Application;
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


class Application
{
protected:
	tstring				m_sTitle;
	Scene*				m_currentScene;
	Scene*				m_nextScene;
	std::stack<Scene*>	m_sceneStack;
	LARGE_INTEGER		m_nAnimationInterval;
	int					m_nWidth;
	int					m_nHeight;
	int					m_nWindowMode;
	bool				m_bRunning;
	bool				m_bPause;
	bool				m_bSaveScene;

protected:
	void _initGraph();
	void _mainLoop();
	void _enterNextScene();

public:
	Application();
	~Application();

	// ���ڿ�ѡģʽ
	enum { SHOW_CONSOLE = 1, NO_CLOSE = 2, NO_MINI_MIZE = 4 };
	// ��ȡ����ʵ��
	static Application * get();
	// ��������ԭ��
	static void setOrigin(int originX, int originY);
	// ��ȡ����ԭ������������
	static int getOriginX();
	// ��ȡ����ԭ�������������
	static int getOriginY();
	// ��������
	int run();
	// ��ͣ����
	void pause();
	// ��ֹ����
	void quit();
	// ��ֹ����
	void end();
	// �����ͼ����
	void createWindow(int width, int height, int mode = 0);
	// �����ͼ����
	void createWindow(tstring title, int width, int height, int mode = 0);
	// �޸Ĵ��ڴ�С
	void setWindowSize(int width, int height);
	// ���ô��ڱ���
	void setWindowText(tstring title);
	// �رմ���
	void close();
	// ��ȡ���ڿ��
	int getWidth() const;
	// ��ȡ���ڸ߶�
	int getHeight() const;
	// �л�����
	void enterScene(Scene *scene, bool save = true);
	// ������һ����
	void backScene();
	// ��Ϸ�Ƿ���������
	bool isRunning();
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
};

class FreePool
{
	friend class Application;

private:
	static void __flush();

public:
	// ��һ���ڵ�����ͷų�
	static void add(Object * nptr);
};

class Scene
{
	friend class Application;
	friend class MouseMsg;

protected:
	std::vector<Node*> m_vChildren;

protected:
	void _exec();
	void _onDraw();

public:
	Scene();
	~Scene();

	// ����ӳ�Ա������
	void add(Node * child, int zOrder = 0);
	// ɾ���ӳ�Ա
	bool del(Node * child);
	// ��������ӳ�Ա
	void clearAllChildren();
};


class MouseMsg
{
	friend class Application;

private:
	static void __exec();

public:
	UINT uMsg;			// ��ǰ�����Ϣ
	bool mkLButton;		// �������Ƿ���
	bool mkMButton;		// ����м��Ƿ���
	bool mkRButton;		// ����Ҽ��Ƿ���
	short x;			// ��ǰ��� x ����
	short y;			// ��ǰ��� y ����
	short wheel;		// �����ֹ���ֵ (120 �ı���)

public:
	// ��ȡ��ǰ�����Ϣ
	static MouseMsg getMsg();
	// ����Ƿ���
	static bool getLButtonDown();
	// �Ҽ��Ƿ���
	static bool getRButtonDown();
	// �м��Ƿ���
	static bool getMButtonDown();
	// ��ȡ���X����
	static int getMouseX();
	// ��ȡ���Y����
	static int getMouseY();
	// ��ȡ������ֵ
	static int getMouseWheel();
	// ����ƶ���Ϣ
	static bool getMouseMovedMsg();
	// ���˫����Ϣ
	static bool getLButtonDBClickedMsg();
	// �Ҽ�������Ϣ
	static bool getLButtonDownMsg();
	// ���������Ϣ
	static bool getLButtonUpMsg();
	// �Ҽ�˫����Ϣ
	static bool getRButtonDBClicked();
	// �Ҽ�������Ϣ
	static bool getRButtonDownMsg();
	// �Ҽ�������Ϣ
	static bool getRButtonUpMsg();
	// �м�˫����Ϣ
	static bool getMButtonDBClicked();
	// �м�������Ϣ
	static bool getMButtonDownMsg();
	// �м�������Ϣ
	static bool getMButtonUpMsg();
	// �����ֲ�����Ϣ
	static bool getWheelMsg();
	// ��������Ϣ
	static void resetMouseMsg();
};


class KeyMsg
{
	friend class Application;

public:
	// ��ĸ��ֵ
	static const VK_KEY A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
	// ���ּ�ֵ
	static const VK_KEY NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0;
	// С���ּ���ֵ
	static const VK_KEY NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, NUMPAD_0, Decimal;
	// ���Ƽ�ֵ
	static const VK_KEY Enter, Space, Up, Down, Left, Right, Esc, Shift, LShift, RShift, Ctrl, LCtrl, RCtrl;
	// F ��ֵ
	static const VK_KEY F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

private:
	static void __exec();

protected:
	tstring m_sName;
	KEY_CALLBACK m_callback;

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
	static void clearAllListener();
	// �жϼ��Ƿ񱻰��£����·���true
	static bool isKeyDown(VK_KEY key);
};


class FileUtils
{
public:
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
	static void playBackgroundMusic(tstring pszFilePath, bool bLoop = false);
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

	// ��ͣ��������
	static void pauseAllMusics();
	// ����������������
	static void resumeAllMusics();
	// ֹͣ��������
	static void stopAllMusics();
	// ֹͣ�������֣����ͷ��ڴ�
	static void end();
};


class Timer
{
	friend class Application;

protected:
	bool			m_bRunning;
	tstring			m_sName;
	TIMER_CALLBACK	m_callback;
	LARGE_INTEGER	m_nLast;
	LARGE_INTEGER	m_nAnimationInterval;
	UINT			m_nMilliSeconds;

private:
	static void __exec();

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
	static bool deleteTimer(tstring name);
	// ɾ�����ж�ʱ��
	static void clearAllTimers();
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

protected:
	int m_nRef;

public:
	Object();
	virtual ~Object();

	void retain();
	void release();
};


class FontStyle :
	public virtual Object
{
	friend class Text;

protected:
	LOGFONT m_font;

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
	friend class Layer;
	friend class BatchNode;

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
};


class BatchNode :
	public virtual Node
{
protected:
	std::vector<Node*> m_vChildren;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

public:
	BatchNode();
	virtual ~BatchNode();

	// ����ӽڵ�
	void add(Node *child, int z_Order = 0);
	// ɾ���ӽڵ�
	bool del(Node * child);
	// ��������ӽڵ�
	void clearAllChildren();
};


class Image :
	public virtual Node
{
	friend class ImageButton;

protected:
	CImage m_Image;
	CRect m_rDest;
	CRect m_rSrc;
	float m_fScaleX;
	float m_fScaleY;

protected:
	virtual void _onDraw() override;

public:
	Image();
	/**
	*  ��ͼƬ�ļ���ȡͼ��(png/bmp/jpg/gif/emf/wmf/ico)
	*  ������ͼƬ�ļ�����ͼƬ�ü����꣬ͼƬ�ü���Ⱥ͸߶�
	*/
	Image(LPCTSTR ImageFile, int x = 0, int y = 0, int width = 0, int height = 0);
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
	*  ��ͼƬ�ļ���ȡͼ��(png/bmp/jpg/gif/emf/wmf/ico)
	*  ������ͼƬ�ļ�����ͼƬ�ü����꣬ͼƬ�ü���Ⱥ͸߶�
	*/
	void setImageFile(LPCTSTR ImageFile, int x = 0, int y = 0, int width = 0, int height = 0);
	/**
	*  ����Դ�ļ���ȡͼ�񣬲�֧�� png (bmp/jpg/gif/emf/wmf/ico)
	*  ��������Դ���ƣ�ͼƬ�ü����꣬ͼƬ�ü���Ⱥ͸߶�
	*/
	void setImageRes(LPCTSTR pResName, int x = 0, int y = 0, int width = 0, int height = 0);
	// �ü�ͼƬ���ü����ָ� stretch ���죩
	void crop(int x = 0, int y = 0, int width = 0, int height = 0);
	// ��ͼƬ���쵽�̶����
	void stretch(int width = 0, int height = 0);
	// ����������ͼƬ��0 ~ 1.0f��
	void scale(float scaleX, float scaleY);
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
	// ���浽��ͼ
	static void screenshot();
};


class Text :
	public virtual Node
{
	friend class TextButton;

protected:
	tstring		m_sText;
	COLORREF	m_color;
	FontStyle *	m_pFontStyle;

protected:
	virtual void _onDraw() override;

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
};


class MouseNode :
	public virtual Node
{
private:
	bool m_bTarget;
	bool m_bBlock;
	enum { NORMAL, MOUSEIN, SELECTED } m_eStatus;
	CLICK_CALLBACK m_callback;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;

	void _setNormal();
	void _setMouseIn();
	void _setSelected();

	// ��д�ú�����ʵ�����λ�õ��ж�
	virtual void _judge() = 0;
	// ����״̬
	virtual void _onNormal() = 0;
	// �������ʱ
	virtual void _onMouseIn() = 0;
	// ���ѡ��ʱ
	virtual void _onSelected() = 0;

public:
	MouseNode();
	virtual ~MouseNode();

	// �����ʱ
	virtual void onClicked();
	// ����Ƿ�����
	virtual bool isMouseIn();
	// ����Ƿ�ѡ��
	virtual bool isSelected();
	// ���ûص�����
	virtual void setOnMouseClicked(const CLICK_CALLBACK & callback);
	// ����״̬
	virtual void reset();
	// ���ýڵ��Ƿ����������Ϣ
	void setBlock(bool block);
};


class Button :
	public virtual MouseNode
{
protected:
	int m_nWidth;
	int m_nHeight;
	bool m_bEnable;

protected:
	virtual bool _exec(bool active) override;
	virtual void _onDraw() override;
	virtual void _judge() override;

	virtual void _onNormal() = 0;
	virtual void _onMouseIn() = 0;
	virtual void _onSelected() = 0;
	virtual void _onDisable() = 0;

public:
	Button();
	virtual ~Button();

	// ��ť�Ƿ�����
	virtual bool isEnable();
	// �����Ƿ�����
	virtual void setEnable(bool enable);
};



class TextButton :
	public virtual Button
{
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

public:
	TextButton();
	TextButton(Text * text);
	virtual ~TextButton();

	// ���ð�ť����
	void setNormalText(Text * text);
	// �����������ʱ�İ�ť����
	void setMouseInText(Text * text);
	// �������ѡ��ʱ�İ�ť����
	void setSelectedText(Text * text);
	// ���ð�ť����ʱ�İ�ť����
	void setUnableText(Text * text);

	// ���ð�ť������
	virtual void setX(int x) override;
	// ���ð�ť������
	virtual void setY(int y) override;
	// ���ð�ť��������
	virtual void setPos(int x, int y) override;
};



class ImageButton :
	public virtual Button
{
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

public:
	ImageButton();
	ImageButton(Image * image);
	virtual ~ImageButton();

	// ���ð�ťͼƬ
	void setNormalImage(Image * image);
	// �����������ʱ�İ�ťͼƬ
	void setMouseInImage(Image * image);
	// �������ѡ��ʱ�İ�ťͼƬ
	void setSelectedImage(Image * image);
	// ���ð�ť����ʱ�İ�ťͼƬ
	void setUnableImage(Image * image);

	// ���ð�ť������
	virtual void setX(int x) override;
	// ���ð�ť������
	virtual void setY(int y) override;
	// ���ð�ť��������
	virtual void setPos(int x, int y) override;
};


class Shape :
	public virtual Node
{
protected:
	enum STYLE { round, solid, fill };	// ��״�����ʽ
	STYLE		_style;
	COLORREF	fillColor = 0;
	COLORREF	lineColor = 0;

protected:
	virtual void _onDraw() override;
	virtual void solidShape() = 0;
	virtual void fillShape() = 0;
	virtual void roundShape() = 0;

public:
	Shape();
	virtual ~Shape();

	// ��ȡ��״�������ɫ
	COLORREF getFillColor() const;
	// ��ȡ��״��������ɫ
	COLORREF getLineColor() const;
	// ���������ɫ
	void setFillColor(COLORREF color);
	// ����������ɫ
	void setLineColor(COLORREF color);
};


class Rect :
	public virtual Shape
{
protected:
	int m_nWidth;
	int m_nHeight;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;

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
};


class Circle :
	public virtual Shape
{
protected:
	int m_nRadius;

protected:
	virtual void solidShape() override;
	virtual void fillShape() override;
	virtual void roundShape() override;

public:
	Circle();
	Circle(int x, int y, int radius);
	virtual ~Circle();

	// ��ȡԲ�ΰ뾶
	int getRadius() const;
	// ����Բ�ΰ뾶
	void setRadius(int m_nRadius);
};

}	// End of easy2d namespace


// Ĭ��ʹ�� easy2d �����ռ�
using namespace easy2d;