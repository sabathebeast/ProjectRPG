#include "map.h"
#include <string>
#include "entity.h"
#include "components.h"


Map::Map()
{
	std::string input;
	// load baseMap
	/*inputFile.open("baseMap.txt");

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
	inputFile.close();*/


	inputFile.open("map.txt");

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
				baseMap[row][column] = std::stoi(inputData[i]);
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

void Map::addMapExlore(const Scene& scene, Level level, float playerDirectionX, float playerDirectionY)
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this, level, playerDirectionX, playerDirectionY](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			if (tag.tag == "player")
			{
				int playerLocXGrid = static_cast<int>(position.x - (playerDirectionX * tileWidth));
				int playerLocYGrid = static_cast<int>(position.y - (playerDirectionY * tileHeight));

				if (level == Level::level_0)
				{
					for (int i = -(clearViewSize); i < clearViewSize; i++)
					{
						for (int j = -(clearViewSize); j < clearViewSize; j++)
						{
							if (playerLocYGrid / tileHeight + i >= 0 && playerLocYGrid / tileHeight + i <= tileRow && playerLocXGrid / tileWidth + j >= 0 && playerLocXGrid / tileWidth + j <= tileColumn)
							{
								if (levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 0 || levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] > 1)
								{
									levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] = 1;
								}

							}
						}
					}
					for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
					{
						for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
						{
							if (playerLocYGrid / tileHeight + i >= 0 && playerLocYGrid / tileHeight + i <= tileRow && playerLocXGrid / tileWidth + j >= 0 && playerLocXGrid / tileWidth + j <= tileColumn)
							{
								if (levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 0 || levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 3)
								{
									levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] = 2;
								}
							}
						}
					}
					for (int i = -(grayViewSize); i < grayViewSize; i++)
					{
						for (int j = -(grayViewSize); j < grayViewSize; j++)
						{
							if (playerLocYGrid / tileHeight + i >= 0 && playerLocYGrid / tileHeight + i <= tileRow && playerLocXGrid / tileWidth + j >= 0 && playerLocXGrid / tileWidth + j <= tileColumn)
							{
								if (levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 0)
								{
									levelZeroExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] = 3;
								}
							}
						}
					}
					for (int row = 0; row < tileRow; row++)
					{
						for (int column = 0; column < tileColumn; column++)
						{
							int posX = static_cast<int>((column + playerDirectionX) * tileWidth);
							int posY = static_cast<int>((row + playerDirectionY) * tileHeight);

							if (levelZeroExploreMap[row][column] == 0)
							{
								DrawRectangle(posX, posY, tileWidth, tileHeight, BLACK);
							}
							else if (levelZeroExploreMap[row][column] == 2)
							{
								DrawRectangle(posX, posY, tileWidth, tileHeight, Color{ 0,0,0,50 });
							}
							else if (levelZeroExploreMap[row][column] == 3)
							{
								DrawRectangle(posX, posY, tileWidth, tileHeight, Color{ 0,0,0,150 });
							}
						}

					}
				}
				else if (level == Level::level_1)
				{
					for (int i = -(clearViewSize); i < clearViewSize; i++)
					{
						for (int j = -(clearViewSize); j < clearViewSize; j++)
						{
							if (playerLocYGrid / tileHeight + i >= 0 && playerLocYGrid / tileHeight + i <= tileRow && playerLocXGrid / tileWidth + j >= 0 && playerLocXGrid / tileWidth + j <= tileColumn)
							{
								if (levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 0 || levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] > 1)
								{
									levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] = 1;
								}

							}
						}
					}
					for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
					{
						for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
						{
							if (playerLocYGrid / tileHeight + i >= 0 && playerLocYGrid / tileHeight + i <= tileRow && playerLocXGrid / tileWidth + j >= 0 && playerLocXGrid / tileWidth + j <= tileColumn)
							{
								if (levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 0 || levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 3)
								{
									levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] = 2;
								}
							}
						}
					}
					for (int i = -(grayViewSize); i < grayViewSize; i++)
					{
						for (int j = -(grayViewSize); j < grayViewSize; j++)
						{
							if (playerLocYGrid / tileHeight + i >= 0 && playerLocYGrid / tileHeight + i <= tileRow && playerLocXGrid / tileWidth + j >= 0 && playerLocXGrid / tileWidth + j <= tileColumn)
							{
								if (levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] == 0)
								{
									levelOneExploreMap[playerLocYGrid / tileHeight + i][playerLocXGrid / tileWidth + j] = 3;
								}
							}
						}
					}
					for (int row = 0; row < tileRow; row++)
					{
						for (int column = 0; column < tileColumn; column++)
						{
							int posX = static_cast<int>((column + playerDirectionX) * tileWidth);
							int posY = static_cast<int>((row + playerDirectionY) * tileHeight);

							if (levelOneExploreMap[row][column] == 0)
							{
								DrawRectangle(posX, posY, tileWidth, tileHeight, BLACK);
							}
							else if (levelOneExploreMap[row][column] == 2)
							{
								DrawRectangle(posX, posY, tileWidth, tileHeight, Color{ 0,0,0,50 });
							}
							else if (levelOneExploreMap[row][column] == 3)
							{
								DrawRectangle(posX, posY, tileWidth, tileHeight, Color{ 0,0,0,150 });
							}
						}

					}
				}
			}
		});
}
