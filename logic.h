#pragma once

#include "raylib.h"
#include <vector>
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
	float xScrollingOffset = 0.f;
	float yScrollingOffset = 0.f;

	Level level = Level::level_0;
	QuestState questState = QuestState::None;

	void createAllGameEntity();
	void drawObject();
	void playPlayerAnimation(Sprite2DComponent& sprite, int sourceY_multiplyer);
	void showQuest();
	void playerMovement(float deltaTime);
	void getPlayerFramesXY();
	void handleLevels();
};