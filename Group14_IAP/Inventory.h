#pragma once
#include <string>
#include <vector>
#include "Item.h"

class Inventory
{
private:

	char InventorySlots[4][5];
	Item* Inventory[25];
	Item* ItemDatabase[50];
	Item* EquippedItem;
	Item* MonsterItemDatabase[50];
	Item* WeaponDatabase[10];
	int Currency;

public:

	void DrawInventory() const;

	Item* getInventory(std::string Name) const;

	void setInventory(std::string ItemName, int Number);

	Item* getEquippedItem() const;

	void setEquippedItem(Item* ItemSelect);

	void RemoveItemFromInventory(std::string ItemName, int Number);

	void DatabaseInitialisation();

	int getCurrency() const;

	void setCurrency(int NewAmount);

	Item* DrawDatabase(char DatabaseType, std::string ItemName);

	void FactoryCreateItem(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, int number, char DatabaseType);

};

