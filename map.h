#pragma once
#include "raylib.h"

static const int tileSize = 16;
static const int tileRow = 70;
static const int tileColumn = 70;

class Map
{
public:
	Map();
	~Map();

	const int mapWidth = tileColumn * tileSize;
	const int mapHeight = tileRow * tileSize;
	const int mapTileSize = tileSize;

	void loadMap(unsigned char arr[tileRow][tileColumn]);
	void drawMap(Texture tex0, Texture tex1, Texture tex2, Vector2 direction, unsigned char levelExploreMap[tileRow][tileColumn]);
	unsigned char levelZeroExploreMap[tileRow][tileColumn] = { 0 };
	unsigned char levelOneExploreMap[tileRow][tileColumn] = { 0 };
	unsigned char map[tileRow][tileColumn];
private:
};
