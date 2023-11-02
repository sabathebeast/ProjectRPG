#pragma once

#include "src/entt.hpp"
#include "scene.h"

class Entity
{
public:
	Entity() {}
	Entity(entt::entity s_entity_handle, Scene* s_scene)
		: entityHandle(s_entity_handle), scene(s_scene) {};

	inline entt::entity getHandle() { return entityHandle; }

	template<typename T, typename... Args>
	T& addComponent(Args&&... args)
	{
		return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& getComponent()
	{
		return scene->registry.get<T>(entityHandle);
	}

	template<typename T>
	void removeComponent()
	{
		scene->registry.remove<T>(entityHandle);
	}

	template<typename T>
	bool hasComponent()
	{
		return scene->registry.all_of<T>(entityHandle);
	}

private:
	entt::entity entityHandle = entt::entity(0);
	Scene* scene = nullptr;
};