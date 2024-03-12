#pragma once

#include "raylib.h"
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include "enum.h"

struct Sprite2DComponent;

class Logic
{
public:
	Logic();
	~Logic();
	void Render();
	void Update();
	static constexpr int windowHeight = 1024;
	static constexpr int windowWidth = 1024;
	static constexpr char MAX_NAME_CHAR = 6;

private:
	float playerSpeed = 0.f;
	int questReturnValue = 0;
	int goldCurrency = 0;
	int playerFramesX = 0;
	int playerFramesY = 0;
	int letterCount = 0;
	bool isNameGiven = false;
	bool inMainMenu = true;
	float mapScrollingSpeed = 10.f;
	float xScrollingOffset = 0.f;
	float yScrollingOffset = 0.f;

	Vector2 playerLocation = { 0.f, 0.f };
	Vector2 playerDirection = { 0.f, 0.f };

	std::string playerName;
	char name[MAX_NAME_CHAR + 1] = "\0";

	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;

	Level level = Level::level_0;
	QuestState questState = QuestState::None;

	void createAllGameEntity();
	void drawObject();
	void playPlayerAnimation(Sprite2DComponent& sprite, int sourceY_multiplyer);
	void showQuest();
	void playerMovement(float deltaTime);
	void getPlayerFramesXY();
	void handleLevels();
	void saveGame();
	void loadGame();
	void constructMapEntities();
	void modifyPlayerSpeedOnRuntime();

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

	template <typename ...Args>
	auto registerLuaFunction(const char* name)
	{
		return [](Args ...args)
			{
				
			};
	}
};