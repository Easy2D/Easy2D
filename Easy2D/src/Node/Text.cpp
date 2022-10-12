#include <easy2d/e2dnode.h>

easy2d::Text::Text()
	: Text(gcnew TextLayout())
{
}

easy2d::Text::Text(const String& text, const TextStyle& textStyle, const DrawingStyle& drawingStyle)
	: Text(gcnew TextLayout(text, textStyle), drawingStyle)
{
}

easy2d::Text::Text(TextLayout* layout, const DrawingStyle& drawingStyle)
	: _layout(nullptr)
	, _style(drawingStyle)
{
	setTextLayout(layout);
}

easy2d::Text::~Text()
{
	GC::release(_layout);
}

easy2d::TextLayout* easy2d::Text::getLayout() const
{
	return _layout;
}

easy2d::String easy2d::Text::getText() const
{
	return _layout->getText();
}

easy2d::Font easy2d::Text::getFont() const
{
	return _layout->getFont();
}

easy2d::TextStyle easy2d::Text::getTextStyle() const
{
	return _layout->getStyle();
}

easy2d::DrawingStyle easy2d::Text::getDrawingStyle() const
{
	return _style;
}

int easy2d::Text::getLineCount() const
{
	return _layout->getLineCount();
}

void easy2d::Text::setTextLayout(TextLayout* layout)
{
	if (_layout != layout)
	{
		GC::release(_layout);
		_layout = layout;
		updateLayout();
		GC::retain(_layout);
	}
}

void easy2d::Text::setText(const String& text)
{
	_layout->setText(text);
	updateLayout();
}

void easy2d::Text::setTextStyle(const TextStyle& style)
{
	_layout->setStyle(style);
	updateLayout();
}

void easy2d::Text::setDrawingStyle(DrawingStyle style)
{
	_style = style;
}

void easy2d::Text::setFont(const Font & font)
{
	_layout->setFont(font);
	updateLayout();
}

void easy2d::Text::setFontFamily(const String& family)
{
	_layout->setFontFamily(family);
	updateLayout();
}

void easy2d::Text::setFontSize(float size)
{
	_layout->setFontSize(size);
	updateLayout();
}

void easy2d::Text::setFontWeight(UINT weight)
{
	_layout->setFontWeight(weight);
	updateLayout();
}

void easy2d::Text::setItalic(bool italic)
{
	_layout->setItalic(italic);
	updateLayout();
}

void easy2d::Text::setFillColor(Color color)
{
	_style.fillColor = color;
}

void easy2d::Text::setWrapping(bool wrapping)
{
	_layout->setWrapping(wrapping);
	updateLayout();
}

void easy2d::Text::setWrappingWidth(float wrappingWidth)
{
	_layout->setWrappingWidth(wrappingWidth);
	updateLayout();
}

void easy2d::Text::setLineSpacing(float lineSpacing)
{
	_layout->setLineSpacing(lineSpacing);
	updateLayout();
}

void easy2d::Text::setAlignment(TextAlign align)
{
	_layout->setAlignment(align);
	updateLayout();
}

void easy2d::Text::setUnderline(bool hasUnderline)
{
	_layout->setUnderline(hasUnderline);
	updateLayout();
}

void easy2d::Text::setStrikethrough(bool hasStrikethrough)
{
	_layout->setStrikethrough(hasStrikethrough);
	updateLayout();
}

void easy2d::Text::setStrokeColor(Color strokeColor)
{
	_style.strokeColor = strokeColor;
}

void easy2d::Text::setStrokeWidth(float strokeWidth)
{
	_style.strokeWidth = strokeWidth;
}

void easy2d::Text::setLineJoin(LineJoin lineJoin)
{
	_style.lineJoin = lineJoin;
}

void easy2d::Text::onRender()
{
	if (_layout)
	{
		Renderer::getSolidColorBrush()->SetOpacity(_displayOpacity);
		Renderer::DrawTextLayout(_layout, _style);
	}
}

void easy2d::Text::updateLayout()
{
	setSize(_layout->getSize());
}
