#include <easy2d/e2dnode.h>
#include <easy2d/e2daction.h>

easy2d::Canvas::Canvas(const Size& size)
	: _image(nullptr)
	, _rt(nullptr)
	, _brush(nullptr)
	, _interpolationMode(InterpolationMode::Linear)
	, _drawing()
{
	setSize(size);
}

easy2d::Canvas::~Canvas()
{
	_discardResources();
}

void easy2d::Canvas::draw(const Function<void(CanvasBrush*)>& drawing)
{
	_initialize();
	_drawing = drawing;
	redraw();
}

void easy2d::Canvas::redraw()
{
	if (_drawing)
	{
		CanvasBrush brush(_rt, _brush);
		_drawing(&brush);
	}
}

void easy2d::Canvas::resizeAndClear(Size size)
{
	_discardResources();
	setSize(size);
}

easy2d::Image* easy2d::Canvas::getImage() const
{
	return _image;
}

void easy2d::Canvas::onRender()
{
	if (_image)
	{
		if (Renderer::isDeviceResourceRecreated())
		{
			// 重新构建资源并绘图
			_discardResources();
			_initialize();
			redraw();
		}

		Renderer::getRenderTarget()->DrawBitmap(
			_image->getBitmap(),
			reinterpret_cast<const D2D1_RECT_F&>(getBounds()),
			_displayOpacity,
			(_interpolationMode == InterpolationMode::Nearest) ? D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR : D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		);
	}
}

void easy2d::Canvas::_initialize()
{
	if (!_rt)
	{
		HRESULT hr = S_OK;
		
		ID2D1BitmapRenderTarget* rt = nullptr;
		hr = Renderer::getRenderTarget()->CreateCompatibleRenderTarget(
			reinterpret_cast<const D2D1_SIZE_F&>(getSize()),
			&rt
		);

		ID2D1SolidColorBrush* brush = nullptr;
		if (SUCCEEDED(hr))
		{
			hr = rt->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&brush
			);
		}

		if (SUCCEEDED(hr))
		{
			ID2D1Bitmap* bitmap = nullptr;
			hr = rt->GetBitmap(&bitmap);

			if (SUCCEEDED(hr))
			{
				_rt = rt;
				_rt->AddRef();

				_brush = brush;
				_brush->AddRef();

				if (_image)
				{
					_image->resetBitmap(bitmap);
				}
				else
				{
					_image = gcnew Image(bitmap);
					GC::retain(_image);
				}
			}
			SafeRelease(bitmap);
		}
		SafeRelease(brush);
		SafeRelease(rt);

		if (FAILED(hr))
		{
			E2D_ERROR(L"Canvas beginDraw failed! ERR_CODE=%#X", hr);
		}
	}
}

void easy2d::Canvas::_discardResources()
{
	SafeRelease(_brush);
	SafeRelease(_rt);
	GC::release(_image);
}

easy2d::InterpolationMode easy2d::Canvas::getInterpolationMode() const
{
	return _interpolationMode;
}

void easy2d::Canvas::setInterpolationMode(InterpolationMode mode)
{
	_interpolationMode = mode;
}

// CanvasBrush

easy2d::CanvasBrush::CanvasBrush(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush)
	: _rt(rt)
	, _brush(brush)
	, _state(nullptr)
	, _opacity(1.0f)
	, _style()
{
	_rt->AddRef();
	_brush->AddRef();

	HRESULT hr = Renderer::getID2D1Factory()->CreateDrawingStateBlock(&_state);
	if (SUCCEEDED(hr))
	{
		_rt->SaveDrawingState(_state);
	}
	else
	{
		E2D_ERROR(L"CanvasBrush create drawing state block failed! ERR_CODE=%#X", hr);
	}

	_rt->BeginDraw();
}

easy2d::CanvasBrush::~CanvasBrush()
{
	_rt->EndDraw();

	if (_state)
	{
		_rt->RestoreDrawingState(_state);
	}
	SafeRelease(_state);
	SafeRelease(_brush);
	SafeRelease(_rt);
}

void easy2d::CanvasBrush::drawShape(Shape* shape)
{
	_brush->SetOpacity(_opacity);
	if (_style.mode == DrawingStyle::Mode::Fill || _style.mode == DrawingStyle::Mode::Round)
	{
		ID2D1StrokeStyle* strokeStyle = nullptr;
		switch (_style.lineJoin)
		{
		case LineJoin::Miter:
			strokeStyle = Renderer::getMiterID2D1StrokeStyle();
			break;
		case LineJoin::Bevel:
			strokeStyle = Renderer::getBevelID2D1StrokeStyle();
			break;
		case LineJoin::Round:
			strokeStyle = Renderer::getRoundID2D1StrokeStyle();
			break;
		}
		_brush->SetColor(reinterpret_cast<const D2D1_COLOR_F&>(_style.strokeColor));
		_rt->DrawGeometry(shape->_geo, _brush, _style.strokeWidth, strokeStyle);
	}

	if (_style.mode == DrawingStyle::Mode::Fill || _style.mode == DrawingStyle::Mode::Solid)
	{
		_brush->SetColor(reinterpret_cast<const D2D1_COLOR_F&>(_style.fillColor));
		_rt->FillGeometry(shape->_geo, _brush);
	}
}

void easy2d::CanvasBrush::drawImage(Image* image, const Point& pos, const Rect& cropRect, InterpolationMode mode)
{
	drawImage(image, Rect(pos, image->getSize()), cropRect, mode);
}

void easy2d::CanvasBrush::drawImage(Image* image, const Rect& destRect, const Rect& cropRect, InterpolationMode mode)
{
	const Rect* srcRect = nullptr;
	if (!cropRect.isEmpty())
	{
		srcRect = &cropRect;
	}

	_rt->DrawBitmap(
		image->getBitmap(),
		reinterpret_cast<const D2D1_RECT_F&>(destRect),
		_opacity,
		(mode == InterpolationMode::Nearest) ? D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR : D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		reinterpret_cast<const D2D1_RECT_F*>(srcRect)
	);
}

void easy2d::CanvasBrush::drawImage(KeyFrame* frame, const Point& pos, InterpolationMode mode)
{
	auto image = frame->getImage();
	drawImage(image, Rect(pos, image->getSize()), frame->getCropRect(), mode);
}

void easy2d::CanvasBrush::drawImage(KeyFrame* frame, const Rect& destRect, InterpolationMode mode)
{
	auto image = frame->getImage();
	drawImage(image, destRect, frame->getCropRect(), mode);
}

void easy2d::CanvasBrush::drawText(TextLayout* layout, const Point& pos)
{
	_brush->SetOpacity(_opacity);
	Renderer::DrawTextLayout(layout, _style, pos, _rt, _brush);
}

void easy2d::CanvasBrush::drawText(const String& text, const Point& pos, const TextStyle& style)
{
	TextLayout layout(text, style);
	drawText(&layout, pos);
}

void easy2d::CanvasBrush::clear()
{
	_rt->Clear();
}

void easy2d::CanvasBrush::clear(const Color& color)
{
	_rt->Clear(reinterpret_cast<const D2D1_COLOR_F&>(color));
}

easy2d::Color easy2d::CanvasBrush::getFillColor() const
{
	return _style.fillColor;
}

easy2d::Color easy2d::CanvasBrush::getStrokeColor() const
{
	return _style.strokeColor;
}

float easy2d::CanvasBrush::getStrokeWidth() const
{
	return _style.strokeWidth;
}

void easy2d::CanvasBrush::setFillColor(Color fillColor)
{
	_style.fillColor = fillColor;
}

void easy2d::CanvasBrush::setStrokeColor(Color strokeColor)
{
	_style.strokeColor = strokeColor;
}

void easy2d::CanvasBrush::setStrokeWidth(float strokeWidth)
{
	_style.strokeWidth = strokeWidth;
}

void easy2d::CanvasBrush::setLineJoin(LineJoin lineJoin)
{
	_style.lineJoin = lineJoin;
}

easy2d::DrawingStyle::Mode easy2d::CanvasBrush::getDrawingMode() const
{
	return _style.mode;
}

void easy2d::CanvasBrush::setDrawingMode(DrawingStyle::Mode mode)
{
	_style.mode = mode;
}

easy2d::DrawingStyle easy2d::CanvasBrush::getDrawingStyle() const
{
	return _style;
}

void easy2d::CanvasBrush::setDrawingStyle(DrawingStyle style)
{
	_style = style;
}

float easy2d::CanvasBrush::getOpacity() const
{
	return _opacity;
}

void easy2d::CanvasBrush::setOpacity(float opacity)
{
	_opacity = max(min(opacity, 1.f), 0.f);
}

void easy2d::CanvasBrush::setTransform(const Matrix32& matrix)
{
	_rt->SetTransform(matrix.toD2DMatrix());
}
