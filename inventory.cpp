#include "inventory.h"
#include "soundData.h"

void Inventory::addItem(Item& item)
{
	currentSize++;
	if (currentSize <= maxSize)
	{
		canAddItems = true;
		items.push_back(item);
		if (items.size() > 1)
		{
			sortItems();
		}
	}
	else
	{
		canAddItems = false;
		currentSize--;
	}
}

void Inventory::sortItems()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items.back().id == items[i].id && i < items.size() - 1 && (items.back().quantity + items[i].quantity) <= items[i].stackSize)
		{
			items[i].quantity = items[i].quantity + items.back().quantity;
			items.pop_back();
			currentSize--;
			break;
		}
	}
}

void Inventory::removeOrDecreaseItems(const char* rmvid, int rmvquantity)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].id == rmvid)
		{
			if (items[i].quantity - rmvquantity <= 0)
			{
				items.erase(items.begin() + i);
			}
			else
			{
				items[i].quantity -= rmvquantity;
			}
		}
	}
}

void Inventory::addGold(int amount)
{
	goldCount += amount;
}

void Inventory::handleInventoryIsFull(SoundData& soundData)
{
	if (canAddItems == false)
	{
		PlaySound(soundData.getSounds()[*Sounds::InventoryIsFull]);
		canAddItems = true;
	}
}
