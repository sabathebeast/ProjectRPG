#include "gameState.h"
#include "inventory.h"
#include "textureData.h"
#include "player.h"
#include "attributes.h"

void GameState::loadGame(Inventory& inventory, TextureData& textureData, Player& player, int& questRetruntValue, Attributes& attributes)
{
	saveTheGame = false;

	inputFile.open("saveGame.txt");
	std::string input;

	if (!inputFile.fail())
	{
		while (inputFile >> input)
		{
			inputData.push_back(input);
		}
	}
	inputFile.close();

	addEverythingToSaveGame(player, inventory, questRetruntValue, attributes);

	// load inventory
	inputFile.open("inventory.txt");
	inputData.clear();

	if (!inputFile.fail())
	{
		while (inputFile >> input)
		{
			inputData.push_back(input);
		}
		if (!inputData.empty())
		{
			for (int i = 0; i < inputData.size(); i += 6)
			{
				if (inputData[i] == "fish")
				{
					Item fish;
					fish.id = "fish";
					fish.isStackable = std::stoi(inputData[i + 2]);
					fish.stackSize = std::stoi(inputData[i + 3]);
					fish.quantity = std::stoi(inputData[i + 4]);
					fish.itemType = (ItemType)std::stoi(inputData[i + 5]);
					fish.texture = textureData.getTextures()[*Textures::Fish];
					inventory.addItem(fish);
					continue;
				}
				else if (inputData[i] == "barrel")
				{
					Item barrel;
					barrel.id = "barrel";
					barrel.isStackable = std::stoi(inputData[i + 2]);
					barrel.stackSize = std::stoi(inputData[i + 3]);
					barrel.quantity = std::stoi(inputData[i + 4]);
					barrel.itemType = (ItemType)std::stoi(inputData[i + 5]);
					barrel.texture = textureData.getTextures()[*Textures::Barrel];
					inventory.addItem(barrel);
					continue;
				}
				else if (inputData[i] == "woodStash")
				{
					Item woodStash;
					woodStash.id = "woodStash";
					woodStash.isStackable = std::stoi(inputData[i + 2]);
					woodStash.stackSize = std::stoi(inputData[i + 3]);
					woodStash.quantity = std::stoi(inputData[i + 4]);
					woodStash.itemType = (ItemType)std::stoi(inputData[i + 5]);
					woodStash.texture = textureData.getTextures()[*Textures::WoodStash];
					inventory.addItem(woodStash);
					continue;
				}
			}
		}
	}
	inputFile.close();
}

void GameState::saveGame(Inventory& inventory, Player& player, int& questRetruntValue, Attributes& attributes)
{
	saveTheGame = true;

	outputFile.open("saveGame.txt");
	if (!outputFile.fail())
	{
		addEverythingToSaveGame(player, inventory, questRetruntValue, attributes);
	}
	outputFile.close();

	outputFile.open("inventory.txt");
	if (!outputFile.fail())
	{
		for (int i = 0; i < inventory.getItems().size(); i++)
		{
			addToInventory(inventory.getItems()[i].id, inventory.getItems()[i].isStackable, inventory.getItems()[i].stackSize, inventory.getItems()[i].quantity, (int)inventory.getItems()[i].itemType);
		}
	}
	outputFile.close();
}

void GameState::addEverythingToSaveGame(Player& player, Inventory& inventory, int& questRetruntValue, Attributes& attributes)
{
	addToSaveGame("name", player.name);
	addToSaveGame("playerLocationX", player.location.x);
	addToSaveGame("playerLocationY", player.location.y);
	addToSaveGame("playerDirectionX", player.direction.x);
	addToSaveGame("isNameGiven", player.isNameGiven);
	addToSaveGame("playerDirectionY", player.direction.y);
	addToSaveGame("gold", inventory.goldCount);
	addToSaveGame("questReturnValue", questRetruntValue);
	addToSaveGame("xpCount", attributes.xpCount);
	addToSaveGame("playerLevel", attributes.playerLevel);
	addToSaveGame("levelXP", attributes.levelXP);
	addToSaveGame("strenght", attributes.strenght);
	addToSaveGame("extraStrenght", attributes.extraStrenght);
	addToSaveGame("agility", attributes.agility);
	addToSaveGame("extraAgility", attributes.extraAgility);
	addToSaveGame("stamina", attributes.stamina);
	addToSaveGame("extraStamina", attributes.extraStamina);
	addToSaveGame("currentHealth", attributes.currentHealth);
	addToSaveGame("maxHealth", attributes.maxHealth);
	addToSaveGame("currentEnergy", attributes.currentEnergy);
	addToSaveGame("maxEnergy", attributes.maxEnergy);
	addToSaveGame("talentPoints", attributes.talentPoints);
	//addToSaveGame("queststate", (int)queststate);
	//addToSaveGame<level(int)>("level", level);*/
}
