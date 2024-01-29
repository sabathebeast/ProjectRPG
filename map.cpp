#include "map.h"
#include <string>
#include "entity.h"
#include "components.h"


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

void Map::addMapExlore(const Scene& scene, Level level, float playerDirectionX, float playerDirectionY)
{
	entt::basic_view playerView = scene.registry.view<const TagComponent, const PositionComponent, const Sprite2DComponent, const ActiveComponent>();
	playerView.each([this, level, playerDirectionX, playerDirectionY](const TagComponent& tag, const PositionComponent& position, const Sprite2DComponent& sprite, const ActiveComponent& active)
		{
			if (tag.tag == "player")
			{
				int playerLocXGrid = static_cast<int>(position.x - (playerDirectionX * tileSize));
				int playerLocYGrid = static_cast<int>(position.y - (playerDirectionY * tileSize));

				if (level == Level::level_0)
				{
					for (int i = -(clearViewSize); i < clearViewSize; i++)
					{
						for (int j = -(clearViewSize); j < clearViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] > 1)
								{
									levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 1;
								}

							}
						}
					}
					for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
					{
						for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 3)
								{
									levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 2;
								}
							}
						}
					}
					for (int i = -(grayViewSize); i < grayViewSize; i++)
					{
						for (int j = -(grayViewSize); j < grayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0)
								{
									levelZeroExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 3;
								}
							}
						}
					}
					for (int row = 0; row < tileRow; row++)
					{
						for (int column = 0; column < tileColumn; column++)
						{
							int posX = static_cast<int>((column + playerDirectionX) * tileSize);
							int posY = static_cast<int>((row + playerDirectionY) * tileSize);

							if (levelZeroExploreMap[row][column] == 0)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, BLACK);
							}
							else if (levelZeroExploreMap[row][column] == 2)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,50 });
							}
							else if (levelZeroExploreMap[row][column] == 3)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,150 });
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
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] > 1)
								{
									levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 1;
								}

							}
						}
					}
					for (int i = -(clearGrayViewSize); i < clearGrayViewSize; i++)
					{
						for (int j = -(clearGrayViewSize); j < clearGrayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0 || levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 3)
								{
									levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 2;
								}
							}
						}
					}
					for (int i = -(grayViewSize); i < grayViewSize; i++)
					{
						for (int j = -(grayViewSize); j < grayViewSize; j++)
						{
							if (playerLocYGrid / tileSize + i >= 0 && playerLocYGrid / tileSize + i <= tileRow && playerLocXGrid / tileSize + j >= 0 && playerLocXGrid / tileSize + j <= tileColumn)
							{
								if (levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] == 0)
								{
									levelOneExploreMap[playerLocYGrid / tileSize + i][playerLocXGrid / tileSize + j] = 3;
								}
							}
						}
					}
					for (int row = 0; row < tileRow; row++)
					{
						for (int column = 0; column < tileColumn; column++)
						{
							int posX = static_cast<int>((column + playerDirectionX) * tileSize);
							int posY = static_cast<int>((row + playerDirectionY) * tileSize);

							if (levelOneExploreMap[row][column] == 0)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, BLACK);
							}
							else if (levelOneExploreMap[row][column] == 2)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,50 });
							}
							else if (levelOneExploreMap[row][column] == 3)
							{
								DrawRectangle(posX, posY, tileSize, tileSize, Color{ 0,0,0,150 });
							}
						}

					}
				}
			}
		});
}
