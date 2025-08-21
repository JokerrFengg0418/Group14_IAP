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
    if (gameInventory != nullptr) {
        // Weapons
        Item* sword = gameInventory->DrawDatabase('W', "Sword");
        if (sword != nullptr) {
            inventory.push_back(sword);
        }
        Item* slingShot = gameInventory->DrawDatabase('W', "SlingShot");
        if (slingShot != nullptr) {
            inventory.push_back(slingShot);
        }
        Item* bowAndArrow = gameInventory->DrawDatabase('W', "Bow and Arrow");
        if (bowAndArrow != nullptr) {
            inventory.push_back(bowAndArrow);
        }
        Item* mace = gameInventory->DrawDatabase('W', "Mace");
        if (mace != nullptr) {
            inventory.push_back(mace);
        }
        Item* axe = gameInventory->DrawDatabase('W', "Axe");
        if (axe != nullptr) {
            inventory.push_back(axe);
        }
        Item* crossbow = gameInventory->DrawDatabase('W', "Crossbow");
        if (crossbow != nullptr) {
            inventory.push_back(crossbow);
        }
        Item* turret = gameInventory->DrawDatabase('W', "Turret");
        if (turret != nullptr) {
            inventory.push_back(turret);
        }

        // Armor (Note: Assuming armor is in the 'A' database)
        Item* woodenArmor = gameInventory->DrawDatabase('A', "Wooden Armor");
        if (woodenArmor != nullptr) {
            inventory.push_back(woodenArmor);
        }
        Item* silverArmor = gameInventory->DrawDatabase('A', "Silver Armor");
        if (silverArmor != nullptr) {
            inventory.push_back(silverArmor);
        }
        Item* shield = gameInventory->DrawDatabase('A', "Shield");
        if (shield != nullptr) {
            inventory.push_back(shield);
        }
        Item* helmet = gameInventory->DrawDatabase('A', "Helmet");
        if (helmet != nullptr) {
            inventory.push_back(helmet);
        }
        Item* leatherArmor = gameInventory->DrawDatabase('A', "Leather Armor");
        if (leatherArmor != nullptr) {
            inventory.push_back(leatherArmor);
        }

        // Consumables / Items (Note: Assuming these are in the 'I' database)
        Item* healthPotion = gameInventory->DrawDatabase('I', "Health Potion");
        if (healthPotion != nullptr) {
            inventory.push_back(healthPotion);
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

void Shop::refreshStock() {
    // We only store pointers owned by Inventory DBs, so clearing is safe
    inventory.clear();
    populateShop();   // Re-fill (you can add randomness inside populateShop if desired)
}
