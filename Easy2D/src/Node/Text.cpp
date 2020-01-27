#include <easy2d/e2dnode.h>

//-------------------------------------------------------
// Style
//-------------------------------------------------------

easy2d::Text::Style::Style()
	: color(Color::White)
	, alignment(Align::Left)
	, wrapping(false)
	, wrappingWidth(0.0)
	, lineSpacing(0.0)
	, hasUnderline(false)
	, hasStrikethrough(false)
	, hasOutline(true)
	, outlineColor(Color(Color::Black, 0.5))
	, outlineWidth(1.0)
	, outlineJoin(LineJoin::Round)
{}

easy2d::Text::Style::Style(
	Color color,
	Align alignment,
	bool wrapping,
	float wrappingWidth,
	float lineSpacing,
	bool hasUnderline,
	bool hasStrikethrough,
	bool hasOutline,
	Color outlineColor,
	float outlineWidth,
	LineJoin outlineJoin
)
	: color(color)
	, alignment(alignment)
	, wrapping(wrapping)
	, wrappingWidth(wrappingWidth)
	, lineSpacing(lineSpacing)
	, hasUnderline(hasUnderline)
	, hasStrikethrough(hasStrikethrough)
	, hasOutline(hasOutline)
	, outlineColor(outlineColor)
	, outlineWidth(outlineWidth)
	, outlineJoin(outlineJoin)
{}



//-------------------------------------------------------
// Text
//-------------------------------------------------------

easy2d::Text::Text()
	: _font()
	, _style()
	, _textLayout(nullptr)
	, _textFormat(nullptr)
{
}

easy2d::Text::Text(const String & text, const Font & font, const Style & style)
	: _font(font)
	, _style(style)
	, _textLayout(nullptr)
	, _textFormat(nullptr)
	, _text(text)
{
	_reset();
}

easy2d::Text::~Text()
{
	SafeRelease(_textFormat);
	SafeRelease(_textLayout);
}

easy2d::String easy2d::Text::getText() const
{
	return _text;
}

easy2d::Font easy2d::Text::getFont() const
{
	return _font;
}

easy2d::Text::Style easy2d::Text::getStyle() const
{
	return _style;
}

easy2d::String easy2d::Text::getFontFamily() const
{
	return _font.family;
}

float easy2d::Text::getFontSize() const
{
	return _font.size;
}

UINT easy2d::Text::getFontWeight() const
{
	return _font.weight;
}

easy2d::Color easy2d::Text::getColor() const
{
	return _style.color;
}

easy2d::Color easy2d::Text::getOutlineColor() const
{
	return _style.outlineColor;
}

float easy2d::Text::getOutlineWidth() const
{
	return _style.outlineWidth;
}

easy2d::LineJoin easy2d::Text::getOutlineJoin() const
{
	return _style.outlineJoin;
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

bool easy2d::Text::isItalic() const
{
	return _font.italic;
}

bool easy2d::Text::hasStrikethrough() const
{
	return _style.hasStrikethrough;
}

bool easy2d::Text::hasUnderline() const
{
	return _style.hasUnderline;
}

bool easy2d::Text::hasOutline() const
{
	return _style.hasOutline;
}

void easy2d::Text::setText(const String& text)
{
	_text = text;
	_reset();
}

void easy2d::Text::setStyle(const Style& style)
{
	_style = style;
	_reset();
}

void easy2d::Text::setFont(const Font & font)
{
	_font = font;
	_reset();
}

void easy2d::Text::setFontFamily(const String& family)
{
	_font.family = family;
	_reset();
}

void easy2d::Text::setFontSize(float size)
{
	_font.size = size;
	_reset();
}

void easy2d::Text::setFontWeight(UINT weight)
{
	_font.weight = weight;
	_reset();
}

void easy2d::Text::setColor(Color color)
{
	_style.color = color;
}

void easy2d::Text::setItalic(bool value)
{
	_font.italic = value;
	_reset();
}

void easy2d::Text::setWrapping(bool wrapping)
{
	if (_style.wrapping != wrapping)
	{
		_style.wrapping = wrapping;
		_reset();
	}
}

void easy2d::Text::setWrappingWidth(float wrappingWidth)
{
	if (_style.wrappingWidth != wrappingWidth)
	{
		_style.wrappingWidth = max(wrappingWidth, 0);

		if (_style.wrapping)
		{
			_reset();
		}
	}
}

void easy2d::Text::setLineSpacing(float lineSpacing)
{
	if (_style.lineSpacing != lineSpacing)
	{
		_style.lineSpacing = lineSpacing;
		_reset();
	}
}

void easy2d::Text::setAlignment(Align align)
{
	if (_style.alignment != align)
	{
		_style.alignment = align;
		_reset();
	}
}

void easy2d::Text::setUnderline(bool hasUnderline)
{
	if (_style.hasUnderline != hasUnderline)
	{
		_style.hasUnderline = hasUnderline;
		if (!_textFormat)
			_createFormat();
		_createLayout();
	}
}

void easy2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (_style.hasStrikethrough != hasStrikethrough)
	{
		_style.hasStrikethrough = hasStrikethrough;
		if (!_textFormat)
			_createFormat();
		_createLayout();
	}
}

void easy2d::Text::setOutline(bool hasOutline)
{
	_style.hasOutline = hasOutline;
}

void easy2d::Text::setOutlineColor(Color outlineColor)
{
	_style.outlineColor = outlineColor;
}

void easy2d::Text::setOutlineWidth(float outlineWidth)
{
	_style.outlineWidth = outlineWidth;
}

void easy2d::Text::setOutlineJoin(LineJoin outlineJoin)
{
	_style.outlineJoin = outlineJoin;
}

void easy2d::Text::onRender()
{
	if (_textLayout)
	{
		// 设置画刷颜色和透明度
		Renderer::getSolidColorBrush()->SetOpacity(_displayOpacity);
		// 设置文本渲染样式
		Renderer::SetTextStyle(
			_style.color,
			_style.hasOutline,
			_style.outlineColor,
			(float)_style.outlineWidth,
			_style.outlineJoin
		);
		Renderer::DrawTextLayout(_textLayout);
	}
}

void easy2d::Text::_reset()
{
	// 创建文字格式化
	_createFormat();
	// 创建文字布局
	_createLayout();
}

void easy2d::Text::_createFormat()
{
	SafeRelease(_textFormat);

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		_font.family.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT(_font.weight),
		_font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		float(_font.size),
		L"",
		&_textFormat
	);

	if (FAILED(hr))
	{
		E2D_WARNING(L"Text::_createFormat error : Create IDWriteTextFormat failed!");
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

void easy2d::Text::_createLayout()
{
	SafeRelease(_textLayout);

	// 文本为空字符串时，重置属性
	if (_text.empty())
	{
		this->setSize(0, 0);
		return;
	}

	if (_textFormat == nullptr)
	{
		E2D_WARNING(L"Text::_createLayout failed! _textFormat NULL pointer exception.");
		return;
	}
	
	UINT32 length = (UINT32)_text.length();

	// 创建 TextLayout
	HRESULT hr;
	// 对文本自动换行情况下进行处理
	if (_style.wrapping)
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(
			_text.c_str(),
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
			this->setSize(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(_text.c_str(), length, _textFormat, 0, 0, &_textLayout);
		// 为防止文本对齐问题，根据刚才创建的 layout 宽度重新创建它
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.width, metrics.height);
			// 重新创建 layout
			SafeRelease(_textLayout);
			hr = Renderer::getIDWriteFactory()->CreateTextLayout(_text.c_str(), length, _textFormat, _width, 0, &_textLayout);
		}
	}

	if (FAILED(hr))
	{
		E2D_WARNING(L"Text::_createLayout error : Create IDWriteTextLayout failed!");
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
