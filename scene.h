#pragma once

#include "src/entt.hpp"

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