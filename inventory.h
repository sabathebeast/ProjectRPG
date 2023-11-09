#pragma once
#include "raylib.h"
#include <vector>

enum class ItemType : unsigned char
{
	Food,
	Collectible,
};

struct Item
{
	const char* id;
	bool isStackable = false;
	int stackSize = 0;
	int quantity = 0;
	Texture texture;
	ItemType itemType;
};

class Inventory
{
public:
	void addItem(Item& item);
	inline const std::vector<Item> getItems() const { return items; }
	bool canAddItems = true;
	void removeOrDecreaseItems(const char* rmvid, int rmvquantity);
	void addGold(int amount);
	inline const int getGoldCount() const { return goldCount; }
	inline int setGoldCount(int amount) { return goldCount = amount; }
private:
	std::vector<Item> items;
	int currentSize = 0;
	int maxSize = 16;
	void sortItems();
	int goldCount = 0;
};