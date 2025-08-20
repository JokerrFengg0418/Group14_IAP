#include "Shop.h"
#include <iostream>

Shop::Shop()
{
}

Shop::~Shop()
{
    for (Item* item : inventory) {
        delete item;
    }
}

void Shop::populateShop() {
    // Adding some example items to the shop's inventory
    inventory.push_back(new Item("Health Potion", "Restores health.", 'P', 50, 20, 30));
    inventory.push_back(new Item("Iron Sword", "A sturdy sword.", 'W', 100, 40, 60));
    inventory.push_back(new Item("Leather Armor", "Lightweight armor.", 'A', 75, 30, 45));
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
void Shop::buyItem(Player* player) {
    if (player == nullptr) {
        std::cout << "Error: Player object is invalid." << std::endl;
        return;
    }

    int choice;
    std::cout << "Enter the number of the item you wish to buy (or 0 to exit): ";
    std::cin >> choice;

    if (choice > 0 && choice <= inventory.size()) {
        Item* selectedItem = inventory[choice - 1];
        if ((player->getInventory().getCurrency() >= selectedItem->GetItemValue('S'))){

            // Use the setCurrency() function on the player's Inventory object
            player->getInventory().setCurrency(player->getInventory().getCurrency() - selectedItem->GetItemValue('S'));

            // You'll also need to add the item to the player's inventory here
            // player->getInventory()->setInventory(selectedItem->GetItemWord('N'), 1);


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
