#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{

class Renderer;

// �ı����뷽ʽ
enum class TextAlign
{
	Left,		/* ����� */
	Right,		/* �Ҷ��� */
	Center		/* ���ж��� */
};


// ����
struct Font
{
	String	family;			// ������
	float	size;			// �ֺ�
	UINT	weight;			// ��ϸֵ
	bool	italic;			// б��

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

	Font();

	Font(
		const String& family,
		float size,
		UINT weight = Font::Weight::Normal,
		bool italic = false
	);
};


// �ı���ˢ
struct TextBrush
{
	Font		font;				// ����
	TextAlign	alignment;			// ���뷽ʽ
	bool		wrapping;			// ���Զ�����
	float		wrappingWidth;		// �Զ����п��
	float		lineSpacing;		// �м��
	bool		hasUnderline;		// �»���
	bool		hasStrikethrough;	// ɾ����

	TextBrush(
		Font font = Font(),
		TextAlign alignment = TextAlign::Left,
		bool wrapping = false,
		float wrappingWidth = 0.0,
		float lineSpacing = 0.0,
		bool hasUnderline = false,
		bool hasStrikethrough = false
	);
};


// �ı�
class Text
	: public Object
{
	friend Renderer;

public:
	Text();

	Text(
		const String& content,
		const TextBrush& style = TextBrush()
	);

	virtual ~Text();

	// ��ȡ�ı�����
	const String& getContent() const;

	// ��ȡ����
	Font getFont() const;

	// ��ȡ�ı���ˢ
	TextBrush getStyle() const;

	// ��ȡ�ı���ʾ����
	int getLineCount() const;

	// �Ƿ���ʾɾ����
	bool hasStrikethrough() const;

	// �Ƿ���ʾ�»���
	bool hasUnderline() const;

	// ��ȡ��С
	Size getSize() const;

	// �����ı�����
	void setContent(
		const String& text
	);

	// �����ı���ˢ
	void setBrush(
		const TextStyle& style
	);

	// ��������
	void setFont(
		const Font& font
	);

	// ����������
	void setFontFamily(
		const String& family
	);

	// �����ֺ�
	void setFontSize(
		float size
	);

	// ���������ϸֵ
	void setFontWeight(
		UINT weight
	);

	// ��������б��
	void setItalic(
		bool italic
	);

	// �򿪻�ر��ı��Զ�����
	void setWrapping(
		bool wrapping
	);

	// �����ı��Զ����еĿ��
	void setWrappingWidth(
		float wrappingWidth
	);

	// �����м�ࣨĬ��Ϊ 0��
	void setLineSpacing(
		float lineSpacing
	);

	// ���ö��뷽ʽ
	void setAlignment(
		TextAlign align
	);

	// �����»���
	void setUnderline(
		bool hasUnderline
	);

	// ����ɾ����
	void setStrikethrough(
		bool hasStrikethrough
	);

	void reset(const String& content, const TextBrush& textBrush);

protected:
	// �������ָ�ʽ��
	void _recreateFormat();

	// �������ֲ���
	void _recreateLayout();

protected:
	IDWriteTextFormat* _textFormat;
	IDWriteTextLayout* _textLayout;
	Size				_size;
	String				_content;
	TextBrush			_textBrush;
};

}
