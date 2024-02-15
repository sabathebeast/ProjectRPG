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
	const char* id = 0;
	bool isStackable = false;
	int stackSize = 0;
	int quantity = 0;
	Texture texture;
	ItemType itemType;
};

struct Gear
{
	const char* id;
	Texture texture;
	bool isOnMouse = false;
};

class Inventory
{
public:
	void addItem(Item& item);
	void addGear(Gear& gear);
	inline const std::vector<Item> getItems() const { return items; }
	bool canAddItems = true;
	void removeOrDecreaseItems(const char* rmvid, int rmvquantity);
	void addGold(int amount);
	inline const int getGoldCount() const { return goldCount; }
	inline int setGoldCount(int amount) { return goldCount = amount; }
	void handleInventoryIsFull(SoundData& soundData);
	std::vector<Gear> gears;
	int currentToolbarSize = 0;
private:
	std::vector<Item> items;
	int currentBagSize = 0;
	int maxBagSize = 16;
	int maxToolbarSize = 10;
	bool canAddGear = true;
	void sortItems();
	int goldCount = 0;
};