#include <e2dcollider.h>
#include <e2dnode.h>

easy2d::RectCollider::RectCollider()
	: _d2dRectangle(nullptr)
{
}

easy2d::RectCollider::RectCollider(double x, double y, double width, double height)
	: _d2dRectangle(nullptr)
{
	this->setRect(x, y, x + width, y + height);
}

easy2d::RectCollider::RectCollider(Node * node)
	: _d2dRectangle(nullptr)
{
	this->setRect(0, 0, node->getRealWidth(), node->getRealHeight());
	this->setAutoResize(true);
}

easy2d::RectCollider::~RectCollider()
{
	SafeRelease(_d2dRectangle);
}

void easy2d::RectCollider::setRect(double left, double top, double right, double bottom)
{
	SafeRelease(_d2dRectangle);

	Renderer::getID2D1Factory()->CreateRectangleGeometry(
		D2D1::RectF(
			float(left), 
			float(top), 
			float(right), 
			float(bottom)),
		&_d2dRectangle
	);
}

void easy2d::RectCollider::_resize()
{
	if (_parentNode && _enable)
	{
		this->setRect(0, 0, _parentNode->getRealWidth(), _parentNode->getRealHeight());
	}
}

ID2D1RectangleGeometry * easy2d::RectCollider::getD2dGeometry() const
{
	return _d2dRectangle;
}
