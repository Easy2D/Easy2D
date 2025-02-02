#include <easy2d/e2dtransition.h>
#include <easy2d/e2dnode.h>

easy2d::FadeTransition::FadeTransition(float duration, bool sequential)
	: Transition(duration)
	, _sequential(sequential)
{
}

void easy2d::FadeTransition::_init(Scene * prev, Scene * next)
{
	Transition::_init(prev, next);
	_outLayerParam.opacity = 1;
	_inLayerParam.opacity = 0;
}

void easy2d::FadeTransition::_updateCustom()
{
	if (_sequential)
	{
		if (_delta < 0.5)
		{
			_outLayerParam.opacity = 1 - float(_delta) * 2;
			_inLayerParam.opacity = 0;
		}
		else
		{
			_outLayerParam.opacity = 0;
			_inLayerParam.opacity = float(_delta - 0.5) * 2;
		}
	}
	else
	{
		_outLayerParam.opacity = float(1 - _delta);
		_inLayerParam.opacity = float(_delta);
	}

	if (_delta >= 1)
	{
		this->_stop();
	}
}

void easy2d::FadeTransition::_reset()
{
}
