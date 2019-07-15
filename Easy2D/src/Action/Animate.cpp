#include <e2daction.h>
#include <e2dnode.h>

easy2d::Animate::Animate() 
	: _frameIndex(0)
	, _animation(nullptr)
{
}

easy2d::Animate::Animate(Animation * animation)
	: _frameIndex(0)
	, _animation(nullptr)
{
	this->setAnimation(animation);
}

easy2d::Animate::~Animate()
{
	GC::release(_animation);
}

easy2d::Animation * easy2d::Animate::getAnimation() const
{
	return _animation;
}

void easy2d::Animate::setAnimation(Animation * animation)
{
	if (animation && animation != _animation)
	{
		GC::release(_animation);
		_animation = animation;
		_animation->retain();
	}
}

void easy2d::Animate::_init()
{
	Action::_init();

	auto target = dynamic_cast<Sprite*>(_target);
	if (target && _animation)
	{
		target->open(_animation->getFrames()[_frameIndex]);
		++_frameIndex;
	}
}

void easy2d::Animate::_update()
{
	Action::_update();

	if (!_animation)
	{
		this->stop();
		return;
	}

	while ((Time::getTotalTime() - _last) >= _animation->getInterval())
	{
		auto& frames = _animation->getFrames();
		auto target = dynamic_cast<Sprite*>(_target);

		if (target)
		{
			target->open(frames[_frameIndex]);
		}

		_last += _animation->getInterval();
		++_frameIndex;

		if (_frameIndex == frames.size())
		{
			this->stop();
			break;
		}
	}
}

void easy2d::Animate::_resetTime()
{
	Action::_resetTime();
	_last = Time::getTotalTime();
}

void easy2d::Animate::reset()
{
	Action::reset();
	_frameIndex = 0;
}

easy2d::Animate * easy2d::Animate::clone() const
{
	if (_animation)
	{
		return gcnew Animate(_animation);
	}
	return nullptr;
}

easy2d::Animate * easy2d::Animate::reverse() const
{
	if (_animation)
	{
		auto animation = _animation->reverse();
		if (animation)
		{
			return gcnew Animate(animation);
		}
	}
	return nullptr;
}
