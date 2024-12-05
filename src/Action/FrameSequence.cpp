#include <easy2d/e2daction.h>

easy2d::FrameSequence::FrameSequence()
	: _interval(1)
{
}

easy2d::FrameSequence::FrameSequence(const std::vector<KeyFrame*>& frames)
	: _interval(1)
{
	this->add(frames);
}

easy2d::FrameSequence::FrameSequence(float interval)
	: _interval(interval)
{
}

easy2d::FrameSequence::FrameSequence(float interval, const std::vector<KeyFrame*>& frames)
	: _interval(interval)
{
	this->add(frames);
}

easy2d::FrameSequence::~FrameSequence()
{
	for (auto frame : _frames)
	{
		GC::release(frame);
	}
}

void easy2d::FrameSequence::setInterval(float interval)
{
	_interval = max(interval, 0);
}

void easy2d::FrameSequence::add(KeyFrame* frame)
{
	if (frame)
	{
		_frames.push_back(frame);
		frame->retain();
	}
}

void easy2d::FrameSequence::add(Image* image)
{
	add(gcnew KeyFrame(image));
}

void easy2d::FrameSequence::add(const std::vector<KeyFrame*>& frames)
{
	for (const auto &image : frames)
	{
		this->add(image);
	}
}

void easy2d::FrameSequence::add(const std::vector<Image*>& frames)
{
	for (const auto& image : frames)
	{
		this->add(image);
	}
}

float easy2d::FrameSequence::getInterval() const
{
	return _interval;
}

const std::vector<easy2d::KeyFrame*>& easy2d::FrameSequence::getFrames() const
{
	return _frames;
}

std::vector<easy2d::KeyFrame*>& easy2d::FrameSequence::getFrames()
{
	return _frames;
}

easy2d::FrameSequence * easy2d::FrameSequence::clone() const
{
	auto animation = gcnew FrameSequence(_interval);
	if (animation)
	{
		for (auto frame : _frames)
		{
			animation->add(frame);
		}
	}
	return animation;
}

easy2d::FrameSequence * easy2d::FrameSequence::reverse() const
{
	auto& oldFrames = this->getFrames();
	std::vector<KeyFrame*> frames(oldFrames.size());

	if (!oldFrames.empty())
	{
		for (auto iter = oldFrames.crbegin(),
			iterCrend = oldFrames.crend();
			iter != iterCrend;
			++iter)
		{
			KeyFrame* frame = *iter;
			if (frame)
			{
				frames.push_back(frame);
			}
		}
	}

	return gcnew FrameSequence(this->getInterval(), frames);
}
