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

void Scene::createBasicGameEntity(float posX, float posY, Texture texture, const char* tag)
{
	Entity gameEntity = createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<TextureComponent>(texture);
	gameEntity.addComponent<ActiveComponent>();
}

void Scene::createAnimatedGameEntity(float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag)
{
	Entity gameEntity = createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<Sprite2DComponent>(texture, currentFrame, frameCount, frameSpeed, framesX, framesY, sourceX, sourceY);
	gameEntity.addComponent<ActiveComponent>();
}

void Scene::createMapEntities(float posX, float posY, const char* tag)
{
	Entity gameEntity = createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<TileComponent>();
}

void Scene::createEntitiyWithCollision(float posX, float posY, Texture texture, const char* tag)
{
	Entity gameEntity = createEntity(tag);
	gameEntity.addComponent<PositionComponent>(posX, posY);
	gameEntity.addComponent<TextureComponent>(texture);
	gameEntity.addComponent<ColllisionComponent>();
}
