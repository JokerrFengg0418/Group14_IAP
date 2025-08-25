#include "Inventory.h"
#include "Item.h"
#include <iostream>
#include <string>
#include <cctype>

// ---- small helpers ---------------------------------------------------------

static inline std::string toLowerCopy(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

static inline int firstEmptySlot(Item* inv[25]) {
    for (int i = 0; i < 25; ++i) if (inv[i] == nullptr) return i;
    return -1;
}

static inline int findSlotByExactName(Item* inv[25], const std::string& q) {
    for (int i = 0; i < 25; ++i) {
        if (inv[i] && inv[i]->GetItemWord('N') == q) return i;
    }
    return -1;
}

static inline int findSlotByNameContains(Item* inv[25], const std::string& q) {
    for (int i = 0; i < 25; ++i) {
        if (inv[i] && inv[i]->GetItemWord('N').find(q) != std::string::npos) return i;
    }
    return -1;
}

// ---- Inventory implementation ---------------------------------------------

Inventory::Inventory()
    : Currency(0),
    EquippedItem(nullptr),          // legacy pointer (kept for compatibility)
    EquippedWeapon(nullptr),
    EquippedArmor(nullptr)
{
    for (int i = 0; i < 50; ++i) { ItemDatabase[i] = nullptr; MonsterItemDatabase[i] = nullptr; ArmorDatabase[i] = nullptr; }
    for (int i = 0; i < 10; ++i) { WeaponDatabase[i] = nullptr; }
    for (int i = 0; i < 25; ++i) { inventory[i] = nullptr; }

    // Build DB after arrays are null-inited
    DatabaseInitialisation();
}

Inventory::~Inventory()
{
}

//Drawing Inventory (UI Save Me)
void Inventory::DrawInventory() const {
    std::cout << "+------------------------------------------------------------------------------------+\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            int index = i * 5 + j;
            if (inventory[index] != nullptr) {
                std::cout << '|' << inventory[index]->GetItemWord('N');
            }
            else {
                std::cout << '|' << "                ";
            }
        }
        std::cout << '|' << std::endl;
    }
    std::cout << "+------------------------------------------------------------------------------------+\n";
}

//Draws Specifications of Inventory Item
Item* Inventory::getInventory(std::string Name) const {
    for (int i = 0; i < 25; ++i) {
        Item* it = inventory[i];
        if (it && it->GetItemWord('N') == Name) return it;
    }
    return nullptr;
}

//Sets Item into Inventory (adds 1 for stackables, places weapons/armors into empty slots)
void Inventory::setInventory(std::string ItemName, int Number) {
    if (Number <= 0) return;

    // classify item using DB membership
    const bool isWeapon = (DrawDatabase('W', ItemName) != nullptr);
    const bool isArmor = (DrawDatabase('A', ItemName) != nullptr);
    const bool isStackable = (!isWeapon && !isArmor); // only consumables stack

    // If already present:
    for (int i = 0; i < 25; ++i) {
        Item* it = inventory[i];
        if (!it) continue;
        if (it->GetItemWord('N') == ItemName) {
            if (isStackable) {
                int cur = it->GetItemValue('V');
                it->SetItemValue('V', cur + Number);
                return;
            }
            else {
                // weapon/armor: non-stackable → add new slots below
                break;
            }
        }
    }

    // Find the source pointer in DBs
    Item* src = DrawDatabase('W', ItemName);
    if (!src) src = DrawDatabase('A', ItemName);  // <--- NEW armor DB
    if (!src) src = DrawDatabase('I', ItemName);
    if (!src) src = DrawDatabase('M', ItemName);
    if (!src) {
        std::cerr << "[Inventory] Unknown item: " << ItemName << "\n";
        return;
    }

    // For non-stackables, add one slot per piece; for stackables, just place 1 slot
    int copies = isStackable ? 1 : Number;
    for (int k = 0; k < copies; ++k) {
        int empty = -1;
        for (int i = 0; i < 25; ++i) { if (inventory[i] == nullptr) { empty = i; break; } }
        if (empty < 0) { std::cout << "Inventory is full.\n"; return; }
        inventory[empty] = src;   // DB owns pointer; slot is just a reference/type token
    }
}

// Removes Number from inventory for stackables; removes slot for weapons/armors
void Inventory::RemoveItemFromInventory(std::string ItemName, int Number) {
    if (Number <= 0) return;

    const bool isWeapon = (DrawDatabase('W', ItemName) != nullptr);
    const bool isArmor = (DrawDatabase('A', ItemName) != nullptr);
    const bool isStackable = (!isWeapon && !isArmor); // consumables only

    for (int i = 0; i < 25; ++i) {
        Item* it = inventory[i];
        if (!it) continue;
        if (it->GetItemWord('N') != ItemName) continue;

        if (!isStackable) {
            // weapon or armor → remove one slot
            inventory[i] = nullptr;   // DO NOT delete; DB owns it
            return;
        }

        // stackable (consumables): subtract quantity
        int cur = it->GetItemValue('V') - Number;
        if (cur <= 0) inventory[i] = nullptr;
        else          it->SetItemValue('V', cur);
        return;
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

    case 'A': // <--- NEW: Armor DB
        for (int i = 0; i < 50; i++) {
            if (ArmorDatabase[i] == nullptr) {
                ArmorDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);
                return;
            }
        }
        break;

    case 'I': // consumables/other items
        for (int i = 0; i < 50; i++) {
            if (ItemDatabase[i] == nullptr) {
                ItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);
                return;
            }
        }
        break;

    case 'M': // monster drops
        for (int i = 0; i < 50; i++) {
            if (MonsterItemDatabase[i] == nullptr) {
                MonsterItemDatabase[i] = new Item(ItemName, ItemDescription, Type, Value, ResaleValue, SaleValue, number);
                return;
            }
        }
        break;
    }
}

Item* Inventory::FindItemByName(const std::string& itemName) {
    for (int i = 0; i < 25; ++i) {
        Item* it = inventory[i];
        if (it) {
            // DB names may have spaces; partial match helps
            if (it->GetItemWord('N').find(itemName) != std::string::npos) {
                return it;
            }
        }
    }
    return nullptr;
}

void Inventory::DatabaseInitialisation() {


    //ItemName, Description, Character type,Integer Value (Number of items) if its an armour or a weapon, its dmg value/hp value, Resale Value, Sale Value, 

    // ===== Weapons (DB 'W') =====
    FactoryCreateItem("   Broadsword   ", "A basic but reliable blade.", 'W', 50, 20, 30, 10, 'W');
    FactoryCreateItem("   SlingShots   ", "A simple ranged weapon.", 'W', 50, 20, 30, 10, 'W');
    FactoryCreateItem(" Bow and Arrows ", "Classic ranged weapon.", 'W', 80, 30, 50, 10, 'W');
    FactoryCreateItem("      Mace      ", "A heavy blunt weapon.", 'W', 100, 40, 60, 5, 'W');
    FactoryCreateItem("   Battle Axe   ", "For chopping and combat.", 'W', 250, 100, 150, 5, 'W');
    FactoryCreateItem("    Crossbow    ", "Advanced ranged weapon.", 'W', 150, 60, 90, 5, 'W');
    FactoryCreateItem("     Turret     ", "Stationary defense weapon.", 'W', 5, 120, 180, 2, 'W');

    // ===== Armors (DB 'A') =====
    FactoryCreateItem("  Wooden Armor  ", "Basic protective armor.", 'A', 50, 20, 30, 10, 'A');
    FactoryCreateItem("  Silver Armor  ", "Stronger defense.", 'A', 100, 40, 60, 5, 'A');
    FactoryCreateItem("   Gold Armor   ", "Shiny and strong.", 'A', 150, 60, 90, 30, 'A');
    FactoryCreateItem(" Leather Armors ", "Lightweight armor.", 'A', 10, 4, 6, 20, 'A');
    FactoryCreateItem("     Shield     ", "Protects against attacks.", 'A', 100, 40, 60, 5, 'A');
    FactoryCreateItem("     Helmet     ", "Protects your head.", 'A', 80, 30, 50, 5, 'A');

    // ===== Consumables / Other (DB 'I') =====
    FactoryCreateItem(" Health Potions ", "Restores health.", 'I', 1, 20, 30, 20, 'I');

    // ===== Monster/Dungeon Drop Items / Other (DB 'M') =====
    FactoryCreateItem("    Rat Tail    ", "A tail from a rat (Sellable).", 'M', 1, 3, 0, 0, 'M');
    FactoryCreateItem("      Fang      ", "A fang from a hellhound (Sellable/Quest Item).", 'M', 1, 20, 0, 0, 'M');
    FactoryCreateItem("  Rotten Flesh  ", "A rotten flesh from a zombie (Sellable).", 'M', 1, 5, 0, 0, 'M');
    FactoryCreateItem("   Mana Cores   ", "A mana core from a goblin (Sellable/Quest Item).", 'M', 1, 15, 0, 0, 'M');
    FactoryCreateItem("    Bat Wing    ", "A wing from a bat (Sellable).", 'M', 1, 7, 0, 0, 'M');
    FactoryCreateItem("      Bone      ", "A bone from a skeleton (Sellable).", 'M', 1, 8, 0, 0, 'M');
    FactoryCreateItem("  Flying Broom  ", "A flying broom from a witch (Sellable).", 'M', 1, 50, 0, 0, 'M');
    FactoryCreateItem("Crystal of Power", "A flying broom from a witch (Sellable).", 'M', 1, 50, 0, 0, 'M');
    FactoryCreateItem("    Red Ruby    ", "A flying broom from a witch (Sellable).", 'M', 1, 50, 0, 0, 'M');
}

//Draws Item from Database instead of Inventory//
Item* Inventory::DrawDatabase(char DatabaseType, std::string ItemName) {
    switch (DatabaseType) {
    case 'W':
        for (int i = 0; i < 10; ++i) {
            Item* it = WeaponDatabase[i];
            if (it && it->GetItemWord('N') == ItemName) return it;
        }
        break;

    case 'A': // NEW Armor database
        for (int i = 0; i < 50; ++i) {
            Item* it = ArmorDatabase[i];
            if (it && it->GetItemWord('N') == ItemName) return it;
        }
        break;

    case 'I': // Items (consumables/others)
        for (int i = 0; i < 50; ++i) {
            Item* it = ItemDatabase[i];
            if (it && it->GetItemWord('N') == ItemName) return it;
        }
        break;

    case 'M': // Monster drops
        for (int i = 0; i < 50; ++i) {
            Item* it = MonsterItemDatabase[i];
            if (it && it->GetItemWord('N') == ItemName) return it;
        }
        break;
    }
    return nullptr;
}

Item* Inventory::PullInventoryIndex(int Index) const {
    if (Index < 0 || Index >= 25) return nullptr;
    return inventory[Index];
}

// --- Currency ---------------------------------------------------------------
int Inventory::getCurrency() const { return Currency; }
void Inventory::setCurrency(int NewAmount) { Currency = NewAmount; }

// --- Equipment getters ------------------------------------------------------
Item* Inventory::getEquippedWeapon() const { return EquippedWeapon; }
Item* Inventory::getEquippedArmor()  const { return EquippedArmor; }

// --- Backward-compat getter (returns weapon) --------------------------------
Item* Inventory::getEquippedItem() const { return EquippedWeapon; }

// --- Equip by NAME (high-level helpers) -------------------------------------
// Route: weapon first, then armor
bool Inventory::equipByName(const std::string& itemName) {
    if (equipWeaponByName(itemName)) return true;
    return equipArmorByName(itemName);
}

// Equip weapon: removes the slot (weapons are non-stackable)
bool Inventory::equipWeaponByName(const std::string& name) {
    int slot = findSlotByNameContains(inventory, name);
    if (slot < 0) { std::cout << "Item not found: " << name << "\n"; return false; }

    Item* found = inventory[slot];
    // Validate weapon by DB membership
    if (!DrawDatabase('W', found->GetItemWord('N'))) {
        std::cout << "This is not a weapon.\n"; return false;
    }

    // Return currently equipped weapon (if any) to first empty slot
    if (EquippedWeapon && EquippedWeapon != found) {
        int freeIdx = firstEmptySlot(inventory);
        if (freeIdx < 0) { std::cout << "Inventory full—cannot swap weapon.\n"; return false; }
        inventory[freeIdx] = EquippedWeapon;
    }

    // Equip and remove from bag
    EquippedWeapon = found;
    EquippedItem = EquippedWeapon; // keep legacy pointer in sync
    inventory[slot] = nullptr;

    std::cout << "Equipped weapon: " << found->GetItemWord('N') << "\n";
    return true;
}

// Equip armor: non-stackable, remove the slot
bool Inventory::equipArmorByName(const std::string& name) {
    int slot = findSlotByNameContains(inventory, name);
    if (slot < 0) { std::cout << "Item not found: " << name << "\n"; return false; }

    Item* found = inventory[slot];

    // Validate armor via DB membership
    if (!DrawDatabase('A', found->GetItemWord('N'))) {
        std::cout << "This is not an armor piece.\n"; return false;
    }

    // If another armor is equipped, put it back to first empty slot (no stacking)
    if (EquippedArmor && EquippedArmor != found) {
        int freeIdx = firstEmptySlot(inventory);
        if (freeIdx < 0) {
            std::cout << "Inventory full—cannot swap armor.\n";
            return false;
        }
        inventory[freeIdx] = EquippedArmor;
    }

    // Equip and remove the inventory slot (non-stackable behavior)
    EquippedArmor = found;
    inventory[slot] = nullptr;

    std::cout << "Equipped armor: " << EquippedArmor->GetItemWord('N') << "\n";
    return true;
}

// --- Equip by POINTER (compat): route to weapon or armor by DB --------------
void Inventory::setEquippedItem(Item* SelectedItem) {
    if (!SelectedItem) { std::cout << "Invalid item selected.\n"; return; }
    const std::string nm = SelectedItem->GetItemWord('N');

    if (DrawDatabase('W', nm)) { equipWeaponByName(nm); return; }
    if (DrawDatabase('A', nm)) { equipArmorByName(nm);  return; }

    std::cout << "You can't equip this item!\n";
}

// --- Unequip (compat = weapon) ----------------------------------------------
void Inventory::unequip() { unequipWeapon(); }

// --- Unequip weapon: moves the pointer back to the first empty slot ----------
void Inventory::unequipWeapon() {
    if (!EquippedWeapon) { std::cout << "No weapon equipped.\n"; return; }
    int freeIdx = firstEmptySlot(inventory);
    if (freeIdx < 0) { std::cout << "Inventory full—cannot unequip weapon.\n"; return; }
    std::cout << "Unequipped weapon: " << EquippedWeapon->GetItemWord('N') << "\n";
    inventory[freeIdx] = EquippedWeapon;
    EquippedWeapon = nullptr;
    EquippedItem = nullptr; // keep legacy pointer in sync
}

// --- Unequip armor: non-stackable, just return to the first empty slot -------
void Inventory::unequipArmor() {
    if (!EquippedArmor) { std::cout << "No armor equipped.\n"; return; }
    int freeIdx = firstEmptySlot(inventory);
    if (freeIdx < 0) { std::cout << "Inventory full—cannot unequip armor.\n"; return; }

    std::cout << "Unequipped armor: " << EquippedArmor->GetItemWord('N') << "\n";
    inventory[freeIdx] = EquippedArmor;
    EquippedArmor = nullptr;
}
