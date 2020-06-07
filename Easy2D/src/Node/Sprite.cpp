#include <easy2d/e2dnode.h>


easy2d::Sprite::Sprite()
	: _image(nullptr)
{
}

easy2d::Sprite::Sprite(Image * image)
	: _image(nullptr)
{
	open(image);
}

easy2d::Sprite::Sprite(const String& filePath)
	: _image(nullptr)
{
	open(filePath);
}

easy2d::Sprite::Sprite(int resNameId, const String& resType)
	: _image(nullptr)
{
	open(resNameId, resType);
}

easy2d::Sprite::Sprite(const String& filePath, const Rect& cropRect)
	: _image(nullptr)
{
	open(filePath);
	crop(cropRect);
}

easy2d::Sprite::Sprite(int resNameId, const String& resType, const Rect& cropRect)
	: _image(nullptr)
{
	open(resNameId, resType);
	crop(cropRect);
}

easy2d::Sprite::~Sprite()
{
	GC::release(_image);
}

bool easy2d::Sprite::open(Image * image)
{
	if (image)
	{
		GC::release(_image);
		_image = image;
		_image->retain();

		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

bool easy2d::Sprite::open(const String& filePath)
{
	if (!_image)
	{
		_image = gcnew Image;
		_image->retain();
	}

	if (_image->open(filePath))
	{
		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

bool easy2d::Sprite::open(int resNameId, const String& resType)
{
	if (!_image)
	{
		_image = gcnew Image;
		_image->retain();
	}

	if (_image->open(resNameId, resType))
	{
		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

void easy2d::Sprite::crop(const Rect& cropRect)
{
	_image->crop(cropRect);
	Node::setSize(
		min(max(cropRect.size.width, 0), _image->getSourceWidth() - _image->getCropX()),
		min(max(cropRect.size.height, 0), _image->getSourceHeight() - _image->getCropY())
	);
}

easy2d::Image * easy2d::Sprite::getImage() const
{
	return _image;
}

void easy2d::Sprite::onRender()
{
	if (_image)
	{
		_image->draw(Rect(0, 0, _width, _height), _displayOpacity);
	}
}
