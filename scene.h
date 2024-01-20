#pragma once

#include "src/entt.hpp"

class Entity;

class Scene
{
public:
	friend class Entity;
	Scene();
	~Scene();

	Entity createEntity(std::string tag);
	entt::registry registry;
	
private:
};