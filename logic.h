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
	friend class GameState;
	void Render();
	void Update();
	static constexpr int windowHeight = 1024;
	static constexpr int windowWidth = 1024;
	static constexpr char MAX_NAME_CHAR = 6;

private:
	int questReturnValue = 0;
	int letterCount = 0;
	bool inMainMenu = true;
	float xScrollingOffset = 0.f;
	float yScrollingOffset = 0.f;
	
	char name[MAX_NAME_CHAR + 1] = "\0";

	Level level = Level::level_0;
	QuestState questState = QuestState::None;

	void createAllGameEntity();
	void drawObject();
	void playPlayerAnimation(Sprite2DComponent& sprite, int sourceY_multiplyer);
	void showQuest();
	void playerMovement(float deltaTime);
	void getPlayerFramesXY();
	void handleLevels();
	void constructMapEntities();
	void modifyPlayerSpeedOnRuntime();

	template <typename ...Args>
	auto registerLuaFunction(const char* name)
	{
		return [](Args ...args)
			{

			};
	}
};