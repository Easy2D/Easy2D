#pragma once
#include <e2dbase.h>
#include <atlimage.h>

namespace easy2d 
{
	class FontStyle;
	class Color;
	class Action;

	class Node :
		public Object
	{
		friend class Scene;
		friend class BatchNode;

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


	class Image :
		public RectNode
	{
		friend class Sprite;
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
		// ������Ϸ��ͼ
		static void saveScreenshot();

	protected:
		CImage	m_Image;
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
		friend class BatchSprite;
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
		// ��ͣ���ж���
		virtual void pauseAllAction();
		// �������ж���
		virtual void resumeAllAction();
		// ֹͣ���ж���
		virtual void stopAllAction();

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
		public Node
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
		// ͬʱ�޸����о����ͼƬ
		virtual void setImage(Image * image);

		virtual float getScaleX() const;
		virtual float getScaleY() const;
		virtual float getOpacity() const;

		virtual void setScale(float scaleX, float scaleY);
		virtual void setOpacity(float opacity);

	protected:
		std::vector<Sprite*> m_vSprites;
		float	m_fScaleX;
		float	m_fScaleY;
		BYTE	m_nAlpha;

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

}	// End of easy2d namespace