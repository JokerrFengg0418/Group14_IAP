#pragma once
#include <string>
#include <vector>
#include "Item.h"

class Inventory
{
private:

	Item* Inventory[25];
	int Currency;
	Item* ItemDatabase[50];
	Item* EquippedItem;
	Item* MonsterItemDatabase[50];
	Item* WeaponDatabase[10];

	char InventorySlots[4][5];

public:

	void DrawInventory() const;

	Item* getInventory(std::string Name) const;

	void setInventory(std::string ItemName, int Number);

	void RemoveItemFromInventory(std::string ItemName, int Number);

	void DatabaseInitialisation();

	int getCurrency() const;

	void setCurrency(int NewAmount);

	Item* DrawDatabase(char DatabaseType, std::string ItemName);

	void FactoryCreateItem(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, char DatabaseType);

};

