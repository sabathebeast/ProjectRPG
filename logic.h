#pragma once

#include "raylib.h"
#include <vector>
#include <memory>
#include <fstream>

enum class Level : unsigned char
{
	level_0,
	level_1,
	level_2,
};

class Scene;
class Entity;
class Item;

class Logic
{
public:
	Logic();
	~Logic();
	void Render();
	void Update();
	const int windowWidth = GetScreenWidth();
	const int windowHeight = GetScreenHeight();
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
	void handleLevels();
	void saveGame();
	void loadGame();
	Texture playerTexture;
	Texture vendorTexture;
	Texture woodStashTexture;
	Texture bagTexture;
	Texture goldCurrencyTexture;
	Texture fishTexture;
	Texture barrelTexture;
	Texture dirtTexture;
	Texture waterTexture;
	Texture grassTexture;
	Texture houseTexture;
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
	bool isQuestFullfilled = false;
	int questTextSize = 0;
	int goldCurrency = 0;
	int playerFramesX = 0;
	int playerFramesY = 0;
	Vector2 playerDirection = { 0.f, 0.f };
	float mapScrollingSpeed = 10.f;
	Level level = Level::level_0;
	std::ofstream outputFile;
	std::ifstream inputFile;
};