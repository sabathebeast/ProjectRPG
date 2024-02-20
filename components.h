#pragma once

#include <string>
#include "raylib.h"


struct TagComponent
{
	std::string tag;
};

struct PositionComponent
{
	float x, y;
};

struct TextureComponent
{
	Texture texture;
};

struct Sprite2DComponent
{
	Texture texture;
	int currentFrame, frameCount, frameSpeed, framesX, framesY;
	float sourceX, sourceY;
};

struct ActiveComponent
{
	bool isActive = true;
};

struct TileComponent
{
	bool isDrawable = true;
};

struct ColllisionComponent
{
	bool isPlayerBehind = false;
};