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
    inventory.clear();
    if (gameInventory == nullptr) return;

    // Pull from databases (fix: Leather Armor is in 'I', not 'A')
    pushIfFound(inventory, gameInventory->DrawDatabase('W', "Iron Sword"), "Iron Sword", 'W');
    pushIfFound(inventory, gameInventory->DrawDatabase('I', "Health Potion"), "Health Potion", 'I');
    pushIfFound(inventory, gameInventory->DrawDatabase('I', "Leather Armor"), "Leather Armor", 'I');

    // If you added more items, add them here with correct DB keys
    // e.g. pushIfFound(inventory, gameInventory->DrawDatabase('W', "Sword"), "Sword", 'W');
}

void Shop::displayItems() {
    std::cout << "Welcome to the shop! What would you like to buy?\n";
    std::cout << "------------------------------------------\n";

    if (inventory.empty()) {
        std::cout << "(No stock available)\n";
        std::cout << "------------------------------------------\n";
        return;
    }

    for (size_t i = 0; i < inventory.size(); ++i) {
        Item* it = inventory[i];
        if (!it) {
            std::cout << i + 1 << ". <invalid item>\n";
            continue;
        }
        std::cout << i + 1 << ". "
            << it->GetItemWord('N') << " - "
            << it->GetItemWord('D') << " ("
            << it->GetItemValue('S') << " gold)\n";
    }
    std::cout << "------------------------------------------\n";
}

void Shop::buyItem(Inventory* playerInv) {
    if (playerInv == nullptr) {
        std::cout << "Error: Player inventory is invalid.\n";
        return;
    }
    if (inventory.empty()) {
        std::cout << "The shop has no items in stock.\n";
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
    if (playerInv->getCurrency() < cost) {
        std::cout << "You don't have enough gold to buy that.\n";
        return;
    }

    playerInv->setCurrency(playerInv->getCurrency() - cost);
    playerInv->setInventory(selectedItem->GetItemWord('N'), 1);
    std::cout << "You have successfully purchased a "
        << selectedItem->GetItemWord('N') << "!\n";
}

void Shop::refreshStock() {
    inventory.clear();
    populateShop();  // (optional) randomize inside here if desired
    std::cout << "Shop stock refreshed.\n";
}
