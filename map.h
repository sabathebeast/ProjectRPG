#pragma once

#include "raylib.h"
#include <fstream>
#include <vector>
#include "enum.h"

class Scene;

class Map
{
public:
	Map();
	~Map() {};

	static constexpr int tileWidth = 16;
	static constexpr int tileHeight = 16;
	static constexpr int tileRow = 70;
	static constexpr int tileColumn = 70;
	static constexpr int mapWidth = tileColumn * tileWidth;
	static constexpr int mapHeight = tileRow * tileHeight;

	void addMapExlore(const Scene& scene, Level level, float playerDirectionX, float playerDirectionY);

	inline const unsigned char(&getMap())[tileRow][tileColumn] { return map; }

private:
	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;

	unsigned char levelZeroExploreMap[tileRow][tileColumn] = { 0 };
	unsigned char levelOneExploreMap[tileRow][tileColumn] = { 0 };
	unsigned char baseMap[tileRow][tileColumn] = { 0 };
	unsigned char map[tileRow][tileColumn] = { 0 };

	int clearViewSize = 6;
	int clearGrayViewSize = 8;
	int grayViewSize = 15;

	void loadMap(unsigned char arr[tileRow][tileColumn]);
	void passExploreMap(int playerLocXGrid, int playerLocYGrid, float playerDirectionX, float playerDirectionY, unsigned char (&currentExploreMap)[tileRow][tileColumn]);
};
