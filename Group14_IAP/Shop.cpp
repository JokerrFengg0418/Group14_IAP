#include "Shop.h"
#include <iostream>
#include "Option.h"

Shop::Shop(Inventory* inventoryPtr) : gameInventory(inventoryPtr) 
{
    if (gameInventory == nullptr) {
        std::cerr << "Error: Shop was not provided with a valid inventory." << std::endl;
    }
    populateShop();
}

Shop::~Shop()
{
}

void Shop::populateShop() {
    // Adding some example items to the shop's inventory
    if (gameInventory != nullptr) {
        // Populate the shop with items from your databases.
        // For example, pulling an item named "Iron Sword" from the 'W'eapon database.
        Item* ironSword = gameInventory->DrawDatabase('W', "Iron Sword");
		if (ironSword != nullptr) {
			inventory.push_back(ironSword);
		}

        // Add more items as needed, for example, from the 'M'onster or 'I'tem databases.
        Item* healthPotion = gameInventory->DrawDatabase('I', "Health Potion");
        if (healthPotion != nullptr) {
            inventory.push_back(healthPotion);
        }

        Item* leatherArmor = gameInventory->DrawDatabase('A', "Leather Armor");
        if (leatherArmor != nullptr) {
            inventory.push_back(leatherArmor);
        }
    }
}

void Shop::displayItems() {
    std::cout << "Welcome to the shop! What would you like to buy?" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << inventory[i]->GetItemWord('N')
            << " - " << inventory[i]->GetItemWord('D')
            << " (" << inventory[i]->GetItemValue('S') << " gold)" << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}
void Shop::buyItem(Inventory* PlayerInventory) {
    if (PlayerInventory == nullptr) {
        std::cout << "Error: Player object is invalid." << std::endl;
        return;
    }

    int choice;
    std::cout << "Enter the number of the item you wish to buy (or 0 to exit): ";
    std::cin >> choice;

    if (choice > 0 && choice <= inventory.size()) {
        Item* selectedItem = inventory[choice - 1];
        if ((PlayerInventory->getCurrency() >= selectedItem->GetItemValue('S'))) {

            // Use the setCurrency() function on the player's Inventory object
            PlayerInventory->setCurrency(PlayerInventory->getCurrency() - selectedItem->GetItemValue('S'));

            // Add the item to the player's inventory here
            PlayerInventory->setInventory(selectedItem->GetItemWord('N'), 1);


            std::cout << "You have successfully purchased a " << selectedItem->GetItemWord('N') << "!" << std::endl;
        }
        else {
            std::cout << "You don't have enough gold to buy that." << std::endl;
        }
    }
    else if (choice != 0) {
        std::cout << "Invalid choice." << std::endl;
    }
}
