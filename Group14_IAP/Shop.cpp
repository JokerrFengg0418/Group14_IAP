#include "Shop.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <iomanip>
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

static inline std::string clip(const std::string& s, size_t n) {
    if (s.size() <= n) return s;
    return s.substr(0, n - 3) + "...";
}

// ---- helpers for name matching ----
static std::string toLowerCopy(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
static std::string trimCopy(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    return s.substr(b, e - b + 1);
}
// normalize: trim + lowercase (to compare user input with padded names)
static std::string norm(const std::string& s) { return toLowerCopy(trimCopy(s)); }

void Shop::populateShop()
{
    inventory.clear();
    if (!gameInventory) return;

    std::vector<std::pair<char, std::string>> candidates = {
        // Use the exact names that exist in your DB!
        {'W', "   Broadsword   "},
        {'W', "   SlingShots   "},
        {'W', " Bow and Arrows "},
        {'W', "      Mace      "},
        {'W', "   Battle Axe   "},
        {'W', "    Crossbow    "},
        {'W', "     Turret     "},

        {'A', "  Wooden Armor  "},
        {'A', "  Silver Armor  "},
        {'A', "   Gold Armor   "},
        {'A', " Leather Armors "},
        {'A', "     Shield     "},
        {'A', "     Helmet     "},

        {'I', " Health Potion  "},
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

void Shop::sellItem(Inventory* PlayerInventory)
{
    if (!PlayerInventory) {
        std::cout << "Error: Player inventory invalid.\n";
        return;
    }

    // Build a list of distinct names currently in the inventory
    std::vector<std::string> distinctNames;
    for (int i = 0; i < 25; ++i) {
        Item* it = PlayerInventory->PullInventoryIndex(i);
        if (!it) continue;
        const std::string nm = it->GetItemWord('N');
        if (nm.empty()) continue;
        bool seen = false;
        for (auto& s : distinctNames) if (s == nm) { seen = true; break; }
        if (!seen) distinctNames.push_back(nm);
    }

    if (distinctNames.empty()) {
        std::cout << "You have nothing to sell.\n";
        return;
    }

    // Show a formatted list of SELLABLE items (R > 0), with description, qty, and values
    std::vector<std::string> sellableNames;

    std::cout << "\nItems you can sell:\n";
    std::cout << std::left
        << std::setw(24) << "Name"
        << std::setw(32) << "Description"
        << std::right
        << std::setw(8) << "Qty"
        << std::setw(14) << "Value"
        << std::setw(14) << "Total Value"
        << "\n";
    std::cout << std::string(92, '-') << "\n";

    for (auto& nm : distinctNames) {
        // Find any one slot pointer for resale value & description
        Item* any = nullptr;
        for (int i = 0; i < 25 && !any; ++i) {
            Item* it = PlayerInventory->PullInventoryIndex(i);
            if (it && it->GetItemWord('N') == nm) any = it;
        }
        if (!any) continue;

        const int resalePer = any->GetItemValue('R');
        if (resalePer <= 0) continue; // not buyable by shop -> skip

        // Compute available quantity:
        // - If item appears in multiple slots -> treat as non-stackable => qty = slot count
        // - Else single slot: if V>1 treat as stack size, else qty=1
        int slotsWithName = 0, sumV = 0;
        for (int i = 0; i < 25; ++i) {
            Item* it = PlayerInventory->PullInventoryIndex(i);
            if (!it || it->GetItemWord('N') != nm) continue;
            ++slotsWithName;
            sumV += std::max(0, it->GetItemValue('V'));
        }
        int available = (slotsWithName <= 1) ? std::max(1, sumV) : slotsWithName;
        if (available <= 0) continue;

        sellableNames.push_back(nm);

        const std::string desc = any->GetItemWord('D');
        long long total = static_cast<long long>(resalePer) * available;

        std::cout << std::left
            << std::setw(24) << nm
            << std::setw(32) << clip(desc, 32)
            << std::right
            << std::setw(8) << available
            << std::setw(14) << resalePer
            << std::setw(14) << total
            << "\n";
    }

    if (sellableNames.empty()) {
        std::cout << "(Nothing here is sellable.)\n";
        return;
    }

    // Prompt: item name
    std::cout << "\nEnter the item name to sell: ";
    std::string typed;
    std::getline(std::cin >> std::ws, typed);
    const std::string typedN = norm(typed);
    if (typedN.empty()) { std::cout << "No name entered.\n"; return; }

    // Try exact (normalized) match first, then unique partial among SELLABLE names
    std::string chosenName;
    for (auto& nm : sellableNames) {
        if (norm(nm) == typedN) { chosenName = nm; break; }
    }
    if (chosenName.empty()) {
        std::vector<std::string> candidates;
        for (auto& nm : sellableNames) {
            if (norm(nm).find(typedN) != std::string::npos) candidates.push_back(nm);
        }
        if (candidates.empty()) {
            std::cout << "No item named like \"" << typed << "\" found in your inventory.\n";
            return;
        }
        if (candidates.size() > 1) {
            std::cout << "Ambiguous name. Did you mean:\n";
            for (auto& c : candidates) std::cout << "  - " << c << "\n";
            std::cout << "Please type the full name exactly as shown.\n";
            return;
        }
        chosenName = candidates.front();
    }

    // Recompute available & resale for the chosen item (safe and up-to-date)
    Item* any = nullptr;
    for (int i = 0; i < 25 && !any; ++i) {
        Item* it = PlayerInventory->PullInventoryIndex(i);
        if (it && it->GetItemWord('N') == chosenName) any = it;
    }
    if (!any) { std::cout << "Item disappeared from inventory.\n"; return; }

    const int resalePer = any->GetItemValue('R');
    if (resalePer <= 0) {
        std::cout << "The shop will not buy this item.\n";
        return;
    }

    int slotsWithName = 0, sumV = 0;
    for (int i = 0; i < 25; ++i) {
        Item* it = PlayerInventory->PullInventoryIndex(i);
        if (!it || it->GetItemWord('N') != chosenName) continue;
        ++slotsWithName;
        sumV += std::max(0, it->GetItemValue('V'));
    }
    int available = (slotsWithName <= 1) ? std::max(1, sumV) : slotsWithName;

    // Prompt: quantity
    std::cout << "Enter quantity to sell (available: " << available << "): ";
    int qty = 0;
    if (!(std::cin >> qty)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid quantity.\n";
        return;
    }
    if (qty <= 0) { std::cout << "Quantity must be positive.\n"; return; }
    if (qty > available) {
        std::cout << "You only have " << available << ". Selling " << available << " instead.\n";
        qty = available;
    }

    // Remove from inventory and pay gold
    if (slotsWithName <= 1 && available > 1) {
        // stackable case (single slot with V > 1)
        PlayerInventory->RemoveItemFromInventory(chosenName, qty);
    }
    else {
        // non-stackable duplicates: remove one slot per unit
        for (int k = 0; k < qty; ++k) {
            PlayerInventory->RemoveItemFromInventory(chosenName, 1);
        }
    }

    const int goldGain = resalePer * qty;
    PlayerInventory->setCurrency(PlayerInventory->getCurrency() + goldGain);
    std::cout << "Sold " << qty << " x " << chosenName << " for " << goldGain << " gold.\n";
}


