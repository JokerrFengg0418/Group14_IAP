#include "Inventory.h"
#include "Item.h"
#include <iostream>
#include <string>

//Drawing Inventory (UI Save Me)
void Inventory::DrawInventory() const{
	std::cout << "   0 1 2 3 4\n";
	std::cout << "+-----------+\n";
	for (int i = 0; i < 4; ++i) {
		std::cout << i << " ";
		for (int j = 0; j < 5; ++j) {
			std::cout << '|' << InventorySlots[i][j] << ' ';
		}
		std::cout << '|' << std::endl;
	}
	std::cout << "+-----------+\n";
}

//Draws Specifications of Inventory Item
Item* Inventory::getInventory(std::string Name) const{

	for (int i = 0; i < 25; i++) {

		if (this->Inventory[i]->GetItemWord('N') == Name) {

			return Inventory[i];


		}

	}

	return nullptr;
}

//Sets Item into Inventory
void Inventory::setInventory(std::string ItemName, int Number) {

	for (int i = 0; i < 25; i++) {

		if (Inventory[i]->GetItemWord('N') == ItemName) {

			int CurrentValue = Inventory[i]->GetItemValue('V');

			Inventory[i]->SetItemValue('V', CurrentValue + Number);

			return;
		}

	}

	for (int i = 0; i < 3; i++) {

		if (i == 0 && DrawDatabase('W', ItemName) != nullptr) {
			
			for (int i = 0; i < 25; i++) {

				if (Inventory[i] == nullptr) {

					Inventory[i] = DrawDatabase('W', ItemName);

					return;

				}
			}
		} else if (i == 1 && DrawDatabase('M', ItemName) != nullptr) {

			for (int i = 0; i < 25; i++) {

				if (Inventory[i] == nullptr) {

					Inventory[i] = DrawDatabase('M', ItemName);

					return;

				}
			}
		} else if (i == 2 && DrawDatabase('I', ItemName) != nullptr) {

			for (int i = 0; i < 25; i++) {

				if (Inventory[i] == nullptr) {

					Inventory[i] = DrawDatabase('I', ItemName);

					return;

				}
			}
		}

	}

}

void Inventory::RemoveItemFromInventory(std::string ItemName, int Number) {

	for (int i = 0; i < 25; i++) {

		if (Inventory[i]->GetItemWord('N') == ItemName) {

			int CurrentValue = Inventory[i]->GetItemValue('V');
			CurrentValue - Number;

			if (CurrentValue <= 0) {

				delete Inventory[i];
				Inventory[i] = nullptr;

			}
			else {

				Inventory[i]->SetItemValue('V', CurrentValue);

			}

		}

	}

}

void Inventory::FactoryCreateItem(std::string ItemName, std::string ItemDescription, char Type, int Value, int ResaleValue, int SaleValue, char DatabaseType) {

	switch (DatabaseType) {

	case 'W':

		for (int i = 0; i < 10; i++) {

			if (WeaponDatabase[i] == nullptr) {

				WeaponDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue);

				return;
			}
		}

		break;

	case 'I':

		for (int i = 0; i < 50; i++) {

			if (ItemDatabase[i] == nullptr) {

				ItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue);

				return;
			}

		}

		break;

	case 'M':

		for (int i = 0; i < 50; i++) {

			if (MonsterItemDatabase[i] == nullptr) {

				MonsterItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue);

				return;
			}

		}

		break;

	}

}

void Inventory::DatabaseInitialisation() {

	//Weapon Section here

	//Monster Section here

	//Item Section here


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

	case 'I':

		for (int i = 0; i < 50; i++) {

			if (MonsterItemDatabase[i]->GetItemWord('N') == ItemName) {

				return MonsterItemDatabase[i];

			}

		}

	case 'M':

		for (int i = 0; i < 50; i++) {

			if (ItemDatabase[i]->GetItemWord('N') == ItemName) {

				return ItemDatabase[i];

			}

		}

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


