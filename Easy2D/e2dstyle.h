#pragma once
#include <e2dbase.h>

namespace easy2d 
{
	class FontStyle :
		public Object
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

}	// End of easy2d namespace