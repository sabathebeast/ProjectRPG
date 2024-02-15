#include "inventory.h"
#include "soundData.h"

void Inventory::addItem(Item& item)
{
	currentBagSize++;
	if (currentBagSize <= maxBagSize)
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
		currentBagSize--;
	}
}

void Inventory::addGear(Gear& gear)
{
	currentToolbarSize++;
	if (currentToolbarSize <= maxToolbarSize)
	{
		canAddGear = true;
		gears.push_back(gear);
	}
	else
	{
		canAddGear = false;
		currentToolbarSize--;
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
			currentBagSize--;
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
