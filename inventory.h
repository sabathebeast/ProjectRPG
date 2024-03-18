#pragma once

#include "raylib.h"
#include <vector>

class SoundData;

enum class ItemType : unsigned char
{
	Food,
	Collectible,
};

struct Item
{
	const char* id = "";
	bool isStackable = false;
	int stackSize = 0;
	int quantity = 0;
	Texture texture;
	ItemType itemType;
};

struct Gear
{
	const char* id = "";
	Texture texture;
	bool isOnMouse = false;
};

class Inventory
{
public:
	Inventory() {}
	~Inventory() {}
	friend class GameState;

	int currentToolbarSize = 0;
	std::vector<Gear> gears;
	bool canAddItems = true;

	inline const std::vector<Item> getItems() const { return items; }
	inline const int getGoldCount() const { return goldCount; }
	inline int setGoldCount(int amount) { return goldCount = amount; }

	void addItem(Item& item);
	void addGear(Gear& gear);
	void removeOrDecreaseItems(const char* rmvid, int rmvquantity);
	void addGold(int amount);
	void handleInventoryIsFull(SoundData& soundData);

private:
	int goldCount = 0;
	int currentBagSize = 0;
	int maxBagSize = 16;
	int maxToolbarSize = 10;
	bool canAddGear = true;

	std::vector<Item> items;

	void sortItems();
};