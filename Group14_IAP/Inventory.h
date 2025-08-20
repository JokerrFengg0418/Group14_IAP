#pragma once
class Inventory
{
private:

	Item* Inventory[25];
	int Currency;
	Item* ItemDatabase[50];
	Item* EquippedItem;
	Item* MonsterItemDatabase[50];
	Item* WeaponDatabase[10];

public:

	void DrawInventory() const;

	Item* GetInventory(std::string Name) const;

	void SetInventory(std::string ItemName, int Number);

	void RemoveItemFromInventory(std::string ItemName, int Number);

	void DatabaseInitialisation();

	int GetCurrency() const;

	void SetCurrency(int NewAmount);

	Item* DrawDatabase(char DatabaseType, std::string ItemName);

	void UseItem(std::string ItemName);

};

