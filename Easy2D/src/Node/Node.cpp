#include <e2dnode.h>
#include <e2dmanager.h>
#include <e2daction.h>
#include <algorithm>

// 默认中心点位置
static float s_fDefaultPiovtX = 0;
static float s_fDefaultPiovtY = 0;

easy2d::Node::Node()
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
	, _transform()
	, _visiable(true)
	, _parent(nullptr)
	, _parentScene(nullptr)
	, _hashName(0)
	, _needSort(false)
	, _needTransform(false)
	, _autoUpdate(true)
	, _positionFixed(false)
{
}

easy2d::Node::~Node()
{
	ActionManager::__clearAllBindedWith(this);
	for (auto child : _children)
	{
		child->_parent = nullptr;
		GC::release(child);
	}
}

void easy2d::Node::_update()
{
	// 更新转换矩阵
	_updateTransform();

	if (_children.empty())
	{
		if (_autoUpdate && !Game::isPaused())
		{
			this->onUpdate();
		}
	}
	else
	{
		// 子节点排序
		_sortChildren();

		// 遍历子节点
		size_t size = _children.size();
		size_t i;
		for (i = 0; i < size; ++i)
		{
			auto child = _children[i];
			// 访问 Order 小于零的节点
			if (child->getOrder() < 0)
			{
				child->_update();
			}
			else
			{
				break;
			}
		}

		if (_autoUpdate && !Game::isPaused())
		{
			this->onUpdate();
		}

		// 访问其他节点
		for (; i < size; ++i)
			_children[i]->_update();
	}
}

void easy2d::Node::_render()
{
	if (!_visiable)
	{
		return;
	}

	// 更新转换矩阵
	_updateTransform();

	if (_children.empty())
	{
		// 转换渲染器的二维矩阵
		Renderer::getRenderTarget()->SetTransform(_transform.toD2DMatrix());
		// 渲染自身
		this->onRender();
	}
	else
	{
		// 子节点排序
		_sortChildren();

		size_t size = _children.size();
		size_t i;
		for (i = 0; i < size; ++i)
		{
			auto child = _children[i];
			// 访问 Order 小于零的节点
			if (child->getOrder() < 0)
			{
				child->_render();
			}
			else
			{
				break;
			}
		}

		// 转换渲染器的二维矩阵
		Renderer::getRenderTarget()->SetTransform(_transform.toD2DMatrix());
		// 渲染自身
		this->onRender();

		// 访问剩余节点
		for (; i < size; ++i)
			_children[i]->_render();
	}
}

void easy2d::Node::_drawCollider()
{
	// 绘制所有子节点的几何碰撞体
	for (auto child : _children)
	{
		child->_drawCollider();
	}
}

void easy2d::Node::_updateTransform() const
{
	if (!_needTransform)
		return;

	_transform = Matrix32::scaling(_scaleX, _scaleY)
		* Matrix32::skewing(_skewAngleX, _skewAngleY)
		* Matrix32::rotation(_rotation)
		* Matrix32::translation(_posX, _posY);

	_transform.translate(-_width * _pivotX, -_height * _pivotY);

	if (_parent)
	{
		_transform = _transform * _parent->_transform;
	}

	// 标志已执行过变换
	_needTransform = false;

	// 通知子节点进行转换
	for (auto& child : _children)
	{
		child->_needTransform = true;
	}
}

void easy2d::Node::_sortChildren()
{
	if (_needSort)
	{
		std::sort(
			std::begin(_children),
			std::end(_children),
			[](Node * n1, Node * n2) { return n1->getOrder() < n2->getOrder(); }
		);

		_needSort = false;
	}
}

void easy2d::Node::_updateOpacity()
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

bool easy2d::Node::isVisiable() const
{
	return _visiable;
}

easy2d::String easy2d::Node::getName() const
{
	return _name;
}

size_t easy2d::Node::getHashName() const
{
	return _hashName;
}

float easy2d::Node::getPosX() const
{
	return _posX;
}

float easy2d::Node::getPosY() const
{
	return _posY;
}

easy2d::Point easy2d::Node::getPos() const
{
	return Point(_posX, _posY);
}

float easy2d::Node::getWidth() const
{
	return _width * _scaleX;
}

float easy2d::Node::getHeight() const
{
	return _height * _scaleY;
}

float easy2d::Node::getRealWidth() const
{
	return _width;
}

float easy2d::Node::getRealHeight() const
{
	return _height;
}

easy2d::Size easy2d::Node::getRealSize() const
{
	return Size(_width, _height);
}

float easy2d::Node::getPivotX() const
{
	return _pivotX;
}

float easy2d::Node::getPivotY() const
{
	return _pivotY;
}

easy2d::Size easy2d::Node::getSize() const
{
	return Size(getWidth(), getHeight());
}

float easy2d::Node::getScaleX() const
{
	return _scaleX;
}

float easy2d::Node::getScaleY() const
{
	return _scaleY;
}

float easy2d::Node::getSkewX() const
{
	return _skewAngleX;
}

float easy2d::Node::getSkewY() const
{
	return _skewAngleY;
}

float easy2d::Node::getRotation() const
{
	return _rotation;
}

float easy2d::Node::getOpacity() const
{
	return _realOpacity;
}

easy2d::Node::Property easy2d::Node::getProperty() const
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

easy2d::Rect easy2d::Node::getBounds() const
{
	return Rect(Point{}, Size(_width, _height));
}

easy2d::Rect easy2d::Node::getBoundingBox() const
{
	return getTransform().transform(getBounds());
}

int easy2d::Node::getOrder() const
{
	return _nOrder;
}

void easy2d::Node::setOrder(int order)
{
	_nOrder = order;
}

void easy2d::Node::setPosX(float x)
{
	this->setPos(x, _posY);
}

void easy2d::Node::setPosY(float y)
{
	this->setPos(_posX, y);
}

void easy2d::Node::setPos(const Point & p)
{
	this->setPos(p.x, p.y);
}

void easy2d::Node::setPos(float x, float y)
{
	if (_posX == x && _posY == y)
		return;

	_posX = float(x);
	_posY = float(y);
	_needTransform = true;
}

void easy2d::Node::setPosFixed(bool fixed)
{
	if (_positionFixed == fixed)
		return;

	_positionFixed = fixed;
	_needTransform = true;
}

void easy2d::Node::movePosX(float x)
{
	this->movePos(x, 0);
}

void easy2d::Node::movePosY(float y)
{
	this->movePos(0, y);
}

void easy2d::Node::movePos(float x, float y)
{
	this->setPos(_posX + x, _posY + y);
}

void easy2d::Node::movePos(const Vector2 & v)
{
	this->movePos(v.x, v.y);
}

void easy2d::Node::setScaleX(float scaleX)
{
	this->setScale(scaleX, _scaleY);
}

void easy2d::Node::setScaleY(float scaleY)
{
	this->setScale(_scaleX, scaleY);
}

void easy2d::Node::setScale(float scale)
{
	this->setScale(scale, scale);
}

void easy2d::Node::setScale(float scaleX, float scaleY)
{
	if (_scaleX == scaleX && _scaleY == scaleY)
		return;

	_scaleX = float(scaleX);
	_scaleY = float(scaleY);
	_needTransform = true;
}

void easy2d::Node::setSkewX(float angleX)
{
	this->setSkew(angleX, _skewAngleY);
}

void easy2d::Node::setSkewY(float angleY)
{
	this->setSkew(_skewAngleX, angleY);
}

void easy2d::Node::setSkew(float angleX, float angleY)
{
	if (_skewAngleX == angleX && _skewAngleY == angleY)
		return;

	_skewAngleX = float(angleX);
	_skewAngleY = float(angleY);
	_needTransform = true;
}

void easy2d::Node::setRotation(float angle)
{
	if (_rotation == angle)
		return;

	_rotation = float(angle);
	_needTransform = true;
}

void easy2d::Node::setOpacity(float opacity)
{
	if (_realOpacity == opacity)
		return;

	_displayOpacity = _realOpacity = min(max(float(opacity), 0), 1);
	// 更新节点透明度
	_updateOpacity();
}

void easy2d::Node::setPivotX(float pivotX)
{
	this->setPivot(pivotX, _pivotY);
}

void easy2d::Node::setPivotY(float pivotY)
{
	this->setPivot(_pivotX, pivotY);
}

void easy2d::Node::setPivot(float pivotX, float pivotY)
{
	if (_pivotX == pivotX && _pivotY == pivotY)
		return;

	_pivotX = min(max(float(pivotX), 0), 1);
	_pivotY = min(max(float(pivotY), 0), 1);
	_needTransform = true;
}

void easy2d::Node::setWidth(float width)
{
	this->setSize(width, _height);
}

void easy2d::Node::setHeight(float height)
{
	this->setSize(_width, height);
}

void easy2d::Node::setSize(float width, float height)
{
	if (_width == width && _height == height)
		return;

	_width = float(width);
	_height = float(height);
	_needTransform = true;
}

void easy2d::Node::setSize(Size size)
{
	this->setSize(size.width, size.height);
}

void easy2d::Node::setProperty(Property prop)
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

void easy2d::Node::addChild(Node * child, int order  /* = 0 */)
{
	if (child == nullptr) E2D_WARNING(L"Node::addChild NULL pointer exception.");

	if (child)
	{
		if (child->_parent != nullptr)
		{
			E2D_WARNING(L"节点已有父节点, 不能再添加到其他节点");
			return;
		}

		for (Node * parent = this; parent != nullptr; parent = parent->getParent())
		{
			if (child == parent)
			{
				E2D_WARNING(L"一个节点不能同时是另一个节点的父节点和子节点");
				return;
			}
		}

		_children.push_back(child);

		child->setOrder(order);

		child->retain();

		child->_parent = this;

		if (this->_parentScene)
		{
			child->_setParentScene(this->_parentScene);
		}

		// 更新子节点透明度
		child->_updateOpacity();
		// 更新节点转换
		child->_needTransform = true;
		// 更新子节点排序
		_needSort = true;
	}
}

void easy2d::Node::addChild(const std::vector<Node*>& nodes, int order)
{
	for (auto node : nodes)
	{
		this->addChild(node, order);
	}
}

easy2d::Matrix32 easy2d::Node::getTransform() const
{
	_updateTransform();
	return _transform;
}

easy2d::Node * easy2d::Node::getParent() const
{
	return _parent;
}

easy2d::Scene * easy2d::Node::getParentScene() const
{
	return _parentScene;
}

std::vector<easy2d::Node*> easy2d::Node::getChildren(const String& name) const
{
	std::vector<Node*> vChildren;
	size_t hash = std::hash<String>{}(name);

	for (auto child : _children)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->_hashName == hash && child->_name == name)
		{
			vChildren.push_back(child);
		}
	}
	return std::move(vChildren);
}

easy2d::Node * easy2d::Node::getChild(const String& name) const
{
	size_t hash = std::hash<String>{}(name);

	for (auto child : _children)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->_hashName == hash && child->_name == name)
		{
			return child;
		}
	}
	return nullptr;
}

const std::vector<easy2d::Node*>& easy2d::Node::getAllChildren() const
{
	return _children;
}

int easy2d::Node::getChildrenCount() const
{
	return static_cast<int>(_children.size());
}

void easy2d::Node::removeFromParent()
{
	if (_parent)
	{
		_parent->removeChild(this);
	}
}

bool easy2d::Node::removeChild(Node * child)
{
	if (child == nullptr) E2D_WARNING(L"Node::removeChildren NULL pointer exception.");

	if (_children.empty())
	{
		return false;
	}

	if (child)
	{
		auto iter = std::find(_children.begin(), _children.end(), child);
		if (iter != _children.end())
		{
			_children.erase(iter);
			child->_parent = nullptr;

			if (child->_parentScene)
			{
				child->_setParentScene(nullptr);
			}

			child->release();
			return true;
		}
	}
	return false;
}

void easy2d::Node::removeChildren(const String& childName)
{
	if (childName.empty()) E2D_WARNING(L"Invalid Node name.");

	if (_children.empty())
	{
		return;
	}

	// 计算名称 Hash 值
	size_t hash = std::hash<String>{}(childName);

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

void easy2d::Node::clearAllChildren()
{
	// 所有节点的引用计数减一
	for (auto child : _children)
	{
		child->release();
	}
	// 清空储存节点的容器
	_children.clear();
}

void easy2d::Node::runAction(Action * action)
{
	ActionManager::start(action, this, false);
}

void easy2d::Node::resumeAction(const String& name)
{
	auto& actions = ActionManager::get(name);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->resume();
		}
	}
}

void easy2d::Node::pauseAction(const String& name)
{
	auto& actions = ActionManager::get(name);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->pause();
		}
	}
}

void easy2d::Node::stopAction(const String& name)
{
	auto& actions = ActionManager::get(name);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->stop();
		}
	}
}

void easy2d::Node::setAutoUpdate(bool bAutoUpdate)
{
	_autoUpdate = bAutoUpdate;
}

void easy2d::Node::setDefaultPiovt(float defaultPiovtX, float defaultPiovtY)
{
	s_fDefaultPiovtX = min(max(float(defaultPiovtX), 0), 1);
	s_fDefaultPiovtY = min(max(float(defaultPiovtY), 0), 1);
}

void easy2d::Node::resumeAllActions()
{
	ActionManager::__resumeAllBindedWith(this);
}

void easy2d::Node::pauseAllActions()
{
	ActionManager::__pauseAllBindedWith(this);
}

void easy2d::Node::stopAllActions()
{
	ActionManager::__stopAllBindedWith(this);
}

void easy2d::Node::setVisiable(bool value)
{
	_visiable = value;
}

void easy2d::Node::setName(const String& name)
{
	if (name.empty()) E2D_WARNING(L"Invalid Node name.");

	if (!name.empty() && _name != name)
	{
		// 保存节点名
		_name = name;
		// 保存节点 Hash 名
		_hashName = std::hash<String>{}(name);
	}
}

void easy2d::Node::_setParentScene(Scene * scene)
{
	_parentScene = scene;
	for (auto child : _children)
	{
		child->_setParentScene(scene);
	}
}