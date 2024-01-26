#include "map.h"
#include <string>

Map::Map()
{
	std::string input;
	// load baseMap
	inputFile.open("baseMap.txt");

	if (!inputFile.fail())
	{
		while (inputFile >> input)
		{
			inputData.push_back(input);
		}
		if (!inputData.empty())
		{
			int row = -1;
			int column = 0;

			for (int i = 0; i < inputData.size(); i++)
			{
				column++;
				if (i % tileColumn == 0)
				{
					row++;
					column = 0;
				}
				if (inputData[i] == "~")
				{
					baseMap[row][column] = 1;
				}
				else if (inputData[i] == "#")
				{
					baseMap[row][column] = 0;
				}
			}
		}
	}
	inputFile.close();
	loadMap(baseMap);
}

Map::~Map()
{
}

void Map::loadMap(unsigned char arr[tileRow][tileColumn])
{
	for (int row = 0; row < tileRow; row++)
	{
		for (int column = 0; column < tileColumn; column++)
		{
			map[row][column] = arr[row][column];
		}
	}
}

void Map::drawMap(Texture tex0, Texture tex1, Texture tex2, Vector2 direction, unsigned char levelExploreMap[tileRow][tileColumn])
{
	for (int row = 0; row < tileRow; row++)
	{
		for (int column = 0; column < tileColumn; column++)
		{
			float posX = (column + direction.x) * tileSize;
			float posY = (row + direction.y) * tileSize;

			if (map[row][column] == 0 && levelExploreMap[row][column] > 0)
			{
				DrawTextureEx(tex0, { posX, posY }, 0.f, 1.f, WHITE);
			}
			else if (map[row][column] == 1 && levelExploreMap[row][column] > 0)
			{
				DrawTextureEx(tex1, { posX, posY }, 0.f, 1.f, WHITE);
			}
			else if (map[row][column] == 2 && levelExploreMap[row][column] > 0)
			{
				DrawTextureEx(tex2, { posX, posY }, 0.f, 1.f, WHITE);
			}
		}
	}
}
