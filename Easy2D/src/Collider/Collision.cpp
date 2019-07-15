#include <e2dcollider.h>
#include <e2dnode.h>
#include <e2dtool.h>

typedef std::pair<size_t, size_t> HashPair;

static std::vector<easy2d::Listener*> s_vListeners;	// ¼àÌýÆ÷ÈÝÆ÷
static bool s_bCollisionEnable = false;				// Åö×²´¥·¢×´Ì¬
static easy2d::Node * s_pActiveNode = nullptr;		// Ö÷¶¯Åö×²Ìå
static easy2d::Node * s_pPassiveNode = nullptr;		// ±»¶¯Åö×²Ìå
static std::set<HashPair> s_sCollisionList;			// Åö×²Ó³Éä


void easy2d::Collision::addName(const String & name1, const String & name2)
{
	if (!name1.empty() && !name2.empty())
	{
		s_sCollisionList.insert(HashPair(std::hash<String>{}(name1), std::hash<String>{}(name2)));
	}
}

void easy2d::Collision::addName(const std::vector<std::pair<String, String> >& names)
{
	for (auto& name : names)
	{
		if (!name.first.empty() && !name.second.empty())
		{
			s_sCollisionList.insert(HashPair(std::hash<String>{}(name.first), std::hash<String>{}(name.second)));
		}
	}
}

bool easy2d::Collision::isCollidable(Node * node1, Node * node2)
{
	return Collision::isCollidable(node1->getName(), node2->getName());
}

bool easy2d::Collision::isCollidable(const String & name1, const String & name2)
{
	size_t hashName1 = std::hash<String>{}(name1), hashName2 = std::hash<String>{}(name2);
	HashPair pair1 = HashPair(hashName1, hashName2), pair2 = HashPair(hashName2, hashName1);
	for (auto& pair : s_sCollisionList)
	{
		if (pair == pair1 || pair == pair2)
		{
			return true;
		}
	}
	return false;
}

easy2d::Node * easy2d::Collision::getActiveNode()
{
	return s_pActiveNode;
}

easy2d::Node * easy2d::Collision::getPassiveNode()
{
	return s_pPassiveNode;
}

bool easy2d::Collision::isCausedBy(const String & name1, const String & name2)
{
	String activeName = s_pActiveNode->getName();
	String passiveName = s_pPassiveNode->getName();
	return (activeName == name1 && passiveName == name2) ||
		(activeName == name2 && passiveName == name1);
}

bool easy2d::Collision::isCausedBy(Node * node1, Node * node2)
{
	return (s_pActiveNode == node1 && s_pPassiveNode == node2) ||
		(s_pActiveNode == node2 && s_pPassiveNode == node1);
}

easy2d::Node* easy2d::Collision::isCausedBy(Node * node)
{
	if (s_pActiveNode == node)
		return s_pPassiveNode;
	if (s_pPassiveNode == node)
		return s_pActiveNode;
	return nullptr;
}

easy2d::Node* easy2d::Collision::isCausedBy(const String& name)
{
	if (s_pActiveNode->getName() == name)
		return s_pActiveNode;
	if (s_pPassiveNode->getName() == name)
		return s_pPassiveNode;
	return nullptr;
}


void easy2d::Collision::setEnable(bool enable)
{
	s_bCollisionEnable = enable;
}

bool easy2d::Collision::isEnable()
{
	return s_bCollisionEnable;
}

void easy2d::Collision::__update(Node * active, Node * passive)
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	s_pActiveNode = active;
	s_pPassiveNode = passive;

	for (size_t i = 0; i < s_vListeners.size(); ++i)
	{
		auto listener = s_vListeners[i];
		// Çå³ýÒÑÍ£Ö¹µÄ¼àÌýÆ÷
		if (listener->_stopped)
		{
			GC::release(listener);
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ¸üÐÂ¼àÌýÆ÷
			listener->_update();
			++i;
		}
	}

	s_pActiveNode = nullptr;
	s_pPassiveNode = nullptr;
}

easy2d::Listener * easy2d::Collision::addListener(const Function<void()>& func, const String& name, bool paused)
{
	auto listener = gcnew Listener(func, name, paused);
	GC::retain(listener);
	s_vListeners.push_back(listener);
	return listener;
}

void easy2d::Collision::addListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(s_vListeners.begin(), s_vListeners.end(), listener);
		if (iter == s_vListeners.end())
		{
			GC::retain(listener);
			s_vListeners.push_back(listener);
		}
	}
}

void easy2d::Collision::removeListener(Listener * listener)
{
	if (listener)
	{
		auto iter = std::find(s_vListeners.begin(), s_vListeners.end(), listener);
		if (iter != s_vListeners.end())
		{
			GC::release(listener);
			s_vListeners.erase(iter);
		}
	}
}

void easy2d::Collision::stopListener(const String& name)
{
	if (s_vListeners.empty() || name.empty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->stop();
		}
	}
}

void easy2d::Collision::startListener(const String& name)
{
	if (s_vListeners.empty() || name.empty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->start();
		}
	}
}

void easy2d::Collision::removeListener(const String& name)
{
	if (s_vListeners.empty() || name.empty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->_name == name)
		{
			listener->_stopped = true;
		}
	}
}

void easy2d::Collision::stopAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->stop();
	}
}

void easy2d::Collision::startAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->start();
	}
}

void easy2d::Collision::removeAllListeners()
{
	for (auto listener : s_vListeners)
	{
		listener->_stopped = true;
	}
}

void easy2d::Collision::__clearListeners()
{
	for (auto listener : s_vListeners)
	{
		GC::release(listener);
	}
	s_vListeners.clear();
}