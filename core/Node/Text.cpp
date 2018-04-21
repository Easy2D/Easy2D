#include "..\e2dnode.h"


e2d::Text::Text()
	: m_bWrappingEnable(false)
	, m_Font()
	, m_nAlign(TextAlign::LEFT)
	, m_fLineSpacing(0.0f)
	, m_fWrappingWidth(0.0f)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
{
}

e2d::Text::Text(String text)
	: m_bWrappingEnable(false)
	, m_Font()
	, m_nAlign(TextAlign::LEFT)
	, m_fLineSpacing(0.0f)
	, m_fWrappingWidth(0.0f)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
	, m_sText(text)
{
	_reset();
}

e2d::Text::Text(Font font)
	: m_bWrappingEnable(false)
	, m_Font(font)
	, m_nAlign(TextAlign::LEFT)
	, m_fLineSpacing(0.0f)
	, m_fWrappingWidth(0.0f)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
{
	_reset();
}

e2d::Text::Text(String text, Font font)
	: m_bWrappingEnable(false)
	, m_Font(font)
	, m_nAlign(TextAlign::LEFT)
	, m_fLineSpacing(0.0f)
	, m_fWrappingWidth(0.0f)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
	, m_sText(text)
{
	_reset();
}

e2d::Text::Text(String text, String fontFamily, double fontSize, UINT32 color, UINT32 fontWeight, bool italic, bool hasUnderline, bool hasStrikethrough)
	: m_bWrappingEnable(false)
	, m_Font(Font(fontFamily, fontSize, color, fontWeight, italic, hasUnderline, hasStrikethrough))
	, m_nAlign(TextAlign::LEFT)
	, m_fLineSpacing(0.0f)
	, m_fWrappingWidth(0.0f)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
	, m_sText(text)
{
	_reset();
}

e2d::Text::~Text()
{
	SafeReleaseInterface(&m_pDWriteTextFormat);
	SafeReleaseInterface(&m_pDWriteTextLayout);
}

e2d::String e2d::Text::getText() const
{
	return m_sText;
}

e2d::Font e2d::Text::getFont() const
{
	return m_Font;
}

e2d::String e2d::Text::getFontFamily() const
{
	return m_Font.fontFamily;
}

double e2d::Text::getFontSize() const
{
	return m_Font.size;
}

UINT32 e2d::Text::getFontWeight() const
{
	return m_Font.weight;
}

UINT32 e2d::Text::getColor() const
{
	return m_Font.color;
}

int e2d::Text::getLineCount() const
{
	if (m_pDWriteTextLayout)
	{
		DWRITE_TEXT_METRICS metrics;
		m_pDWriteTextLayout->GetMetrics(&metrics);
		return static_cast<int>(metrics.lineCount);
	}
	else
	{
		return 0;
	}
}

bool e2d::Text::isItalic() const
{
	return m_Font.italic;
}

void e2d::Text::setText(String text)
{
	m_sText = text;
	_reset();
}

void e2d::Text::setFont(Font font)
{
	m_Font = font;
	_reset();
}

void e2d::Text::setFontFamily(String fontFamily)
{
	m_Font.fontFamily = fontFamily;
	_reset();
}

void e2d::Text::setFontSize(double fontSize)
{
	m_Font.size = static_cast<float>(fontSize);
	_reset();
}

void e2d::Text::setFontWeight(UINT32 fontWeight)
{
	m_Font.weight = fontWeight;
	_reset();
}

void e2d::Text::setColor(UINT32 color)
{
	m_Font.color = color;
}

void e2d::Text::setItalic(bool value)
{
	m_Font.italic = value;
	_reset();
}

void e2d::Text::setWrappingWidth(double fWrappingWidth)
{
	if (m_fWrappingWidth != fWrappingWidth)
	{
		m_fWrappingWidth = max(static_cast<float>(fWrappingWidth), 0);
		m_bWrappingEnable = (m_fWrappingWidth > FLT_MIN);
		_reset();
	}
}

void e2d::Text::setLineSpacing(double fLineSpacing)
{
	if (m_fLineSpacing != fLineSpacing)
	{
		m_fLineSpacing = static_cast<float>(fLineSpacing);
		_reset();
	}
}

void e2d::Text::setAlignment(int nAlign)
{
	if (m_nAlign != nAlign)
	{
		m_nAlign = nAlign;
		_reset();
	}
}

void e2d::Text::setUnderline(bool hasUnderline)
{
	if (m_Font.underline != hasUnderline)
	{
		m_Font.underline = hasUnderline;
		if (!m_pDWriteTextFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (m_Font.strikethrough != hasStrikethrough)
	{
		m_Font.strikethrough = hasStrikethrough;
		if (!m_pDWriteTextFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::onRender()
{
	// �����ı�����
	D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, m_fWidth, m_fHeight);
	// ���û�ˢ��ɫ��͸����
	Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_Font.color, m_fDisplayOpacity));
	// ��Ⱦ��������
	if (m_pDWriteTextLayout)
	{
		Renderer::getRenderTarget()->DrawTextLayout(
			D2D1::Point2F(0, 0),
			m_pDWriteTextLayout,
			Renderer::getSolidColorBrush()
		);
	}
}

void e2d::Text::_reset()
{
	// �������ָ�ʽ��
	_createFormat();
	// �������ֲ���
	_createLayout();
}

void e2d::Text::_createFormat()
{
	SafeReleaseInterface(&m_pDWriteTextFormat);

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		m_Font.fontFamily,
		NULL,
		DWRITE_FONT_WEIGHT(m_Font.weight),
		m_Font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		static_cast<float>(m_Font.size),
		L"",
		&m_pDWriteTextFormat
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	if (m_pDWriteTextFormat)
	{
		// �������ֶ��뷽ʽ
		m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(m_nAlign));
		// �����м��
		if (m_fLineSpacing == 0.0f)
		{
			m_pDWriteTextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			m_pDWriteTextFormat->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				m_fLineSpacing,
				m_fLineSpacing * 0.8f
			);
		}
		// ���ı��Զ�����ʱ�����û�������
		if (m_bWrappingEnable)
		{
			m_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
		else
		{
			m_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}
}

void e2d::Text::_createLayout()
{
	SafeReleaseInterface(&m_pDWriteTextLayout);

	// �ı�Ϊ���ַ���ʱ����������
	if (m_sText.isEmpty())
	{
		this->setSize(0, 0);
		return;
	}

	if (m_pDWriteTextFormat == nullptr)
	{
		WARN_IF(true, "Text::_createLayout failed! m_pDWriteTextFormat NULL pointer exception.");
		return;
	}
	
	UINT32 length = static_cast<UINT32>(m_sText.getLength());

	// ���� TextLayout
	HRESULT hr;
	// ���ı��Զ���������½��д���
	if (m_bWrappingEnable)
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(
			m_sText,
			length,
			m_pDWriteTextFormat,
			m_fWrappingWidth,
			0,
			&m_pDWriteTextLayout
		);
		if (m_pDWriteTextLayout)
		{
			// ��ȡ�ı����ֵĿ�Ⱥ͸߶�
			DWRITE_TEXT_METRICS metrics;
			m_pDWriteTextLayout->GetMetrics(&metrics);
			// �����ı����
			this->setSize(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(m_sText, length, m_pDWriteTextFormat, 0, 0, &m_pDWriteTextLayout);
		// Ϊ��ֹ�ı��������⣬���ݸղŴ����� layout ������´�����
		if (m_pDWriteTextLayout)
		{
			// ��ȡ�ı����ֵĿ�Ⱥ͸߶�
			DWRITE_TEXT_METRICS metrics;
			m_pDWriteTextLayout->GetMetrics(&metrics);
			// �����ı����
			this->setSize(metrics.width, metrics.height);
			// ���´��� layout
			SafeReleaseInterface(&m_pDWriteTextLayout);
			hr = Renderer::getIDWriteFactory()->CreateTextLayout(m_sText, length, m_pDWriteTextFormat, m_fWidth, 0, &m_pDWriteTextLayout);
		}
	}

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	// ����»��ߺ�ɾ����
	DWRITE_TEXT_RANGE range = { 0, length };
	if (m_Font.underline)
	{
		m_pDWriteTextLayout->SetUnderline(true, range);
	}
	if (m_Font.strikethrough)
	{
		m_pDWriteTextLayout->SetStrikethrough(true, range);
	}
}
