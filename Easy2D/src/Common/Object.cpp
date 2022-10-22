#include <easy2d/e2dbase.h>
#include <easy2d/e2dmanager.h>
#include <functional> // std::hash

easy2d::Object::Object()
	: _refCount(1)
	, _name(nullptr)
	, _hashName(0)
{
	// 构造对象时，引用计数置 1
}

easy2d::Object::~Object()
{
	if (_name)
	{
		delete _name;
		_name = nullptr;
	}
}

void easy2d::Object::autorelease()
{
	GC::trace(this);
}

void easy2d::Object::retain()
{
	++_refCount;
}

void easy2d::Object::release()
{
	_refCount--;

	if (_refCount == 0)
	{
		delete this;
	}
}

int easy2d::Object::getRefCount() const
{
	return _refCount;
}

easy2d::String easy2d::Object::getName() const
{
	if (_name)
	{
		return *_name;
	}
	return String();
}

void easy2d::Object::setName(const String& name)
{
	if (!name.empty())
	{
		if (_name)
		{
			*_name = name;
		}
		else
		{
			_name = new String(name);
		}
	}
	else if (_name)
	{
		delete _name;
		_name = nullptr;
	}
	_hashName = std::hash<String>{}(name);
}

bool easy2d::Object::isName(const String& name, size_t hashName) const
{
	if (hashName)
	{
		return hashName == _hashName && name == getName();
	}
	else
	{
		return getName() == name;
	}
}
