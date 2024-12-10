#include <easy2d/e2dtext.h>

easy2d::TextStyle::TextStyle(
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
{}
