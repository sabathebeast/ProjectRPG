#pragma once

#include <string>

class TextureData;
class Attributes;
class SoundData;
class Inventory;
class Scene;
struct Vector2;
class Player;

class UserInterface
{
public:
	void characterOverlayUI(int windowWidth, int windowHeight, TextureData& textureData, Attributes& attributes, std::string playerName);
	void handleOpenCloseBag(SoundData& soundData);
	void openBag(SoundData& soundData);
	void closeBag(SoundData& soundData);
	void bagUI(SoundData& soundData, Inventory& inventory, TextureData& textureData);
	void toolBarUI(int windowWidth, int windowHeight, TextureData& textureData, Attributes& attributes, Vector2 PlayerLocation, Vector2 PlayerDirection, int playerFramesY, Inventory& inventory, Scene& scene);
	void characterInfoUI(int windowWidth, int windowHeight, Attributes& attributes);
	void handleOpenCloseCharacterInfo();
	void menuAndName(Player& player, int windowWidth, int windowHeight);
	bool inMainMenu = true;

private:
	int bagRow = 4;
	int bagColumn = 4;
	bool isBagOpen = false;
	bool isCharacterInfoOpen = false;
	bool isMouseOccupied = false;
	int letterCount = 0;
	static constexpr char MAX_NAME_CHAR = 6;
	char name[MAX_NAME_CHAR + 1] = "\0";
};