#pragma once

#include "raylib.h"
#include <vector>
#include <memory>
#include <fstream>
#include <string>

constexpr char MAX_NAME_CHAR = 6;

enum class Level : unsigned char
{
	level_0,
	level_1,
	level_2,
};

enum class QuestState : unsigned char
{
	None,
	Accepted,
	Fullfilled,
	Done
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
	void toolBarUI();
	void characterInfoUI();
	void characterOverlayUI();
	void handleOpenCloseBag();
	void handleInventoryIsFull();
	void handleOpenCloseCharacterInfo();
	void playerMovementAndCollisions(float deltaTime);
	void getPlayerFramesXY();
	void handleLevels();
	void saveGame();
	void loadGame();
	void setStats();
	void healthRegenerate(double currentTime);
	void energyRegenerate(double currentTime);
	void addLevelExplore();
	bool isEnergyRegenerateTimerStarted = true;
	double energyRegenerateTime = 0.0;
	bool isHealthRegenerateTimerStarted = true;
	double healthRegenerateTime = 0.0;
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
	//std::vector<Entity> gameEntities;
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
	bool isCharacterInfoOpen = false;
	Music themeSong;
	int questReturnValue = 0;
	int goldCurrency = 0;
	int playerFramesX = 0;
	int playerFramesY = 0;
	int letterCount = 0;
	int clearViewSize = 6;
	int clearGrayViewSize = 8;
	int grayViewSize = 15;
	char name[MAX_NAME_CHAR + 1] = "\0";
	std::string playerName;
	bool isNameGiven = false;
	Vector2 playerDirection = { 0.f, 0.f };
	float mapScrollingSpeed = 10.f;
	Level level = Level::level_0;
	QuestState questState = QuestState::None;
	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;

	template <typename T>
	void addToSaveGame(const char* name, T arg)
	{
		outputFile << name << " " << ":" << " " << arg << "\n";
	}

	template <typename A, typename B, typename C, typename D, typename E>
	void addToInventory(A arg0, B arg1, C arg2, D arg3, E arg4)
	{
		outputFile << arg0 << " " << ":" << " " << arg1 << " " << arg2 << " " << arg3 << " " << arg4 << "\n";
	}
};