#pragma once

#include "raylib.h"
#include <vector>
#include <memory>

class Scene;
class Entity;

class Logic
{
public:
	Logic();
	~Logic();
	void Render();
	void Update();
private:
	void createBasicGameEntity(Scene& scene, float posX, float posY, Texture texture, const char* tag);
	void createAnimatedGameEntity(Scene& scene, float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag);
	void createAllGameEntity();
	void initializeTexture(Texture& texture, const char* filePath);
	void initializeAllTexture();
	void initializeSound(Sound& sound, const char* filePath);
	void initializeAllSound();
	void drawObject();
	void playPlayerAnimation(std::vector<Entity>& entity, int sourceY_multiplyer, int i);
	void closeBag();
	void openBag();
	void showQuest();
	void bagUI();
	void handleOpenCloseBag();
	void handleInventoryIsFull();
	void playerMovementAndCollisions(float deltaTime);
	void getPlayerFramesXY();
	Texture playerTexture;
	Texture mapTexture;
	Texture vendorTexture;
	Texture woodStashTexture;
	Texture bagTexture;
	Texture goldCurrencyTexture;
	Texture fishTexture;
	Texture barrelTexture;
	std::vector<Entity> gameEntities;
	std::vector<Texture> gameTextures;
	std::vector<Sound> gameSounds;
	float playerSpeed = 0.f;
	Vector2 playerLocation;
	Sound questAcceptedSound;
	Sound questDoneSound;
	Sound coinCollectedSound;
	Sound openBagSound;
	Sound closeBagSound;
	Sound inventoryFull;
	bool isBagOpen = false;
	Music themeSong;
	int questReturnValue = 0;
	bool isQuestAccepted = false;
	int questTextSize = 0;
	int goldCurrency = 0;
	int playerFramesX = 0;
	int playerFramesY = 0;
};