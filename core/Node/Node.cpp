#include "..\e2dnode.h"
#include "..\e2dmanager.h"
#include "..\e2daction.h"
#include <algorithm>

// Ĭ�����ĵ�λ��
static float s_fDefaultPiovtX = 0;
static float s_fDefaultPiovtY = 0;
static bool s_fDefaultColliderEnabled = true;

e2d::Node::Node()
	: _nOrder(0)
	, _posX(0)
	, _posY(0)
	, _width(0)
	, _height(0)
	, _scaleX(1.0f)
	, _scaleY(1.0f)
	, _rotation(0)
	, _skewAngleX(0)
	, _skewAngleY(0)
	, _displayOpacity(1.0f)
	, _realOpacity(1.0f)
	, _pivotX(s_fDefaultPiovtX)
	, _pivotY(s_fDefaultPiovtY)
	, _initialMatri(D2D1::Matrix3x2F::Identity())
	, _finalMatri(D2D1::Matrix3x2F::Identity())
	, _visiable(true)
	, _collider(nullptr)
	, _parent(nullptr)
	, _parentScene(nullptr)
	, _hashName(0)
	, _needSort(false)
	, _needTransform(false)
	, _autoUpdate(true)
	, _positionFixed(false)
{
	if (s_fDefaultColliderEnabled)
	{
		this->setCollider(Create<ColliderRect>(this));
	}
}

e2d::Node * e2d::Node::create()
{
	return Create<Node>();
}

e2d::Node::~Node()
{
}

void e2d::Node::_update()
{
	if (_needTransform)
	{
		_updateTransform();
	}

	if (!_children.empty())
	{
		if (_needSort)
		{
			// �ӽڵ�����
			auto sortFunc = [](Node * n1, Node * n2) {
				return n1->getOrder() < n2->getOrder();
			};

			std::sort(
				std::begin(_children),
				std::end(_children),
				sortFunc
			);

			_needSort = false;
		}

		// �����ӽڵ�
		size_t size = _children.size();
		size_t i;
		for (i = 0; i < size; ++i)
		{
			auto child = _children[i];
			// ���� Order С����Ľڵ�
			if (child->getOrder() < 0)
			{
				child->_update();
			}
			else
			{
				break;
			}
		}

		if (_autoUpdate)
		{
			if (!Game::isPaused())
			{
				this->onUpdate();
			}
			this->onFixedUpdate();
		}

		// ����ʣ��ڵ�
		for (; i < size; ++i)
			_children[i]->_update();
	}
	else
	{
		if (_autoUpdate)
		{
			if (!Game::isPaused())
			{
				this->onUpdate();
			}
			this->onFixedUpdate();
		}
	}
}

void e2d::Node::_render()
{
	if (!_visiable)
	{
		return;
	}

	if (!_children.empty())
	{
		size_t size = _children.size();
		size_t i;
		for (i = 0; i < size; ++i)
		{
			auto child = _children[i];
			// ���� Order С����Ľڵ�
			if (child->getOrder() < 0)
			{
				child->_render();
			}
			else
			{
				break;
			}
		}

		// ת����Ⱦ���Ķ�ά����
		Renderer::getRenderTarget()->SetTransform(_finalMatri);
		// ��Ⱦ����
		this->onRender();

		// ����ʣ��ڵ�
		for (; i < size; ++i)
			_children[i]->_render();
	}
	else
	{
		// ת����Ⱦ���Ķ�ά����
		Renderer::getRenderTarget()->SetTransform(_finalMatri);
		// ��Ⱦ����
		this->onRender();
	}
}

void e2d::Node::_drawCollider()
{
	// ��������ļ�����ײ��
	if (_collider && _collider->_visiable)
	{
		_collider->_render();
	}

	// ���������ӽڵ�ļ�����ײ��
	for (auto child : _children)
	{
		child->_drawCollider();
	}
}

void e2d::Node::_updateSelfTransform()
{
	// �������ĵ�����
	D2D1_POINT_2F pivot = { _width * _pivotX, _height * _pivotY };
	// �任 Initial �����ӽڵ㽫�������������б任
	_initialMatri = D2D1::Matrix3x2F::Scale(
		_scaleX,
		_scaleY,
		pivot
	) * D2D1::Matrix3x2F::Skew(
		_skewAngleX,
		_skewAngleY,
		pivot
	) * D2D1::Matrix3x2F::Rotation(
		_rotation,
		pivot
	) * D2D1::Matrix3x2F::Translation(
		_posX,
		_posY
	);
	// �����������ĵ�任 Final ����
	_finalMatri = _initialMatri * D2D1::Matrix3x2F::Translation(-pivot.x, -pivot.y);
	// �͸��ڵ�������
	if (!_positionFixed && _parent)
	{
		_initialMatri = _initialMatri * _parent->_initialMatri;
		_finalMatri = _finalMatri * _parent->_initialMatri;
	}
}

void e2d::Node::_updateTransform()
{
	// ���������ת������
	_updateSelfTransform();
	// �����������ײ��Ҳ������Ӧת��
	if (_collider)
	{
		_collider->_transform();
	}
	// ��־��ִ�й��任
	_needTransform = false;
	// �����ӽڵ��µ����нڵ�
	for (auto child : this->_children)
	{
		child->_updateTransform();
	}
}

void e2d::Node::_updateOpacity()
{
	if (_parent)
	{
		_displayOpacity = _realOpacity * _parent->_displayOpacity;
	}
	for (auto child : _children)
	{
		child->_updateOpacity();
	}
}

bool e2d::Node::isVisiable() const
{
	return _visiable;
}

e2d::String e2d::Node::getName() const
{
	return _name;
}

unsigned int e2d::Node::getHashName() const
{
	return _hashName;
}

double e2d::Node::getPosX() const
{
	return _posX;
}

double e2d::Node::getPosY() const
{
	return _posY;
}

e2d::Point e2d::Node::getPos() const
{
	return Point(_posX, _posY);
}

double e2d::Node::getWidth() const
{
	return _width * _scaleX;
}

double e2d::Node::getHeight() const
{
	return _height * _scaleY;
}

double e2d::Node::getRealWidth() const
{
	return _width;
}

double e2d::Node::getRealHeight() const
{
	return _height;
}

e2d::Size e2d::Node::getRealSize() const
{
	return Size(_width, _height);
}

double e2d::Node::getPivotX() const
{
	return _pivotX;
}

double e2d::Node::getPivotY() const
{
	return _pivotY;
}

e2d::Size e2d::Node::getSize() const
{
	return Size(getWidth(), getHeight());
}

double e2d::Node::getScaleX() const
{
	return _scaleX;
}

double e2d::Node::getScaleY() const
{
	return _scaleY;
}

double e2d::Node::getSkewX() const
{
	return _skewAngleX;
}

double e2d::Node::getSkewY() const
{
	return _skewAngleY;
}

double e2d::Node::getRotation() const
{
	return _rotation;
}

double e2d::Node::getOpacity() const
{
	return _realOpacity;
}

e2d::Node::Property e2d::Node::getProperty() const
{
	Property prop;
	prop.visable = _visiable;
	prop.posX = _posX;
	prop.posY = _posY;
	prop.width = _width;
	prop.height = _height;
	prop.opacity = _realOpacity;
	prop.pivotX = _pivotX;
	prop.pivotY = _pivotY;
	prop.scaleX = _scaleX;
	prop.scaleY = _scaleY;
	prop.rotation = _rotation;
	prop.skewAngleX = _skewAngleX;
	prop.skewAngleY = _skewAngleY;
	return prop;
}

e2d::Collider * e2d::Node::getCollider() const
{
	return _collider;
}

int e2d::Node::getOrder() const
{
	return _nOrder;
}

void e2d::Node::setOrder(int order)
{
	_nOrder = order;
}

void e2d::Node::setPosX(double x)
{
	this->setPos(x, _posY);
}

void e2d::Node::setPosY(double y)
{
	this->setPos(_posX, y);
}

void e2d::Node::setPos(const Point & p)
{
	this->setPos(p.x, p.y);
}

void e2d::Node::setPos(double x, double y)
{
	if (_posX == x && _posY == y)
		return;

	_posX = float(x);
	_posY = float(y);
	_needTransform = true;
}

void e2d::Node::setPosFixed(bool fixed)
{
	if (_positionFixed == fixed)
		return;

	_positionFixed = fixed;
	_needTransform = true;
}

void e2d::Node::movePosX(double x)
{
	this->movePos(x, 0);
}

void e2d::Node::movePosY(double y)
{
	this->movePos(0, y);
}

void e2d::Node::movePos(double x, double y)
{
	this->setPos(_posX + x, _posY + y);
}

void e2d::Node::movePos(const Vector & v)
{
	this->movePos(v.x, v.y);
}

void e2d::Node::setScaleX(double scaleX)
{
	this->setScale(scaleX, _scaleY);
}

void e2d::Node::setScaleY(double scaleY)
{
	this->setScale(_scaleX, scaleY);
}

void e2d::Node::setScale(double scale)
{
	this->setScale(scale, scale);
}

void e2d::Node::setScale(double scaleX, double scaleY)
{
	if (_scaleX == scaleX && _scaleY == scaleY)
		return;

	_scaleX = float(scaleX);
	_scaleY = float(scaleY);
	_needTransform = true;
}

void e2d::Node::setSkewX(double angleX)
{
	this->setSkew(angleX, _skewAngleY);
}

void e2d::Node::setSkewY(double angleY)
{
	this->setSkew(_skewAngleX, angleY);
}

void e2d::Node::setSkew(double angleX, double angleY)
{
	if (_skewAngleX == angleX && _skewAngleY == angleY)
		return;

	_skewAngleX = float(angleX);
	_skewAngleY = float(angleY);
	_needTransform = true;
}

void e2d::Node::setRotation(double angle)
{
	if (_rotation == angle)
		return;

	_rotation = float(angle);
	_needTransform = true;
}

void e2d::Node::setOpacity(double opacity)
{
	if (_realOpacity == opacity)
		return;

	_displayOpacity = _realOpacity = min(max(float(opacity), 0), 1);
	// ���½ڵ�͸����
	_updateOpacity();
}

void e2d::Node::setPivotX(double pivotX)
{
	this->setPivot(pivotX, _pivotY);
}

void e2d::Node::setPivotY(double pivotY)
{
	this->setPivot(_pivotX, pivotY);
}

void e2d::Node::setPivot(double pivotX, double pivotY)
{
	if (_pivotX == pivotX && _pivotY == pivotY)
		return;

	_pivotX = min(max(float(pivotX), 0), 1);
	_pivotY = min(max(float(pivotY), 0), 1);
	_needTransform = true;
}

void e2d::Node::setWidth(double width)
{
	this->setSize(width, _height);
}

void e2d::Node::setHeight(double height)
{
	this->setSize(_width, height);
}

void e2d::Node::setSize(double width, double height)
{
	if (_width == width && _height == height)
		return;

	_width = float(width);
	_height = float(height);
	_needTransform = true;
}

void e2d::Node::setSize(Size size)
{
	this->setSize(size.width, size.height);
}

void e2d::Node::setProperty(Property prop)
{
	this->setVisiable(prop.visable);
	this->setPos(prop.posX, prop.posY);
	this->setSize(prop.width, prop.height);
	this->setOpacity(prop.opacity);
	this->setPivot(prop.pivotX, prop.pivotY);
	this->setScale(prop.scaleX, prop.scaleY);
	this->setRotation(prop.rotation);
	this->setSkew(prop.skewAngleX, prop.skewAngleY);
}

void e2d::Node::setCollider(Collider::Type type)
{
	switch (type)
	{
	case Collider::Type::RECT:
	{
		this->setCollider(Create<ColliderRect>(this));
		break;
	}

	case Collider::Type::CIRCLE:
	{
		this->setCollider(Create<ColliderCircle>(this));
		break;
	}

	case Collider::Type::ELLIPSE:
	{
		this->setCollider(Create<ColliderEllipse>(this));
		break;
	}

	default:
		break;
	}
}

void e2d::Node::setCollider(Collider * pCollider)
{
	// ɾ���ɵ���ײ��
	ColliderManager::__removeCollider(_collider);
	// ����µ���ײ��
	ColliderManager::__addCollider(pCollider);

	if (pCollider)
	{
		// ˫���
		this->_collider = pCollider;
		pCollider->_parentNode = this;
	}
	else
	{
		this->_collider = nullptr;
	}
}

void e2d::Node::addColliableName(const String& collliderName)
{
	unsigned int hash = collliderName.getHashCode();
	_colliders.insert(hash);
}

void e2d::Node::addColliableName(const std::vector<String>& colliderNames)
{
	for (const auto &name : colliderNames)
	{
		this->addColliableName(name);
	}
}

void e2d::Node::removeColliableName(const String& collliderName)
{
	unsigned int hash = collliderName.getHashCode();
	_colliders.erase(hash);
}

void e2d::Node::addChild(Node * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "Node::addChild NULL pointer exception.");

	if (child)
	{
		ASSERT(child->_parent == nullptr, "Node already added. It can't be added again!");

		for (Node * parent = this; parent != nullptr; parent = parent->getParent())
		{
			ASSERT(child != parent, "A Node cannot be the child of his own children!");
		}

		_children.push_back(child);

		child->setOrder(order);

		child->retain();

		child->_parent = this;

		if (this->_parentScene)
		{
			child->_setParentScene(this->_parentScene);
		}

		// �����ӽڵ�͸����
		child->_updateOpacity();
		// ���½ڵ�ת��
		child->_needTransform = true;
		// �����ӽڵ�����
		_needSort = true;
	}
}

void e2d::Node::addChild(const std::vector<Node*>& nodes, int order)
{
	for (auto node : nodes)
	{
		this->addChild(node, order);
	}
}

e2d::Node * e2d::Node::getParent() const
{
	return _parent;
}

e2d::Scene * e2d::Node::getParentScene() const
{
	return _parentScene;
}

std::vector<e2d::Node*> e2d::Node::getChildren(const String& name) const
{
	std::vector<Node*> vChildren;
	unsigned int hash = name.getHashCode();

	for (auto child : _children)
	{
		// ��ͬ�����ƿ��ܻ�����ͬ�� Hash ֵ�������ȱȽ� Hash �������������ٶ�
		if (child->_hashName == hash && child->_name == name)
		{
			vChildren.push_back(child);
		}
	}
	return std::move(vChildren);
}

e2d::Node * e2d::Node::getChild(const String& name) const
{
	unsigned int hash = name.getHashCode();

	for (auto child : _children)
	{
		// ��ͬ�����ƿ��ܻ�����ͬ�� Hash ֵ�������ȱȽ� Hash �������������ٶ�
		if (child->_hashName == hash && child->_name == name)
		{
			return child;
		}
	}
	return nullptr;
}

const std::vector<e2d::Node*>& e2d::Node::getAllChildren() const
{
	return _children;
}

int e2d::Node::getChildrenCount() const
{
	return static_cast<int>(_children.size());
}

void e2d::Node::removeFromParent()
{
	if (_parent)
	{
		_parent->removeChild(this);
	}
}

bool e2d::Node::removeChild(Node * child)
{
	WARN_IF(child == nullptr, "Node::removeChildren NULL pointer exception.");

	if (_children.empty())
	{
		return false;
	}

	if (child)
	{
		size_t size = _children.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (_children[i] == child)
			{
				_children.erase(_children.begin() + i);
				child->_parent = nullptr;

				if (child->_parentScene)
				{
					child->_setParentScene(nullptr);
				}

				child->release();
				return true;
			}
		}
	}
	return false;
}

void e2d::Node::removeChildren(const String& childName)
{
	WARN_IF(childName.isEmpty(), "Invalid Node name.");

	if (_children.empty())
	{
		return;
	}

	// �������� Hash ֵ
	unsigned int hash = childName.getHashCode();

	size_t size = _children.size();
	for (size_t i = 0; i < size; ++i)
	{
		auto child = _children[i];
		if (child->_hashName == hash && child->_name == childName)
		{
			_children.erase(_children.begin() + i);
			child->_parent = nullptr;
			if (child->_parentScene)
			{
				child->_setParentScene(nullptr);
			}
			child->release();
		}
	}
}

void e2d::Node::clearAllChildren()
{
	// ���нڵ�����ü�����һ
	for (auto child : _children)
	{
		child->release();
	}
	// ��մ���ڵ������
	_children.clear();
}

void e2d::Node::runAction(Action * action)
{
	if (this != action->getTarget())
	{
		ASSERT(action->getTarget() == nullptr, "The action has already got a target!");
		ActionManager::start(action, this, false);
	}
	else
	{
		action->reset();
	}
}

void e2d::Node::resumeAction(const String& strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->resume();
		}
	}
}

void e2d::Node::pauseAction(const String& strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->pause();
		}
	}
}

void e2d::Node::stopAction(const String& strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->stop();
		}
	}
}

e2d::Action * e2d::Node::getAction(const String& strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			return action;
		}
	}
	return nullptr;
}

std::vector<e2d::Action*> e2d::Node::getActions(const String& strActionName)
{
	std::vector<Action*>::iterator iter;
	auto actions = ActionManager::get(strActionName);
	for (iter = actions.begin(); iter != actions.end();)
	{
		if ((*iter)->getTarget() != this)
		{
			iter = actions.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	return std::move(actions);
}

bool e2d::Node::isPointIn(Point point) const
{
	BOOL ret = 0;
	// ���������ײ�壬����ײ���ж�
	if (_collider)
	{
		_collider->getD2dGeometry()->FillContainsPoint(
			D2D1::Point2F(
				float(point.x),
				float(point.y)),
			_finalMatri,
			&ret
		);
	}
	else
	{
		// Ϊ�ڵ㴴��һ����ʱ��ײ��
		ID2D1RectangleGeometry * rect;
		Renderer::getID2D1Factory()->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _width, _height),
			&rect
		);
		// �жϵ��Ƿ�����ײ����
		rect->FillContainsPoint(
			D2D1::Point2F(
				float(point.x),
				float(point.y)),
			_finalMatri,
			&ret
		);
		// ɾ����ʱ��������ײ��
		SafeReleaseInterface(rect);
	}

	if (ret)
	{
		return true;
	}

	return false;
}

bool e2d::Node::isIntersectWith(const Node * node) const
{
	// ���������ײ�壬����ײ���ж�
	if (this->_collider && node->_collider)
	{
		Collider::Relation relation = this->_collider->getRelationWith(node->_collider);
		if ((relation != Collider::Relation::UNKNOWN) &&
			(relation != Collider::Relation::DISJOINT))
		{
			return true;
		}
	}
	else
	{
		// Ϊ�ڵ㴴��һ����ʱ��ײ��
		ID2D1RectangleGeometry * pRect1;
		ID2D1RectangleGeometry * pRect2;
		ID2D1TransformedGeometry * pCollider;
		D2D1_GEOMETRY_RELATION relation;

		// ���������Сλ�ô�������
		Renderer::getID2D1Factory()->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _width, _height),
			&pRect1
		);
		// ���ݶ�ά�������ת��
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			pRect1,
			_finalMatri,
			&pCollider
		);
		// ������ȽϽڵ�Ĵ�Сλ�ô�������
		Renderer::getID2D1Factory()->CreateRectangleGeometry(
			D2D1::RectF(0, 0, node->_width, node->_height),
			&pRect2
		);
		// ��ȡ�ཻ״̬
		pCollider->CompareWithGeometry(
			pRect2,
			node->_finalMatri,
			&relation
		);
		// ɾ����ʱ��������ײ��
		SafeReleaseInterface(pRect1);
		SafeReleaseInterface(pRect2);
		SafeReleaseInterface(pCollider);
		if ((relation != D2D1_GEOMETRY_RELATION_UNKNOWN) &&
			(relation != D2D1_GEOMETRY_RELATION_DISJOINT))
		{
			return true;
		}
	}

	return false;
}

void e2d::Node::setAutoUpdate(bool bAutoUpdate)
{
	_autoUpdate = bAutoUpdate;
}

void e2d::Node::setDefaultPiovt(double defaultPiovtX, double defaultPiovtY)
{
	s_fDefaultPiovtX = min(max(float(defaultPiovtX), 0), 1);
	s_fDefaultPiovtY = min(max(float(defaultPiovtY), 0), 1);
}

void e2d::Node::setDefaultColliderEnable(bool enable)
{
	s_fDefaultColliderEnabled = enable;
}

void e2d::Node::onDestroy()
{
	ActionManager::__clearAllBindedWith(this);
	ColliderManager::__removeCollider(_collider);
	for (auto child : _children)
	{
		SafeRelease(child);
	}
}

void e2d::Node::resumeAllActions()
{
	ActionManager::__resumeAllBindedWith(this);
}

void e2d::Node::pauseAllActions()
{
	ActionManager::__pauseAllBindedWith(this);
}

void e2d::Node::stopAllActions()
{
	ActionManager::__stopAllBindedWith(this);
}

void e2d::Node::setVisiable(bool value)
{
	_visiable = value;
}

void e2d::Node::setName(const String& name)
{
	WARN_IF(name.isEmpty(), "Invalid Node name.");

	if (!name.isEmpty() && _name != name)
	{
		// ����ڵ���
		_name = name;
		// ����ڵ� Hash ��
		_hashName = name.getHashCode();
	}
}

void e2d::Node::_setParentScene(Scene * scene)
{
	_parentScene = scene;
	for (auto child : _children)
	{
		child->_setParentScene(scene);
	}
}