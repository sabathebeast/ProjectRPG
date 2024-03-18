#pragma once
#include <fstream>
#include <vector>
#include <string>

class GameState
{
public:

private:
	std::ofstream outputFile;
	std::ifstream inputFile;
	std::vector<std::string> inputData;
	bool saveTheGame = false;

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
						if (typeid(T) == typeid(int))
						{
							arg = std::stoi(inputData[i + 2]);
						}
						else if (typeid(T) == typeid(bool))
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
						else if (typeid(T) == typeid(float))
						{
							arg = std::stof(inputData[i + 2]);
						}
						else if (typeid(T) == typeid(std::string))
						{
							//arg = inputData[i + 2];
						}
						break;
					}
				}
			}
		}
	}
};