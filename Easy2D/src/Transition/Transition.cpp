#include <easy2d/e2dbase.h>
#include <easy2d/e2dtransition.h>
#include <easy2d/e2dnode.h>

easy2d::Transition::Transition(float duration)
	: _end(false)
	, _last(0)
	, _delta(0)
	, _outScene(nullptr)
	, _inScene(nullptr)
	, _outLayer(nullptr)
	, _inLayer(nullptr)
	, _outLayerParam()
	, _inLayerParam()
{
	_duration = max(duration, 0);
}

easy2d::Transition::~Transition()
{
	SafeRelease(_outLayer);
	SafeRelease(_inLayer);

	GC::release(_outScene);
	GC::release(_inScene);
}

bool easy2d::Transition::isDone()
{
	return _end;
}

void easy2d::Transition::_init(Scene * prev, Scene * next)
{
	// ����ͼ��
	HRESULT hr = Renderer::getRenderTarget()->CreateLayer(&_inLayer);

	if (SUCCEEDED(hr))
	{
		hr = Renderer::getRenderTarget()->CreateLayer(&_outLayer);
	}

	if (FAILED(hr))
	{
		E2D_WARNING("�������ɶ���ͼ�㴴��ʧ��");
		return;
	}

	_last = Time::getTotalTime();
	_outScene = prev;
	_inScene = next;
	if (_outScene) _outScene->retain();
	if (_inScene) _inScene->retain();

	_windowSize = Window::getSize();
	_outLayerParam = _inLayerParam = D2D1::LayerParameters();
}

void easy2d::Transition::_update()
{
	// ���㶯������
	if (_duration == 0)
	{
		_delta = 1;
	}
	else
	{
		_delta = min((Time::getTotalTime() - _last) / _duration, 1);
	}

	this->_updateCustom();

	// ���³�������
	if (_outScene)
	{
		_outScene->_update();
	}
	if (_inScene)
	{
		_inScene->_update();
	}
}

void easy2d::Transition::_render()
{
	auto pRT = Renderer::getRenderTarget();

	if (_outScene)
	{
		Point rootPos = _outScene->getPos();
		auto clipRect = D2D1::RectF(
			float(max(rootPos.x, 0)),
			float(max(rootPos.y, 0)),
			float(min(rootPos.x + _windowSize.width, _windowSize.width)),
			float(min(rootPos.y + _windowSize.height, _windowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_outLayerParam, _outLayer);

		_outScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}

	if (_inScene)
	{
		Point rootPos = _inScene->getPos();
		auto clipRect = D2D1::RectF(
			float(max(rootPos.x, 0)),
			float(max(rootPos.y, 0)),
			float(min(rootPos.x + _windowSize.width, _windowSize.width)),
			float(min(rootPos.y + _windowSize.height, _windowSize.height))
		);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		pRT->PushLayer(_inLayerParam, _inLayer);

		_inScene->_render();

		pRT->PopLayer();
		pRT->PopAxisAlignedClip();
	}
}

void easy2d::Transition::_stop()
{
	_end = true;
	_reset();
}
