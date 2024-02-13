#pragma once
#include "raylib.h"
#include <fstream>
#include <vector>
#include "enum.h"

static const int tileWidth = 16;
static const int tileHeight = 16;
static const int tileRow = 70;
static const int tileColumn = 70;

class Scene;

class Map
{
public:
	Map();
	~Map();

	const int mapWidth = tileColumn * tileWidth;
	const int mapHeight = tileRow * tileHeight;
	const int mapTileWidth = tileWidth;
	const int mapTileHeight = tileHeight;

	void loadMap(unsigned char arr[tileRow][tileColumn]);
	unsigned char levelZeroExploreMap[tileRow][tileColumn] = { 0 };
	unsigned char levelOneExploreMap[tileRow][tileColumn] = { 0 };
	unsigned char baseMap[tileRow][tileColumn] = { 0 };
	unsigned char map[tileRow][tileColumn] = { 0 };

	void addMapExlore(const Scene& scene, Level level, float playerDirectionX, float playerDirectionY);
private:
	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;
	int clearViewSize = 6;
	int clearGrayViewSize = 8;
	int grayViewSize = 15;
};
