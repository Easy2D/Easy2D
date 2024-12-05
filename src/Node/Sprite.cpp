#include <easy2d/e2dnode.h>
#include <easy2d/e2daction.h>

easy2d::Sprite::Sprite()
	: _image(nullptr)
	, _cropRect()
	, _interpolationMode(InterpolationMode::Linear)
{
}

easy2d::Sprite::Sprite(Image* image)
	: Sprite()
{
	setImage(image);
}

easy2d::Sprite::Sprite(const String& filePath)
	: Sprite()
{
	open(filePath);
}

easy2d::Sprite::Sprite(const Resource& res)
	: Sprite()
{
	open(res);
}

easy2d::Sprite::Sprite(KeyFrame* frame)
	: Sprite()
{
	setKeyFrame(frame);
}

easy2d::Sprite::Sprite(int resNameId, const String& resType)
	: Sprite()
{
	open(resNameId, resType);
}

easy2d::Sprite::Sprite(const String& filePath, const Rect& cropRect)
	: Sprite()
{
	open(filePath);
	crop(cropRect);
}

easy2d::Sprite::Sprite(const Resource& res, const Rect& cropRect)
	: Sprite()
{
	open(res);
	crop(cropRect);
}

easy2d::Sprite::Sprite(int resNameId, const String& resType, const Rect& cropRect)
	: Sprite()
{
	open(resNameId, resType);
	crop(cropRect);
}

easy2d::Sprite::~Sprite()
{
	GC::release(_image);
}

bool easy2d::Sprite::open(const String& filePath)
{
	auto image = Image::load(filePath);
	if (image)
	{
		setImage(image);
		return true;
	}
	return false;
}

bool easy2d::Sprite::open(const Resource& res)
{
	auto image = Image::load(res);
	if (image)
	{
		setImage(image);
		return true;
	}
	return false;
}

bool easy2d::Sprite::open(int resNameId, const String& resType)
{
	return open(Resource{ resNameId, resType });
}

void easy2d::Sprite::crop(const Rect& cropRect)
{
	if (cropRect.isEmpty())
	{
		// 不裁剪
		_cropRect = cropRect;
		if (_image)
		{
			setSize(_image->getWidth(), _image->getHeight());
		}
		else
		{
			setSize(Size{});
		}
	}
	else
	{
		// 保证裁剪矩形不超出图片大小
		_cropRect.leftTop.x = min(max(cropRect.leftTop.x, 0), _image->getWidth());
		_cropRect.leftTop.y = min(max(cropRect.leftTop.y, 0), _image->getHeight());
		_cropRect.rightBottom.x = min(max(cropRect.rightBottom.x, 0), _image->getWidth());
		_cropRect.rightBottom.y = min(max(cropRect.rightBottom.y, 0), _image->getHeight());
		setSize(
			min(max(cropRect.getWidth(), 0), _image->getWidth() - _cropRect.leftTop.x),
			min(max(cropRect.getHeight(), 0), _image->getHeight() - _cropRect.leftTop.y)
		);
	}
}

void easy2d::Sprite::setKeyFrame(KeyFrame* frame)
{
	setImage(frame->getImage());
	crop(frame->getCropRect());
}

easy2d::Rect easy2d::Sprite::getCropRect() const
{
	return _cropRect;
}

easy2d::InterpolationMode easy2d::Sprite::getInterpolationMode() const
{
	return _interpolationMode;
}

void easy2d::Sprite::setInterpolationMode(InterpolationMode mode)
{
	_interpolationMode = mode;
}

easy2d::Image * easy2d::Sprite::getImage() const
{
	return _image;
}

void easy2d::Sprite::setImage(Image* image, bool resetCropRect)
{
	if (_image != image)
	{
		GC::release(_image);
		_image = image;
		GC::retain(_image);
	}

	if (resetCropRect)
	{
		crop(Rect{});
	}
}

void easy2d::Sprite::onRender()
{
	if (_image)
	{
		Rect* srcRect = nullptr;
		if (!_cropRect.isEmpty())
		{
			srcRect = &_cropRect;
		}

		// 渲染图片
		Renderer::getRenderTarget()->DrawBitmap(
			_image->getBitmap(),
			reinterpret_cast<const D2D1_RECT_F&>(getBounds()),
			_displayOpacity,
			(_interpolationMode == InterpolationMode::Nearest) ? D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR : D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			reinterpret_cast<const D2D1_RECT_F*>(srcRect)
		);
	}
}
