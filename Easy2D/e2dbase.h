#pragma once
#include <Windows.h>
#include <tchar.h>
#include <atltypes.h>
#include <vector>
#include <stack>
#include <functional>

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


// Type Declare

typedef CPoint						CVector;
typedef unsigned int				VK_KEY;
typedef std::function<void()>		CLICK_CALLBACK;
typedef std::function<void()>		TIMER_CALLBACK;
typedef std::function<void(VK_KEY)>	KEY_CALLBACK;
typedef std::function<void()>		MOUSE_CALLBACK;


// Classes Declare

namespace easy2d
{
	class Scene;
	class Node;

	class App
	{
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
		void createWindow(tstring title, int width, int height, int mode = 0);
		// �����ͼ����
		void createWindow(tstring title, CSize size, int mode = 0);
		// ��������
		int run();
		// �ͷ������ڴ���Դ
		void free();
		// ���ٸö���
		void destory();

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
		static void setWindowTitle(tstring title);
		// ��ȡ���ڱ���
		static tstring getWindowTitle();
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
		static void setAppName(tstring appname);
		// ��ȡ AppName
		static tstring getAppName();
		// �޸Ĵ��ڱ���ɫ
		static void setBkColor(COLORREF color);
		// ����֡��
		static void setFPS(DWORD fps);
		// ���û�ͼ��ʽΪĬ��ֵ
		static void reset();
		// ��ȡ��ǰ����
		static Scene * getCurrentScene();

	protected:
		tstring				m_sTitle;
		tstring				m_sAppName;
		Scene*				m_CurrentScene;
		Scene*				m_NextScene;
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

}	// End of easy2d namespace