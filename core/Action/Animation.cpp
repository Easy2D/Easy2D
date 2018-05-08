#include "..\e2daction.h"

e2d::Animation::Animation() 
	: _frameIndex(0)
	, _interval(1)
{
}

e2d::Animation::Animation(double interval)
	: _frameIndex(0)
	, _interval(interval)
{
}

#ifdef HIGHER_THAN_VS2012

e2d::Animation::Animation(const std::initializer_list<Image*>& vImages)
	: _frameIndex(0)
	, _interval(1)
{
	this->add(vImages);
}

e2d::Animation::Animation(double interval, const std::initializer_list<Image*>& vImages)
	: _frameIndex(0)
	, _interval(interval)
{
	this->add(vImages);
}

#else

e2d::Animation::Animation(int number, Image * frame, ...)
	: _frameIndex(0)
	, _interval(1)
{
	Image ** ppImage = &frame;

	while (number > 0)
	{
		WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
		this->add(*ppImage);
		ppImage++;
		number--;
	}
}

e2d::Animation::Animation(double interval, int number, Image * frame, ...)
	: _frameIndex(0)
	, _interval(interval)
{
	Image ** ppImage = &frame;

	while (number > 0)
	{
		WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
		this->add(*ppImage);
		ppImage++;
		number--;
	}
}

#endif

e2d::Animation::~Animation()
{
}

void e2d::Animation::setInterval(double interval)
{
	_interval = max(interval, 0);
}

void e2d::Animation::_init()
{
	Action::_init();
}

void e2d::Animation::_update()
{
	Action::_update();

	if (_target == nullptr)
	{
		this->stop();
		return;
	}

	// �ж�ʱ�����Ƿ��㹻
	while ((Time::getTotalTime() - _last) >= _interval)
	{
		// ���¼�¼ʱ��
		_last += _interval;
		// ���عؼ�֡
		static_cast<Sprite*>(_target)->open(_frames[_frameIndex]);
		_frameIndex++;
		// �ж϶����Ƿ����
		if (_frameIndex == _frames.size())
		{
			this->stop();
			break;
		}
	}
}

void e2d::Animation::reset()
{
	Action::reset();
	_frameIndex = 0;
}

void e2d::Animation::onDestroy()
{
	Action::onDestroy();
	for (auto frame : _frames)
	{
		SafeRelease(&frame);
	}
}

void e2d::Animation::add(Image * frame)
{
	if (frame)
	{
		_frames.push_back(frame);
		frame->retain();
	}
}

#ifdef HIGHER_THAN_VS2012
void e2d::Animation::add(const std::initializer_list<Image*>& vImages)
{
	for (const auto &image : vImages)
	{
		this->add(image);
	}
}
#else
void e2d::Animation::add(int number, Image * frame, ...)
{
	Image ** ppImage = &frame;

	while (number > 0)
	{
		WARN_IF((*ppImage) == nullptr, "Animation NULL pointer exception!");
		this->add(*ppImage);
		ppImage++;
		number--;
	}
}
#endif

e2d::Animation * e2d::Animation::clone() const
{
	auto a = new Animation(_interval);
	for (auto frame : _frames)
	{
		a->add(frame);
	}
	return a;
}

e2d::Animation * e2d::Animation::reverse() const
{
	auto a = this->clone();
	a->_frames.reserve(_frames.size());
	return a;
}
