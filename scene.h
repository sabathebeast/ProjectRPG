#pragma once

#include "src/entt.hpp"

struct Texture;

class Scene
{
public:
	friend class Entity;
	Scene();
	~Scene();

	entt::registry registry;

	void createBasicGameEntity(float posX, float posY, Texture texture, const char* tag);
	void createAnimatedGameEntity(float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag);
	void createMapEntities(float posX, float posY, const char* tag);
	
private:
	Entity createEntity(std::string tag);
};