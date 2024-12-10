#include <easy2d/e2dtext.h>
#include <easy2d/e2dbase.h>

//
// Font
//

easy2d::Font::Font()
	: family("")
	, size(22)
	, weight(Font::Weight::Normal)
	, italic(false)
{
}

easy2d::Font::Font(const String & family, float size, UINT weight, bool italic)
	: family(family)
	, size(size)
	, weight(weight)
	, italic(italic)
{
}

//
// TextBrush
//

easy2d::TextBrush::TextBrush(
	Font font,
	TextAlign alignment,
	bool wrapping,
	float wrappingWidth,
	float lineSpacing,
	bool hasUnderline,
	bool hasStrikethrough
)
	: font(font)
	, alignment(alignment)
	, wrapping(wrapping)
	, wrappingWidth(wrappingWidth)
	, lineSpacing(lineSpacing)
	, hasUnderline(hasUnderline)
	, hasStrikethrough(hasStrikethrough)
{
}

//
// Text
//

easy2d::Text::Text()
	: _textFormat(nullptr)
	, _textLayout(nullptr)
	, _size()
	, _content()
	, _style()
{
}

easy2d::Text::Text(const String& content, const TextBrush& style)
	: _textFormat(nullptr)
	, _textLayout(nullptr)
	, _size()
	, _content(content)
	, _style(style)
{
	_recreateFormat();
	_recreateLayout();
}

easy2d::Text::~Text()
{
	SafeRelease(_textFormat);
	SafeRelease(_textLayout);
}

const easy2d::String& easy2d::Text::getText() const
{
	return _content;
}

easy2d::Font easy2d::Text::getFont() const
{
	return _style.font;
}

easy2d::TextBrush easy2d::Text::getStyle() const
{
	return _style;
}

int easy2d::Text::getLineCount() const
{
	if (_textLayout)
	{
		DWRITE_TEXT_METRICS metrics;
		_textLayout->GetMetrics(&metrics);
		return static_cast<int>(metrics.lineCount);
	}
	else
	{
		return 0;
	}
}

bool easy2d::Text::hasStrikethrough() const
{
	return _style.hasStrikethrough;
}

bool easy2d::Text::hasUnderline() const
{
	return _style.hasUnderline;
}

easy2d::Size easy2d::Text::getSize() const
{
	return _size;
}

void easy2d::Text::setText(const String& content)
{
	if (_content != content)
	{
		_content = content;
		if (!_textFormat)
			_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setStyle(const TextBrush& style)
{
	_style = style;
	_recreateFormat();
	_recreateLayout();
}

void easy2d::Text::setFont(const Font& font)
{
	_style.font = font;
	_recreateFormat();
	_recreateLayout();
}

void easy2d::Text::setFontFamily(const String& family)
{
	if (_style.font.family != family)
	{
		_style.font.family = family;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setFontSize(float size)
{
	if (_style.font.size != size)
	{
		_style.font.size = size;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setFontWeight(UINT weight)
{
	if (_style.font.weight != weight)
	{
		_style.font.weight = weight;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setItalic(bool italic)
{
	if (_style.font.italic != italic)
	{
		_style.font.italic = italic;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setWrapping(bool wrapping)
{
	if (_style.wrapping != wrapping)
	{
		_style.wrapping = wrapping;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setWrappingWidth(float wrappingWidth)
{
	if (_style.wrappingWidth != wrappingWidth)
	{
		_style.wrappingWidth = max(wrappingWidth, 0);

		if (_style.wrapping)
		{
			_recreateFormat();
			_recreateLayout();
		}
	}
}

void easy2d::Text::setLineSpacing(float lineSpacing)
{
	if (_style.lineSpacing != lineSpacing)
	{
		_style.lineSpacing = lineSpacing;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setAlignment(TextAlign align)
{
	if (_style.alignment != align)
	{
		_style.alignment = align;
		_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setUnderline(bool hasUnderline)
{
	if (_style.hasUnderline != hasUnderline)
	{
		_style.hasUnderline = hasUnderline;
		if (!_textFormat)
			_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (_style.hasStrikethrough != hasStrikethrough)
	{
		_style.hasStrikethrough = hasStrikethrough;
		if (!_textFormat)
			_recreateFormat();
		_recreateLayout();
	}
}

void easy2d::Text::reset(const String& content, const TextBrush& style)
{
	_content = content;
	_style = style;
	_recreateFormat();
	_recreateLayout();
}

void easy2d::Text::_recreateFormat()
{
	SafeRelease(_textFormat);

	WideString fontFamily;

	if (!_style.font.family.empty())
	{
		fontFamily = NarrowToWide(_style.font.family);
	}

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		fontFamily.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT(_style.font.weight),
		_style.font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		float(_style.font.size),
		L"",
		&_textFormat
	);

	if (FAILED(hr))
	{
		E2D_WARNING("Text::_createFormat error : Create IDWriteTextFormat failed!");
		_textFormat = nullptr;
		return;
	}
	else
	{
		// 设置文字对齐方式
		_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(_style.alignment));
		// 设置行间距
		if (_style.lineSpacing == 0.0)
		{
			_textFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			_textFormat->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				float(_style.lineSpacing),
				float(_style.lineSpacing) * 0.8f
			);
		}
		// 打开文本自动换行时，设置换行属性
		if (_style.wrapping)
		{
			_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
		else
		{
			_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}
}

void easy2d::Text::_recreateLayout()
{
	SafeRelease(_textLayout);

	// 文本为空字符串时，重置属性
	if (_content.empty())
	{
		_size = Size{};
		return;
	}

	if (_textFormat == nullptr)
	{
		E2D_WARNING("Text::_createLayout failed! _textFormat NULL pointer exception.");
		return;
	}

	WideString content = NarrowToWide(_content);
	UINT32 length = (UINT32)content.length();

	// 创建 Text
	HRESULT hr;
	// 对文本自动换行情况下进行处理
	if (_style.wrapping)
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(
			content.c_str(),
			length,
			_textFormat,
			float(_style.wrappingWidth),
			0,
			&_textLayout
		);
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			_size = Size(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(content.c_str(), length, _textFormat, 0, 0, &_textLayout);
		// 为防止文本对齐问题，根据刚才创建的 layout 宽度重新创建它
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			_size = Size(metrics.width, metrics.height);
			// 重新创建 layout
			SafeRelease(_textLayout);
			hr = Renderer::getIDWriteFactory()->CreateTextLayout(content.c_str(), length, _textFormat, _size.width, 0, &_textLayout);
		}
	}

	if (FAILED(hr))
	{
		E2D_WARNING("Text::_createLayout error : Create IDWriteTextLayout failed!");
		_textLayout = nullptr;
		return;
	}

	// 添加下划线和删除线
	DWRITE_TEXT_RANGE range = { 0, length };
	if (_style.hasUnderline)
	{
		_textLayout->SetUnderline(true, range);
	}
	if (_style.hasStrikethrough)
	{
		_textLayout->SetStrikethrough(true, range);
	}
}
