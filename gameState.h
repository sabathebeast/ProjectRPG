#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <type_traits>

class Inventory;
class TextureData;
class Player;
class Attributes;

class GameState
{
public:
	GameState() {}
	~GameState() {}

	void loadGame(Inventory& inventory, TextureData& textureData, Player& player, int& questRetruntValue, Attributes& attributes);
	void saveGame(Inventory& inventory, Player& player, int& questRetruntValue, Attributes& attributes);

private:
	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;
	bool saveTheGame = false;

	void addEverythingToSaveGame(Player& player, Inventory& inventory, int& questRetruntValue, Attributes& attributes);

	template <typename A, typename B, typename C, typename D, typename E>
	void addToInventory(A arg0, B arg1, C arg2, D arg3, E arg4)
	{
		outputFile << arg0 << " " << ":" << " " << arg1 << " " << arg2 << " " << arg3 << " " << arg4 << "\n";
	}

	template <typename T>
	void addToSaveGame(const char* name, T& arg)
	{
		if (saveTheGame)
		{
			outputFile << name << " " << ":" << " " << arg << "\n";
		}
		else // load the game from saveGame.txt
		{
			if (!inputData.empty())
			{
				for (int i = 0; i < inputData.size(); i += 3)
				{
					if (inputData[i] == name)
					{
						if constexpr (std::is_same<T, int>())
						{
							arg = std::stoi(inputData[i + 2]);
						}
						else if constexpr (std::is_same<T, bool>())
						{
							if (std::stoi(inputData[i + 2]) == 1)
							{
								arg = true;
							}
							else
							{
								arg = false;
							}
						}
						else if constexpr (std::is_same<T, float>())
						{
							arg = std::stof(inputData[i + 2]);
						}
						else if constexpr (std::is_same<T, std::string>())
						{
							arg = inputData[i + 2];
						}
						break;
					}
				}
			}
		}
	}
};