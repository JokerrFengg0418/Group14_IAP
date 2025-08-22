#include "Inventory.h"
#include "Item.h"
#include <iostream>
#include <string>


Inventory::Inventory()
	: Currency(0), EquippedItem(nullptr)
{
	for (int i = 0; i < 50; ++i) { ItemDatabase[i] = nullptr; MonsterItemDatabase[i] = nullptr; }
	for (int i = 0; i < 10; ++i) { WeaponDatabase[i] = nullptr; }
	for (int i = 0; i < 25; ++i) { inventory[i] = nullptr; }

	// Build DB after arrays are null-inited
	DatabaseInitialisation();
}

Inventory::~Inventory()
{
}

//Drawing Inventory (UI Save Me)
void Inventory::DrawInventory() const{
	std::cout << "+--------------------------------------------------------------------------+\n";
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			int index = i * 5 + j;
			if (inventory[index] != nullptr) {
				std::cout << '|' << inventory[index]->GetItemWord('N');
			}
			else {
				std::cout << '|' << "              ";
			}
		}
		std::cout << '|' << std::endl;
	}
	std::cout << "+--------------------------------------------------------------------------+\n";
}

//Draws Specifications of Inventory Item
Item* Inventory::getInventory(std::string Name) const {
	for (int i = 0; i < 25; ++i) {
		Item* it = inventory[i];
		if (it && it->GetItemWord('N') == Name) return it;
	}
	return nullptr;
}

//Sets Item into Inventory
void Inventory::setInventory(std::string ItemName, int Number) {
	if (Number <= 0) return;

	// Is it already in inventory? (stack only if not a weapon)
	for (int i = 0; i < 25; ++i) {
		Item* it = inventory[i];
		if (it && it->GetItemWord('N') == ItemName) {
			bool isWeapon = (DrawDatabase('W', ItemName) != nullptr);
			if (!isWeapon) {
				int cur = it->GetItemValue('V');
				it->SetItemValue('V', cur + Number);
			}
			return;
		}
	}

	// Find the source pointer in DBs
	Item* src = DrawDatabase('W', ItemName);
	if (!src) src = DrawDatabase('I', ItemName);
	if (!src) src = DrawDatabase('M', ItemName);
	if (!src) {
		std::cerr << "[Inventory] Unknown item: " << ItemName << "\n";
		return;
	}

	// Put in first empty slot (player holds pointer owned by DB)
	for (int i = 0; i < 25; ++i) {
		if (inventory[i] == nullptr) { inventory[i] = src; return; }
	}
	std::cout << "Inventory is full.\n";
}

void Inventory::RemoveItemFromInventory(std::string ItemName, int Number) {
	if (Number <= 0) return;

	for (int i = 0; i < 25; ++i) {
		Item* it = inventory[i];
		if (!it) continue;
		if (it->GetItemWord('N') == ItemName) {
			bool isWeapon = (DrawDatabase('W', ItemName) != nullptr);

			if (isWeapon) {                 // non-stackable: just clear the slot
				inventory[i] = nullptr;     // DO NOT delete; DB owns it
				return;
			}

			int cur = it->GetItemValue('V') - Number;
			if (cur <= 0) inventory[i] = nullptr;
			else          it->SetItemValue('V', cur);
			return;
		}
	}
}

void Inventory::FactoryCreateItem(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, int number, char DatabaseType) {

	switch (DatabaseType) {

	case 'W':

		for (int i = 0; i < 10; i++) {

			if (WeaponDatabase[i] == nullptr) {

				WeaponDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);

				return;
			}
		}

		break;

	case 'I':

		for (int i = 0; i < 50; i++) {

			if (ItemDatabase[i] == nullptr) {

				ItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);

				return;
			}

		}

		break;

	case 'M':

		for (int i = 0; i < 50; i++) {

			if (MonsterItemDatabase[i] == nullptr) {

				MonsterItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);

				return;
			}

		}

		break;

	}

}

Item* Inventory::FindItemByName(const std::string& itemName)
{
	for (int i = 0; i < 25; ++i) {
		Item* it = inventory[i];
		if (it) {
			// The item name in your database has leading/trailing spaces.
			// Using find() is a flexible way to check for a partial match.
			// For example, "Broadsword" will match "  Broadsword  "
			if (it->GetItemWord('N').find(itemName) != std::string::npos) {
				return it;
			}
		}
	}
	return nullptr;
}

void Inventory::DatabaseInitialisation() {

	//FactoryCreateItem(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, int number, char DatabaseType)

	// ===== Weapons =====
	FactoryCreateItem("  Broadsword  ", "A basic but reliable blade.", 'W', 50, 20, 30, 10, 'W');
	FactoryCreateItem("  SlingShots  ", "A simple ranged weapon.", 'W', 50, 20, 30, 10, 'W');
	FactoryCreateItem("Bow and Arrows", "Classic ranged weapon with arrows.", 'W', 80, 30, 50, 10, 'W');
	FactoryCreateItem("     Mace     ", "A heavy blunt weapon.", 'W', 100, 40, 60, 5, 'W');
	FactoryCreateItem("     Axee     ", "Powerful for chopping and combat.", 'W', 250, 100, 150, 5, 'W');
	FactoryCreateItem("   Crossbow   ", "Advanced ranged weapon.", 'W', 150, 60, 90, 5, 'W');
	FactoryCreateItem("    Turret    ", "Stationary defense weapon.", 'W', 300, 120, 180, 2, 'W');

	// ===== Armors =====
	FactoryCreateItem(" Wooden Armor ", "Basic protective armor.", 'A', 50, 20, 30, 10, 'I');
	FactoryCreateItem(" Silver Armor ", "Strong armor for better defense.", 'A', 100, 40, 60, 5, 'I');
	FactoryCreateItem("Leather Armors", "Lightweight armor.", 'A', 10, 4, 6, 20, 'I');
	FactoryCreateItem("    Shield    ", "Protects against attacks.", 'A', 100, 40, 60, 5, 'I');
	FactoryCreateItem("    Helmet    ", "Protects your head.", 'A', 80, 30, 50, 5, 'I');

	// ===== Consumables / Other =====
	FactoryCreateItem("Health Potions", "Restores health.", 'I', 50, 20, 30, 20, 'I');
}

//Draws Item from Database instead of Inventory//
Item* Inventory::DrawDatabase(char DatabaseType, std::string ItemName) {
	switch (DatabaseType) {
	case 'W':
		for (int i = 0; i < 10; ++i) {
			Item* it = WeaponDatabase[i];
			if (it && it->GetItemWord('N') == ItemName) return it;
		}
		break;

	case 'I': // Items (consumables/armors)  <-- was wrong before
		for (int i = 0; i < 50; ++i) {
			Item* it = ItemDatabase[i];
			if (it && it->GetItemWord('N') == ItemName) return it;
		}
		break;

	case 'M': // Monster drops                <-- was wrong before
		for (int i = 0; i < 50; ++i) {
			Item* it = MonsterItemDatabase[i];
			if (it && it->GetItemWord('N') == ItemName) return it;
		}
		break;
	}
	return nullptr;
}

int Inventory::getCurrency() const {

	return Currency;

}

void Inventory::setCurrency(int NewAmount) {

	Currency = NewAmount;
}

Item* Inventory::getEquippedItem() const {
	return EquippedItem;
}

void Inventory::setEquippedItem(Item* SelectedItem) {
	if (!SelectedItem) {
		std::cout << "Invalid item selected." << std::endl;
		return;
	}

	// Check if the selected item exists in the WeaponDatabase.
	// This correctly validates if the item is a weapon that can be equipped.
	Item* w = DrawDatabase('W', SelectedItem->GetItemWord('N'));
	if (w) {
		EquippedItem = SelectedItem;
		return;
	}

	Item* i = DrawDatabase('I', SelectedItem->GetItemWord('N'));
	if (i) {
		EquippedItem = SelectedItem;
		return;
	}

	std::cout << "You can't equip this item!\n";
}

Item* Inventory::PullInventoryIndex(int Index) const {
	if (Index < 0 || Index >= 25) return nullptr;
	return inventory[Index];
}
