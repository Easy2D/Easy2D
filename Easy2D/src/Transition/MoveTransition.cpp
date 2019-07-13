#include <e2dtransition.h>
#include <e2dnode.h>

easy2d::MoveTransition::MoveTransition(double duration, Direction direction)
	: Transition(duration)
	, _direction(direction)
{
}

void easy2d::MoveTransition::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);

	double width = _windowSize.width;
	double height = _windowSize.height;
	if (_direction == Direction::Up)
	{
		_posDelta = Vector(0, -height);
		_startPos = Point(0, height);
	}
	else if (_direction == Direction::Down)
	{
		_posDelta = Vector(0, height);
		_startPos = Point(0, -height);
	}
	else if (_direction == Direction::Left)
	{
		_posDelta = Vector(-width, 0);
		_startPos = Point(width, 0);
	}
	else if (_direction == Direction::Right)
	{
		_posDelta = Vector(width, 0);
		_startPos = Point(-width, 0);
	}

	if (_outScene) _outScene->getRoot()->setPos(0, 0);
	_inScene->getRoot()->setPos(_startPos);
}

void easy2d::MoveTransition::_updateCustom()
{
	if (_outScene)
	{
		_outScene->getRoot()->setPos(_posDelta * _delta);
	}
	if (_inScene)
	{
		_inScene->getRoot()->setPos(_startPos + _posDelta * _delta);
	}

	if (_delta >= 1)
	{
		this->_stop();
	}
}

void easy2d::MoveTransition::_reset()
{
	if (_outScene) _outScene->getRoot()->setPos(0, 0);
	_inScene->getRoot()->setPos(0, 0);
}

