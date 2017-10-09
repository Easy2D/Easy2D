#include "..\easy2d.h"
#include "..\EasyX\easyx.h"

BatchNode::BatchNode()
{
}


BatchNode::~BatchNode()
{
	clearAllChildren();
}

bool BatchNode::_exec(bool active)
{
	// �����ڵ��Ƿ���ʾ
	if (!m_bDisplay)
	{
		return false;
	}
	// ������������ӽڵ�
	for (int i = int(m_vChildren.size() - 1); i >= 0; i--)
	{
		if (m_vChildren[i]->_exec(active))
		{
			active = false;
		}
	}
	// ���ӽڵ�ȡ���˻��潹�㣬��ýڵ�Ҳȡ���˽���
	return !active;
}

void BatchNode::_onDraw()
{
	// �ڵ��Ƿ���ʾ
	if (!m_bDisplay)
	{
		return;
	}
	
	for (auto child : m_vChildren)
	{
		// �����ӽڵ�
		child->_onDraw();
	}
}

void BatchNode::add(Node * child, int z_Order)
{
	if (child == nullptr) return;

	// ���ýڵ�ĸ�����
	child->setParentScene(this->getParentScene());
	// ���ýڵ��������ڵ��е� z ��˳��
	child->setZOrder(z_Order);
	// ��������ü�����һ
	child->retain();
	// �޸��ӽڵ�λ��
	child->move(getPos());

	// �� z ��˳�����ڵ�
	size_t size = m_vChildren.size();
	for (unsigned i = 0; i <= size; i++)
	{
		if (i != size)
		{
			if (z_Order < m_vChildren.at(i)->getZOrder())
			{
				m_vChildren.insert(m_vChildren.begin() + i, child);
				break;
			}
		}
		else
		{
			m_vChildren.push_back(child);
			break;
		}
	}
}

bool BatchNode::del(Node * child)
{
	if (child == nullptr) return false;

	// Ѱ���Ƿ�����ͬ�ڵ�
	std::vector<Node*>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		// �ҵ���ͬ�ڵ�
		if ((*iter) == child)
		{
			// ��������ü�����һ
			(*iter)->release();
			// ȥ���ýڵ�
			m_vChildren.erase(iter);
			return true;
		}
	}
	return false;
}

int BatchNode::getCount()
{
	return (int)m_vChildren.size();
}

std::vector<Node*>& BatchNode::getChildren()
{
	return m_vChildren;
}

void BatchNode::clearAllChildren()
{
	// ���нڵ�����ü�����һ
	for (auto child : m_vChildren)
	{
		child->autoRelease();
		child->release();
	}
	// ��մ���ڵ������
	m_vChildren.clear();
}

void BatchNode::setX(int x)
{
	// ��������ƶ�λ��
	int var = x - getX();
	// �ƶ������ӽڵ��λ��
	for (auto child : m_vChildren)
	{
		child->move(var, 0);
	}
	Node::setX(x);
}

void BatchNode::setY(int y)
{
	// ��������ƶ�λ��
	int var = y - getY();
	// �ƶ������ӽڵ��λ��
	for (auto child : m_vChildren)
	{
		child->move(0, var);
	}
	Node::setY(y);
}

void BatchNode::setPos(int x, int y)
{
	// ��������ƶ�λ��
	CPoint var(x - getX(), y - getY());
	// �ƶ������ӽڵ��λ��
	for (auto child : m_vChildren)
	{
		child->move(var);
	}
	Node::setPos(x, y);
}

void BatchNode::setPos(CPoint p)
{
	// ��������ƶ�λ��
	CPoint var(p - getPos());
	// �ƶ������ӽڵ��λ��
	for (auto child : m_vChildren)
	{
		child->move(var);
	}
	Node::setPos(p);
}

void BatchNode::move(int x, int y)
{
	// �ƶ������ӽڵ��λ��
	for (auto child : m_vChildren)
	{
		child->move(x, y);
	}
	Node::move(x, y);
}

void BatchNode::move(CVector v)
{
	// �ƶ������ӽڵ��λ��
	for (auto child : m_vChildren)
	{
		child->move(v);
	}
	Node::move(v);
}
