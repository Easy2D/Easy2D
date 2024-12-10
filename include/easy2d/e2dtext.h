#pragma once
#include <easy2d/e2dcommon.h>

namespace easy2d
{

class Renderer;

// 文本对齐方式
enum class TextAlign
{
	Left,		/* 左对齐 */
	Right,		/* 右对齐 */
	Center		/* 居中对齐 */
};


// 字体
struct Font
{
	String	family;			// 字体族
	float	size;			// 字号
	UINT	weight;			// 粗细值
	bool	italic;			// 斜体

	// 字体粗细值
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


// 文本画刷
struct TextBrush
{
	Font		font;				// 字体
	TextAlign	alignment;			// 对齐方式
	bool		wrapping;			// 打开自动换行
	float		wrappingWidth;		// 自动换行宽度
	float		lineSpacing;		// 行间距
	bool		hasUnderline;		// 下划线
	bool		hasStrikethrough;	// 删除线

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


// 文本
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

	// 获取文本内容
	const String& getContent() const;

	// 获取字体
	Font getFont() const;

	// 获取文本画刷
	TextBrush getStyle() const;

	// 获取文本显示行数
	int getLineCount() const;

	// 是否显示删除线
	bool hasStrikethrough() const;

	// 是否显示下划线
	bool hasUnderline() const;

	// 获取大小
	Size getSize() const;

	// 设置文本内容
	void setContent(
		const String& text
	);

	// 设置文本画刷
	void setBrush(
		const TextStyle& style
	);

	// 设置字体
	void setFont(
		const Font& font
	);

	// 设置字体族
	void setFontFamily(
		const String& family
	);

	// 设置字号
	void setFontSize(
		float size
	);

	// 设置字体粗细值
	void setFontWeight(
		UINT weight
	);

	// 设置文字斜体
	void setItalic(
		bool italic
	);

	// 打开或关闭文本自动换行
	void setWrapping(
		bool wrapping
	);

	// 设置文本自动换行的宽度
	void setWrappingWidth(
		float wrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		float lineSpacing
	);

	// 设置对齐方式
	void setAlignment(
		TextAlign align
	);

	// 设置下划线
	void setUnderline(
		bool hasUnderline
	);

	// 设置删除线
	void setStrikethrough(
		bool hasStrikethrough
	);

	void reset(const String& content, const TextBrush& textBrush);

protected:
	// 创建文字格式化
	void _recreateFormat();

	// 创建文字布局
	void _recreateLayout();

protected:
	IDWriteTextFormat* _textFormat;
	IDWriteTextLayout* _textLayout;
	Size				_size;
	String				_content;
	TextBrush			_textBrush;
};

}
