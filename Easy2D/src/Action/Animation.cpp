#include <easy2d/e2daction.h>
#include <easy2d/e2dnode.h>

easy2d::Animation::Animation() 
	: _frameIndex(0)
	, _seq(nullptr)
{
}

easy2d::Animation::Animation(FrameSequence * seq)
	: _frameIndex(0)
	, _seq(seq)
{
	GC::retain(_seq);
}

easy2d::Animation::~Animation()
{
	GC::release(_seq);
}

easy2d::FrameSequence * easy2d::Animation::getFrameSequence() const
{
	return _seq;
}

void easy2d::Animation::setFrameSequence(FrameSequence * seq)
{
	if (seq && seq != _seq)
	{
		GC::release(_seq);
		_seq = seq;
		_seq->retain();
	}
}

void easy2d::Animation::_init()
{
	Action::_init();

	auto target = dynamic_cast<Sprite*>(_target);
	if (target && _seq)
	{
		target->setKeyFrame(_seq->getFrames()[_frameIndex]);
		++_frameIndex;
	}
}

void easy2d::Animation::_update()
{
	Action::_update();

	if (!_seq)
	{
		this->stop();
		return;
	}

	while ((Time::getTotalTime() - _last) >= _seq->getInterval())
	{
		auto& frames = _seq->getFrames();
		auto target = dynamic_cast<Sprite*>(_target);

		if (target)
		{
			target->setKeyFrame(frames[_frameIndex]);
		}

		_last += _seq->getInterval();
		++_frameIndex;

		if (_frameIndex == frames.size())
		{
			this->stop();
			break;
		}
	}
}

void easy2d::Animation::_resetTime()
{
	Action::_resetTime();
	_last = Time::getTotalTime();
}

void easy2d::Animation::reset()
{
	Action::reset();
	_frameIndex = 0;
}

easy2d::Animation * easy2d::Animation::clone() const
{
	if (_seq)
	{
		return gcnew Animation(_seq);
	}
	return nullptr;
}

easy2d::Animation * easy2d::Animation::reverse() const
{
	if (_seq)
	{
		auto animation = _seq->reverse();
		if (animation)
		{
			return gcnew Animation(animation);
		}
	}
	return nullptr;
}
