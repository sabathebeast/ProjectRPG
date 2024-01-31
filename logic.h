#pragma once

#include "raylib.h"
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <array>
#include "enum.h"

constexpr char MAX_NAME_CHAR = 6;

enum class QuestState : unsigned char
{
	None,
	Accepted,
	Fullfilled,
	Done
};

class Scene;
struct Sprite2DComponent;

class Logic
{
public:
	Logic();
	~Logic();
	void Render();
	void Update();
private:
	const int windowWidth = GetScreenWidth();
	const int windowHeight = GetScreenHeight();
	void createBasicGameEntity(Scene& scene, float posX, float posY, Texture texture, const char* tag);
	void createAnimatedGameEntity(Scene& scene, float posX, float posY, Texture texture, int currentFrame, int frameCount, int frameSpeed, int framesX, int framesY, float sourceX, float sourceY, const char* tag);
	void createMapEntities(Scene& scene, float posX, float posY, Texture texture, const char* tag);
	void createAllGameEntity();
	void drawObject();
	void playPlayerAnimation(Sprite2DComponent& sprite, int sourceY_multiplyer);
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
	void constructMapEntities(Texture tex0, Texture tex1, Texture tex2);
	float playerSpeed = 0.f;
	Vector2 playerLocation;
	bool isBagOpen = false;
	bool isCharacterInfoOpen = false;
	int questReturnValue = 0;
	int goldCurrency = 0;
	int playerFramesX = 0;
	int playerFramesY = 0;
	int letterCount = 0;
	Vector2 playerDirection = { 0.f, 0.f };
	char name[MAX_NAME_CHAR + 1] = "\0";
	std::string playerName;
	bool isNameGiven = false;
	float mapScrollingSpeed = 10.f;
	Level level = Level::level_0;
	QuestState questState = QuestState::None;
	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;
	float xScrollingOffset = 0.f;
	float yScrollingOffset = 0.f;

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