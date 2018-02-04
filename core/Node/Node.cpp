#include "..\enodes.h"
#include "..\emanagers.h"
#include "..\etools.h"
#include "..\eactions.h"
#include "..\eshape.h"
#include <algorithm>

// Ĭ�����ĵ�λ��
static float s_fDefaultPiovtX = 0;
static float s_fDefaultPiovtY = 0;

e2d::ENode::ENode()
	: m_nOrder(0)
	, m_fScaleX(1.0f)
	, m_fScaleY(1.0f)
	, m_fRotation(0)
	, m_fSkewAngleX(0)
	, m_fSkewAngleY(0)
	, m_fDisplayOpacity(1.0f)
	, m_fRealOpacity(1.0f)
	, m_fPivotX(s_fDefaultPiovtX)
	, m_fPivotY(s_fDefaultPiovtY)
	, m_MatriInitial(D2D1::Matrix3x2F::Identity())
	, m_MatriFinal(D2D1::Matrix3x2F::Identity())
	, m_bVisiable(true)
	, m_bDisplayedInScene(false)
	, m_pShape(nullptr)
	, m_pParent(nullptr)
	, m_pParentScene(nullptr)
	, m_nHashName(0)
	, m_bSortChildrenNeeded(false)
	, m_bTransformNeeded(false)
	, m_bAutoUpdate(true)
{
}

e2d::ENode::~ENode()
{
	ETimerManager::__clearAllTimersBindedWith(this);
	EActionManager::__clearAllActionsBindedWith(this);
	EShapeManager::__delShape(m_pShape);
	for (auto child : m_vChildren)
	{
		SafeRelease(&child);
	}
}

void e2d::ENode::_update(bool bPaused)
{
	if (m_bTransformNeeded)
	{
		_updateTransform(this);
	}

	if (!m_vChildren.empty())
	{
		// �ӽڵ�����
		if (m_bSortChildrenNeeded)
		{
			std::sort(
				std::begin(m_vChildren),
				std::end(m_vChildren),
				[](ENode * n1, ENode * n2) {
				return n1->getOrder() < n2->getOrder();
			}
			);

			m_bSortChildrenNeeded = false;
		}

		// �����ӽڵ�
		size_t size = m_vChildren.size();
		size_t i;
		for (i = 0; i < size; i++)
		{
			auto child = m_vChildren[i];
			// ���� Order С����Ľڵ�
			if (child->getOrder() < 0)
			{
				child->_update(bPaused);
			}
			else
			{
				break;
			}
		}

		if (bPaused)
		{
			this->onPause();
		}
		else if (m_bAutoUpdate)
		{
			this->onUpdate();
		}

		// ����ʣ��ڵ�
		for (; i < size; i++)
			m_vChildren[i]->_update(bPaused);
	}
	else
	{
		if (bPaused)
		{
			this->onPause();
		}
		else if (m_bAutoUpdate)
		{
			this->onUpdate();
		}
	}
}

void e2d::ENode::_render()
{
	if (!m_bVisiable)
	{
		return;
	}

	if (!m_vChildren.empty())
	{
		size_t size = m_vChildren.size();
		size_t i;
		for (i = 0; i < size; i++)
		{
			auto child = m_vChildren[i];
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
		ERenderer::getRenderTarget()->SetTransform(m_MatriFinal);
		// ��Ⱦ����
		this->onRender();

		// ����ʣ��ڵ�
		for (; i < size; i++)
			m_vChildren[i]->_render();
	}
	else
	{
		// ת����Ⱦ���Ķ�ά����
		ERenderer::getRenderTarget()->SetTransform(m_MatriFinal);
		// ��Ⱦ����
		this->onRender();
	}
}

void e2d::ENode::_drawShape()
{
	// ��������ļ�����״
	if (m_pShape && m_pShape->m_bIsVisiable)
	{
		m_pShape->_render();
	}

	// ���������ӽڵ�ļ�����״
	for (auto child : m_vChildren)
	{
		child->_drawShape();
	}
}

void e2d::ENode::_onEnter()
{
	if (!this->m_bDisplayedInScene)
	{
		this->m_bDisplayedInScene = true;
		this->onEnter();

		for (auto child : m_vChildren)
		{
			child->_onEnter();
		}
	}
}

void e2d::ENode::_onExit()
{
	if (this->m_bDisplayedInScene)
	{
		this->m_bDisplayedInScene = false;
		this->onExit();

		for (auto child : m_vChildren)
		{
			child->_onExit();
		}
	}
}

void e2d::ENode::_updateTransform()
{
	// �������ĵ�����
	D2D1_POINT_2F pivot = D2D1::Point2F(
		getRealWidth() * m_fPivotX,
		getRealHeight() * m_fPivotY
	);
	// �����Ķ�ά���α任���ӽڵ㽫�������������б任
	m_MatriInitial = D2D1::Matrix3x2F::Scale(
		m_fScaleX,
		m_fScaleY,
		pivot
	) * D2D1::Matrix3x2F::Skew(
		m_fSkewAngleX,
		m_fSkewAngleY,
		pivot
	) * D2D1::Matrix3x2F::Rotation(
		m_fRotation,
		pivot
	) * D2D1::Matrix3x2F::Translation(
		m_Pos.x,
		m_Pos.y
	);
	// �͸��ڵ�������
	if (m_pParent)
	{
		m_MatriInitial = m_MatriInitial * m_pParent->m_MatriInitial;
	}
	// �����������ĵ������ձ任
	m_MatriFinal = m_MatriInitial * D2D1::Matrix3x2F::Translation(-pivot.x, -pivot.y);
}

void e2d::ENode::_updateChildrenTransform()
{
	for (auto child : m_vChildren)
	{
		_updateTransform(child);
	}
}

void e2d::ENode::_updateTransform(ENode * node)
{
	// ���������ת������
	node->_updateTransform();
	// �����������״Ҳ������Ӧת��
	if (node->m_pShape)
	{
		node->m_pShape->_transform();
	}
	// �����ӽڵ��µ����нڵ�
	node->_updateChildrenTransform();
	// ��־��ִ�й��任
	node->m_bTransformNeeded = false;
}

void e2d::ENode::_updateChildrenOpacity()
{
	for (auto child : m_vChildren)
	{
		_updateOpacity(child);
	}
}

void e2d::ENode::_updateOpacity(ENode * node)
{
	if (node->m_pParent)
	{
		node->m_fDisplayOpacity = node->m_fRealOpacity * node->m_pParent->m_fDisplayOpacity;
	}
	node->_updateChildrenOpacity();
}

bool e2d::ENode::isVisiable() const
{
	return m_bVisiable;
}

e2d::EString e2d::ENode::getName() const
{
	return m_sName;
}

float e2d::ENode::getPosX() const
{
	return m_Pos.x;
}

float e2d::ENode::getPosY() const
{
	return m_Pos.y;
}

e2d::EPoint e2d::ENode::getPos() const
{
	return m_Pos;
}

float e2d::ENode::getWidth() const
{
	return m_Size.width * m_fScaleX;
}

float e2d::ENode::getHeight() const
{
	return m_Size.height * m_fScaleY;
}

float e2d::ENode::getRealWidth() const
{
	return m_Size.width;
}

float e2d::ENode::getRealHeight() const
{
	return m_Size.height;
}

e2d::ESize e2d::ENode::getRealSize() const
{
	return m_Size;
}

float e2d::ENode::getPivotX() const
{
	return m_fPivotX;
}

float e2d::ENode::getPivotY() const
{
	return m_fPivotY;
}

e2d::ESize e2d::ENode::getSize() const
{
	return ESize(getWidth(), getHeight());
}

float e2d::ENode::getScaleX() const
{
	return m_fScaleX;
}

float e2d::ENode::getScaleY() const
{
	return m_fScaleY;
}

float e2d::ENode::getSkewX() const
{
	return m_fSkewAngleX;
}

float e2d::ENode::getSkewY() const
{
	return m_fSkewAngleY;
}

float e2d::ENode::getRotation() const
{
	return m_fRotation;
}

float e2d::ENode::getOpacity() const
{
	return m_fRealOpacity;
}

e2d::EShape * e2d::ENode::getShape() const
{
	return m_pShape;
}

int e2d::ENode::getOrder() const
{
	return m_nOrder;
}

void e2d::ENode::setOrder(int order)
{
	m_nOrder = order;
}

void e2d::ENode::setPosX(float x)
{
	this->setPos(x, m_Pos.y);
}

void e2d::ENode::setPosY(float y)
{
	this->setPos(m_Pos.x, y);
}

void e2d::ENode::setPos(const EPoint & p)
{
	this->setPos(p.x, p.y);
}

void e2d::ENode::setPos(float x, float y)
{
	if (m_Pos.x == x && m_Pos.y == y)
		return;

	m_Pos.x = x;
	m_Pos.y = y;
	m_bTransformNeeded = true;
}

void e2d::ENode::movePosX(float x)
{
	this->movePos(x, 0);
}

void e2d::ENode::movePosY(float y)
{
	this->movePos(0, y);
}

void e2d::ENode::movePos(float x, float y)
{
	this->setPos(m_Pos.x + x, m_Pos.y + y);
}

void e2d::ENode::movePos(const EVector2 & v)
{
	this->movePos(v.x, v.y);
}

void e2d::ENode::_setSize(float width, float height)
{
	if (m_Size.width == width && m_Size.height == height)
		return;

	m_Size.width = width;
	m_Size.height = height;
	m_bTransformNeeded = true;
}

void e2d::ENode::setScaleX(float scaleX)
{
	this->setScale(scaleX, m_fScaleY);
}

void e2d::ENode::setScaleY(float scaleY)
{
	this->setScale(m_fScaleX, scaleY);
}

void e2d::ENode::setScale(float scale)
{
	this->setScale(scale, scale);
}

void e2d::ENode::setScale(float scaleX, float scaleY)
{
	if (m_fScaleX == scaleX && m_fScaleY == scaleY)
		return;

	m_fScaleX = scaleX;
	m_fScaleY = scaleY;
	m_bTransformNeeded = true;
}

void e2d::ENode::setSkewX(float angleX)
{
	this->setSkew(angleX, m_fSkewAngleY);
}

void e2d::ENode::setSkewY(float angleY)
{
	this->setSkew(m_fSkewAngleX, angleY);
}

void e2d::ENode::setSkew(float angleX, float angleY)
{
	if (m_fSkewAngleX == angleX && m_fSkewAngleY == angleY)
		return;

	m_fSkewAngleX = angleX;
	m_fSkewAngleY = angleY;
	m_bTransformNeeded = true;
}

void e2d::ENode::setRotation(float angle)
{
	if (m_fRotation == angle)
		return;

	m_fRotation = angle;
	m_bTransformNeeded = true;
}

void e2d::ENode::setOpacity(float opacity)
{
	if (m_fRealOpacity == opacity)
		return;

	m_fDisplayOpacity = m_fRealOpacity = min(max(opacity, 0), 1);
	// ���½ڵ�͸����
	_updateOpacity(this);
}

void e2d::ENode::setPivotX(float pivotX)
{
	this->setPivot(pivotX, m_fPivotY);
}

void e2d::ENode::setPivotY(float pivotY)
{
	this->setPivot(m_fPivotX, pivotY);
}

void e2d::ENode::setPivot(float pivotX, float pivotY)
{
	if (m_fPivotX == pivotX && m_fPivotY == pivotY)
		return;

	m_fPivotX = min(max(pivotX, 0), 1);
	m_fPivotY = min(max(pivotY, 0), 1);
	m_bTransformNeeded = true;
}

void e2d::ENode::setShape(EShape * pShape)
{
	// ɾ���ɵ���״
	EShapeManager::__delShape(m_pShape);
	// ����µ���״
	EShapeManager::__addShape(pShape);

	if (pShape)
	{
		// ˫���
		this->m_pShape = pShape;
		pShape->m_pParentNode = this;
	}
	else
	{
		this->m_pShape = nullptr;
	}
}

void e2d::ENode::addChild(ENode * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "ENode::addChild NULL pointer exception.");

	if (child)
	{
		ASSERT(child->m_pParent == nullptr, "ENode already added. It can't be added again!");

		for (ENode * parent = this; parent != nullptr; parent = parent->getParent())
		{
			ASSERT(child != parent, "A ENode cannot be the child of his own children!");
		}

		m_vChildren.push_back(child);

		child->setOrder(order);

		child->retain();

		child->m_pParent = this;

		if (this->m_pParentScene)
		{
			child->_setParentScene(this->m_pParentScene);
		}

		if (this->m_bDisplayedInScene)
		{
			child->_onEnter();
		}

		// �����ӽڵ�͸����
		_updateOpacity(child);
		// ���½ڵ�ת��
		child->m_bTransformNeeded = true;
		// �����ӽڵ�����
		m_bSortChildrenNeeded = true;
	}
}

e2d::ENode * e2d::ENode::getParent() const
{
	return m_pParent;
}

e2d::EScene * e2d::ENode::getParentScene() const
{
	return m_pParentScene;
}

std::vector<e2d::ENode*>& e2d::ENode::getChildren()
{
	return m_vChildren;
}

int e2d::ENode::getChildrenCount() const
{
	return static_cast<int>(m_vChildren.size());
}

e2d::ENode * e2d::ENode::getChild(const EString & name)
{
	WARN_IF(name.isEmpty(), "Invalid ENode name.");

	unsigned int hash = name.hash();

	for (auto child : m_vChildren)
	{
		// ��ͬ�����ƿ��ܻ�����ͬ�� Hash ֵ�������ȱȽ� Hash �������������ٶ�
		if (child->m_nHashName == hash && child->m_sName == name)
			return child;
	}
	return nullptr;
}

std::vector<e2d::ENode*> e2d::ENode::getChildren(const EString & name)
{
	std::vector<ENode*> vChildren;

	WARN_IF(name.isEmpty(), "Invalid ENode name.");

	unsigned int hash = name.hash();

	for (auto child : m_vChildren)
		if (child->m_nHashName == hash && child->m_sName == name)
			vChildren.push_back(child);

	return std::move(vChildren);
}

void e2d::ENode::removeFromParent()
{
	if (m_pParent)
	{
		m_pParent->removeChild(this);
	}
}

bool e2d::ENode::removeChild(ENode * child)
{
	WARN_IF(child == nullptr, "ENode::removeChildren NULL pointer exception.");

	if (m_vChildren.empty())
	{
		return false;
	}

	if (child)
	{
		size_t size = m_vChildren.size();
		for (size_t i = 0; i < size; i++)
		{
			if (m_vChildren[i] == child)
			{
				m_vChildren.erase(m_vChildren.begin() + i);
				child->m_pParent = nullptr;

				if (child->m_pParentScene)
				{
					child->_setParentScene(nullptr);
				}
				if (child->m_bDisplayedInScene)
				{
					child->_onExit();
				}

				child->release();
				return true;
			}
		}
	}
	return false;
}

void e2d::ENode::removeChildren(const EString & childName)
{
	WARN_IF(childName.isEmpty(), "Invalid ENode name.");

	if (m_vChildren.empty())
	{
		return;
	}

	// �������� Hash ֵ
	unsigned int hash = childName.hash();

	size_t size = m_vChildren.size();
	for (size_t i = 0; i < size; i++)
	{
		auto child = m_vChildren[i];
		if (child->m_nHashName == hash && child->m_sName == childName)
		{
			m_vChildren.erase(m_vChildren.begin() + i);
			child->m_pParent = nullptr;
			if (child->m_pParentScene)
			{
				child->_setParentScene(nullptr);
			}
			if (child->m_bDisplayedInScene)
			{
				child->_onExit();
			}
			child->release();
		}
	}
}

void e2d::ENode::clearAllChildren()
{
	// ���нڵ�����ü�����һ
	for (auto child : m_vChildren)
	{
		if (child->m_bDisplayedInScene)
		{
			child->_onExit();
		}
		child->release();
	}
	// ��մ���ڵ������
	m_vChildren.clear();
}

void e2d::ENode::runAction(EAction * action)
{
	WARN_IF(
		action->getTarget() != nullptr,
		"The action is already running, The clone of the action will be created automatically!"
	);
	if (action->getTarget())
	{
		action = action->clone();
	}
	action->_setTarget(this);
	EActionManager::addAction(action);
}

void e2d::ENode::resumeAction(EAction * action)
{
	if (action->getTarget() == this)
	{
		action->resume();
	}
}

void e2d::ENode::pauseAction(EAction * action)
{
	if (action->getTarget() == this)
	{
		action->pause();
	}
}

bool e2d::ENode::isPointIn(EPoint point)
{
	if (m_bTransformNeeded)
	{
		_updateTransform(this);
	}
	// Ϊ�ڵ㴴��һ����״
	ID2D1RectangleGeometry * rect;
	ERenderer::getID2D1Factory()->CreateRectangleGeometry(
		D2D1::RectF(0, 0, getRealWidth(), getRealHeight()),
		&rect
	);
	// �жϵ��Ƿ�����״��
	BOOL ret;
	rect->FillContainsPoint(
		D2D1::Point2F(
			point.x,
			point.y),
		&m_MatriFinal,
		&ret
	);
	if (ret)
	{
		return true;
	}
	else
	{
		for (auto child : m_vChildren)
			if (child->isPointIn(point))
				return true;
	}
	return false;
}

void e2d::ENode::setAutoUpdate(bool bAutoUpdate)
{
	m_bAutoUpdate = bAutoUpdate;
}

void e2d::ENode::setDefaultPiovt(float defaultPiovtX, float defaultPiovtY)
{
	s_fDefaultPiovtX = min(max(defaultPiovtX, 0), 1);
	s_fDefaultPiovtY = min(max(defaultPiovtY, 0), 1);
}

void e2d::ENode::stopAction(EAction * action)
{
	if (action->getTarget() == this)
	{
		action->stop();
	}
}

void e2d::ENode::resumeAllActions()
{
	EActionManager::resumeAllActionsBindedWith(this);
}

void e2d::ENode::pauseAllActions()
{
	EActionManager::pauseAllActionsBindedWith(this);
}

void e2d::ENode::stopAllActions()
{
	EActionManager::stopAllActionsBindedWith(this);
}

void e2d::ENode::setVisiable(bool value)
{
	m_bVisiable = value;
}

void e2d::ENode::setName(const EString & name)
{
	WARN_IF(name.isEmpty(), "Invalid ENode name.");

	if (!name.isEmpty() && m_sName != name)
	{
		// ����ڵ���
		m_sName = name;
		// ����ڵ� Hash ��
		m_nHashName = name.hash();
	}
}

void e2d::ENode::_setParentScene(EScene * scene)
{
	m_pParentScene = scene;
	for (auto child : m_vChildren)
	{
		child->_setParentScene(scene);
	}
}