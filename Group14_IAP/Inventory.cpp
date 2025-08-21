#include "Inventory.h"
#include "Item.h"
#include <iostream>
#include <string>


Inventory::Inventory()
{

	for (int i = 0; i < 50; i++) {
		ItemDatabase[i] = nullptr;
		MonsterItemDatabase[i] = nullptr;
	}
	for (int i = 0; i < 10; i++) {
		WeaponDatabase[i] = nullptr;
	}
	// Initialize databases when an Inventory object is created.
	DatabaseInitialisation();
	for (int i = 0; i < 25; i++) {
		inventory[i] = nullptr;
	}
}

Inventory::~Inventory()
{
}

//Drawing Inventory (UI Save Me)
void Inventory::DrawInventory() const{
	std::cout << "   0 1 2 3 4\n";
	std::cout << "+-----------+\n";
	for (int i = 0; i < 4; ++i) {
		std::cout << i << " ";
		for (int j = 0; j < 5; ++j) {
			int index = i * 5 + j;
			if (inventory[index] != nullptr) {
				std::cout << '|' << inventory[index]->GetItemWord('N') << ' ';
			}
			else {
				std::cout << '|' << "  ";
			}
		}
		std::cout << '|' << std::endl;
	}
	std::cout << "+-----------+\n";
}

//Draws Specifications of Inventory Item
Item* Inventory::getInventory(std::string Name) const{

	for (int i = 0; i < 25; i++) {

		if (this->inventory[i]->GetItemWord('N') == Name) {

			return inventory[i];


		}

	}

	return nullptr;
}

//Sets Item into Inventory
void Inventory::setInventory(std::string ItemName, int Number) {

	for (int i = 0; i < 25; i++) {

		if (inventory[i]->GetItemWord('N') == ItemName) {

			
			if (WeaponDatabase[i]->GetItemWord('N') == ItemName && i < 10) {
				break;
			}
			

			int CurrentValue = inventory[i]->GetItemValue('V');

			inventory[i]->SetItemValue('V', CurrentValue + Number);

			return;
		}

	}

	for (int i = 0; i < 3; i++) {

		if (i == 0 && DrawDatabase('W', ItemName) != nullptr) {
			
			for (int i = 0; i < 25; i++) {

				if (inventory[i] == nullptr) {

					inventory[i] = DrawDatabase('W', ItemName);

					return;

				}
			}
		} else if (i == 1 && DrawDatabase('M', ItemName) != nullptr) {

			for (int i = 0; i < 25; i++) {

				if (inventory[i] == nullptr) {

					inventory[i] = DrawDatabase('M', ItemName);

					return;

				}
			}
		} else if (i == 2 && DrawDatabase('I', ItemName) != nullptr) {

			for (int i = 0; i < 25; i++) {

				if (inventory[i] == nullptr) {

					inventory[i] = DrawDatabase('I', ItemName);

					return;

				}
			}
		}

	}

}

void Inventory::RemoveItemFromInventory(std::string ItemName, int Number) {

	for (int i = 0; i < 25; i++) {

		if (inventory[i]->GetItemWord('N') == ItemName) {

			for (int j = 0; j < 10; j++) {
				if (WeaponDatabase[j]->GetItemWord('N') == ItemName) {
					delete inventory[i];
					inventory[i] = nullptr;
					return;
				}
			}
			int CurrentValue = inventory[i]->GetItemValue('V');
			CurrentValue - Number;

			if (CurrentValue <= 0) {

				delete inventory[i];
				inventory[i] = nullptr;

			}
			else {

				inventory[i]->SetItemValue('V', CurrentValue);

			}

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

	case 'A':

		for (int i = 0; i < 50; i++) {

			if (MonsterItemDatabase[i] == nullptr) {

				MonsterItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);

				return;
			}

		}

		break;

	}

}

void Inventory::DatabaseInitialisation() {

	//FactoryCreateItem(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, int number, char DatabaseType)

	// ===== Weapons =====
	FactoryCreateItem("Sword", "A basic but reliable blade.", 'W', 50, 20, 30, 10, 'W');
	FactoryCreateItem("SlingShot", "A simple ranged weapon.", 'W', 50, 20, 30, 10, 'W');
	FactoryCreateItem("Bow and Arrow", "Classic ranged weapon with arrows.", 'W', 80, 30, 50, 10, 'W');
	FactoryCreateItem("Mace", "A heavy blunt weapon.", 'W', 100, 40, 60, 5, 'W');
	FactoryCreateItem("Axe", "Powerful for chopping and combat.", 'W', 250, 100, 150, 5, 'W');
	FactoryCreateItem("Crossbow", "Advanced ranged weapon.", 'W', 150, 60, 90, 5, 'W');
	FactoryCreateItem("Turret", "Stationary defense weapon.", 'W', 300, 120, 180, 2, 'W');

	// ===== Armors =====
	FactoryCreateItem("Wooden Armor", "Basic protective armor.", 'A', 50, 20, 30, 10, 'I');
	FactoryCreateItem("Silver Armor", "Strong armor for better defense.", 'A', 100, 40, 60, 5, 'I');
	FactoryCreateItem("Leather Armor", "Lightweight armor.", 'A', 10, 4, 6, 20, 'I');
	FactoryCreateItem("Shield", "Protects against attacks.", 'A', 100, 40, 60, 5, 'I');
	FactoryCreateItem("Helmet", "Protects your head.", 'A', 80, 30, 50, 5, 'I');

	// ===== Consumables / Other =====
	FactoryCreateItem("Health Potion", "Restores health.", 'I', 50, 20, 30, 20, 'I');
}

//Draws Item from Database instead of Inventory//
Item* Inventory::DrawDatabase(char DatabaseType, std::string ItemName) {

	switch (DatabaseType) {

	case 'W':

		for (int i = 0; i < 10; i++) {

			if (WeaponDatabase[i]->GetItemWord('N') == ItemName) {

				return WeaponDatabase[i];

			}

		}
		break;

	case 'I':

		for (int i = 0; i < 50; i++) {

			if (MonsterItemDatabase[i]->GetItemWord('N') == ItemName) {

				return MonsterItemDatabase[i];

			}

		}
		break;

	case 'M':

		for (int i = 0; i < 50; i++) {

			if (ItemDatabase[i]->GetItemWord('N') == ItemName) {

				return ItemDatabase[i];

			}

		}
		break;

	default:

		return nullptr;

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
	for (int i = 0; i < 10; i++) {
		if (WeaponDatabase[i]->GetItemWord('N') == SelectedItem->GetItemWord('N') ){
			EquippedItem = SelectedItem;
			return;
		}
	}

	std::cout << "You can't equip this item! \n";
	
}

Item* Inventory::PullInventoryIndex(int Index) const{

	return inventory[Index];

}
