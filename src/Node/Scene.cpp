#include <easy2d/e2dbase.h>
#include <easy2d/e2dnode.h>
#include <easy2d/e2dmanager.h>

easy2d::Scene::Scene()
{
	_setParentScene(this);
}

easy2d::Scene::~Scene()
{
}
