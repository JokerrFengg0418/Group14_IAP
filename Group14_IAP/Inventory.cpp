#include "Inventory.h"
#include "Item.h"

//Drawing Inventory (UI Save Me)
void Inventory::DrawInventory() const{



}

//Draws Specifications of Inventory Item
Item* Inventory::GetInventory(std::string Name) const{

	for (int i = 0; i < 25; i++) {

		if (this->Inventory[i]->GetItemWord('N') == Name) {

			return Inventory[i];


		}

	}

	return nullptr;
}

//Sets Item into Inventory
void Inventory::SetInventory(std::string ItemName, int Number) {

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

void RemoveItemFromInventory(std::string ItemName, int Number) {



}



//Draws Item from Database instead of Inventory
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
