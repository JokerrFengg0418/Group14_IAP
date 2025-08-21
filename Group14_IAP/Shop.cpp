#include "Shop.h"
#include <iostream>
#include <cctype>

Shop::Shop(Inventory* inventoryPtr) : gameInventory(inventoryPtr)
{
    if (gameInventory == nullptr) {
        std::cerr << "Error: Shop was not provided with a valid inventory.\n";
        return; // don't populate with a null DB
    }
    populateShop();
}

Shop::~Shop() {}

// Helper to push only valid items and log misses
static inline void pushIfFound(std::vector<Item*>& v, Item* it, const char* name, char db) {
    if (it) {
        v.push_back(it);
    }
    else {
        std::cerr << "[Shop] Not found in DB '" << db << "': " << name << "\n";
    }
}

void Shop::populateShop() {
    if (gameInventory != nullptr) {
        // Weapons
        Item* sword = gameInventory->DrawDatabase('W', "    Swords    ");
        if (sword != nullptr) {
            inventory.push_back(sword);
        }
        Item* slingShot = gameInventory->DrawDatabase('W', "  SlingShots  ");
        if (slingShot != nullptr) {
            inventory.push_back(slingShot);
        }
        Item* bowAndArrow = gameInventory->DrawDatabase('W', "Bow and Arrows");
        if (bowAndArrow != nullptr) {
            inventory.push_back(bowAndArrow);
        }
        Item* mace = gameInventory->DrawDatabase('W', "     Mace     ");
        if (mace != nullptr) {
            inventory.push_back(mace);
        }
        Item* axe = gameInventory->DrawDatabase('W', "     Axee     ");
        if (axe != nullptr) {
            inventory.push_back(axe);
        }
        Item* crossbow = gameInventory->DrawDatabase('W', "   Crossbow   ");
        if (crossbow != nullptr) {
            inventory.push_back(crossbow);
        }
        Item* turret = gameInventory->DrawDatabase('W', "    Turret    ");
        if (turret != nullptr) {
            inventory.push_back(turret);
        }

        // Armor (Note: Assuming armor is in the 'A' database)
        Item* woodenArmor = gameInventory->DrawDatabase('I', " Wooden Armor ");
        if (woodenArmor != nullptr) {
            inventory.push_back(woodenArmor);
        }
        Item* silverArmor = gameInventory->DrawDatabase('I', " Silver Armor ");
        if (silverArmor != nullptr) {
            inventory.push_back(silverArmor);
        }
        Item* shield = gameInventory->DrawDatabase('I', "    Shield    ");
        if (shield != nullptr) {
            inventory.push_back(shield);
        }
        Item* helmet = gameInventory->DrawDatabase('I', "    Helmet    ");
        if (helmet != nullptr) {
            inventory.push_back(helmet);
        }
        Item* leatherArmor = gameInventory->DrawDatabase('I', "Leather Armors");
        if (leatherArmor != nullptr) {
            inventory.push_back(leatherArmor);
        }

        // Consumables / Items (Note: Assuming these are in the 'I' database)
        Item* healthPotion = gameInventory->DrawDatabase('I', "Health Potions");
        if (healthPotion != nullptr) {
            inventory.push_back(healthPotion);
        }
    }
    std::cout << "------------------------------------------\n";
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

    int choice = -1;
    std::cout << "Enter the number of the item you wish to buy (or 0 to exit): ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }

    if (choice == 0) return;
    if (choice < 1 || static_cast<size_t>(choice) > inventory.size()) {
        std::cout << "Invalid choice.\n";
        return;
    }

    Item* selectedItem = inventory[choice - 1];
    if (!selectedItem) {
        std::cout << "That item is invalid.\n";
        return;
    }

    const int cost = selectedItem->GetItemValue('S');
    if (PlayerInventory->getCurrency() < cost) {
        std::cout << "You don't have enough gold to buy that.\n";
        return;
    }

    PlayerInventory->setCurrency(PlayerInventory->getCurrency() - cost);
    PlayerInventory->setInventory(selectedItem->GetItemWord('N'), 1);
    std::cout << "You have successfully purchased a "
        << selectedItem->GetItemWord('N') << "!\n";
}

void Shop::refreshStock() {
    inventory.clear();
    populateShop();  // (optional) randomize inside here if desired
    std::cout << "Shop stock refreshed.\n";
}
