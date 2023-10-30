#include "scene.h"
#include "entity.h"
#include "components.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::createEntity(std::string tag)
{
	Entity entity(registry.create(), this);
	entity.addComponent<TagComponent>(tag);
	return entity;
}