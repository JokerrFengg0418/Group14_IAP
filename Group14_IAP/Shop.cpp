#include "Shop.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>   
#include <cctype>

Shop::Shop(Inventory* inventoryPtr) : gameInventory(inventoryPtr)
{
    if (!gameInventory) {
        std::cerr << "Error: Shop not given a valid Inventory.\n";
    }
    // Seed RNG once per process (mt19937 is better than rand)
    static bool seeded = false;
    if (!seeded) {
        std::mt19937 rng{ static_cast<unsigned>(std::time(nullptr)) };
        // store engine in a static to keep seeding meaningful if you later need it
        seeded = true;
    }

    populateShop();
}

Shop::~Shop() {}

// Helper to push only valid items
static inline void pushIfFound(std::vector<Item*>& v, Item* it) {
    if (it) v.push_back(it);
}

void Shop::populateShop()
{
    inventory.clear();
    if (!gameInventory) return;

    std::vector<std::pair<char, std::string>> candidates = {
        // Use the exact names that exist in your DB!
        {'W', "  Broadsword  "},
        {'W', "  SlingShots  "},
        {'W', "Bow and Arrows"},
        {'W', "     Mace     "},
        {'W', "  Battle Axe  "},
        {'W', "   Crossbow   "},
        {'W', "    Turret    "},

        {'A', " Wooden Armor "},
        {'A', " Silver Armor "},
        {'A', "  Gold Armor  "},
        {'A', "Leather Armors"},
        {'A', "    Shield    "},
        {'A', "    Helmet    "},

        {'I', "Health Potions"},
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(candidates.begin(), candidates.end(), gen);

    const int MAX_STOCK = 5;
    int picked = 0;

    // C++11-friendly loop (no structured bindings)
    for (const auto& p : candidates) {
        if (picked >= MAX_STOCK) break;

        char dbType = p.first;
        const std::string& name = p.second;

        Item* it = gameInventory->DrawDatabase(dbType, name);
        if (it) {
            bool exists = false;
            for (auto* s : inventory) {
                if (s == it) { exists = true; break; }
            }
            if (!exists) {
                inventory.push_back(it);
                ++picked;
            }
        }
    }
}

void Shop::refreshStock()   // <- rename to match your call site
{
    std::cout << "\nThe shop has refreshed its stock!\n";
    populateShop();
}
void Shop::displayItems()
{
    std::cout << "=== Shop Stock ===\n";
    if (inventory.empty()) {
        std::cout << "(No items available right now)\n";
    }

    for (size_t i = 0; i < inventory.size(); ++i) {
        if (!inventory[i]) continue;
        std::cout << i + 1 << ". "
            << inventory[i]->GetItemWord('N') << " - "
            << inventory[i]->GetItemWord('D') << " ("
            << inventory[i]->GetItemValue('S') << " gold)\n";
    }
    std::cout << "0. Exit\n";
}

void Shop::buyItem(Inventory* PlayerInventory)
{
    if (!PlayerInventory) {
        std::cout << "Error: Player inventory invalid.\n";
        return;
    }

    if (inventory.empty()) {
        std::cout << "Nothing to buy.\n";
        return;
    }

    int choice = -1;
    std::cout << "Enter the number of the item you wish to buy (0 to cancel): ";
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

    // Deduct gold and add to inventory
    PlayerInventory->setCurrency(PlayerInventory->getCurrency() - cost);
    PlayerInventory->setInventory(selectedItem->GetItemWord('N'), 1);

    std::cout << "You bought a " << selectedItem->GetItemWord('N') << "!\n";
}
