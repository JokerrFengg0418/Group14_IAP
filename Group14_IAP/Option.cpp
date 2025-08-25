#include "Option.h"
#include "Inventory.h"
#include "Item.h"
#include "Board.h"
#include <iostream>
#include <string>
#include <limits>
#include "Dungeon.h"

using std::cin;
using std::cout;
using std::endl;

// Inventory

Option::Option(){

    inventoryOpen = false;
    PlayerInventoryPointer = &PlayerInventory;
}

Inventory* Option::getPlayerInventory() const {
    return PlayerInventoryPointer;
}

void Option::runMainMenu() {
    bool running = true;
    while (running) {
        // Simple clear (optional)
        // system("cls"); // Windows

        cout << "=============================\n";
        cout << "           MAIN MENU         \n";
        cout << "=============================\n";
        cout << "1. Inventory\n";
        cout << "2. Shop\n";
        cout << "3. Dungeon\n";
        cout << "4. Exit\n";
        cout << "5. Debug (add 50,000 gold + Broadsword)\n";
        cout << "=============================\n";
        cout << "Enter your choice: ";

        int choice = 0;
        cin >> choice;
        clearCin(); // clean trailing junk/newlines

        switch (choice) {
        case 1: {
            openInventory();
            waitForEnter();
            break;
        }
        case 2: {
            shopOption(PlayerInventoryPointer);
            break;
        }
        case 3: {
            Dungeon dungeon(&mainPlayer);   // pass a pointer to your player
            dungeon.dungeonOption();
            waitForEnter();
            break;
        }
        case 4: {
            cout << "Exiting...\n";
            running = false;
            break;
        }
        case 5: // DEBUG GIVE
            if (PlayerInventoryPointer) {
                PlayerInventoryPointer->setCurrency(
                    PlayerInventoryPointer->getCurrency() + 50000);
                PlayerInventoryPointer->setInventory("  Broadsword  ", 1);
                cout << "[Debug] Added 50,000 gold and a Broadsword to your inventory.\n";
            }
            else {
                cout << "[Debug] Player inventory pointer is null.\n";
            }
            waitForEnter();
            break;
        default:
            cout << "Invalid choice, try again.\n";
            waitForEnter();
            break;
        }
    }
}

void Option::openInventory() {
    inventoryOpen = true;

    while (inventoryOpen) {
        std::cout << "\n================================ INVENTORY ================================= \n";
        PlayerInventory.DrawInventory();
        std::cout << "Enter the name of the item to equip, or 'E' to close: ";

        std::string itemName;
        std::getline(std::cin >> std::ws, itemName);

        if (itemName.empty()) {
            std::cout << "Invalid input, try again.\n";
            continue;
        }

        if (itemName == "E" || itemName == "e") {
            closeInventory();
            break;
        }

        Item* item = PlayerInventory.FindItemByName(itemName);
        if (item) {
            PlayerInventory.setEquippedItem(item);
            std::cout << "Equipped " << item->GetItemWord('N') << "!\n";
        }
        else {
            std::cout << "No item with that name found in your inventory.\n";
        }
    }
}

void Option::closeInventory() {
    inventoryOpen = false;
    std::cout << "Inventory closed.\n";
}

// Shop Menu

void Option::shopOption(Inventory* inventory) {
    if (!inventory) {
        cout << "[Shop] Player inventory is null.\n";
        return;
    }

    Shop shop(inventory);   // uses the player's DBs

    int refreshesLeft = 3;  // ← limit per visit

    bool keepShopping = true;
    while (keepShopping) {
        cout << "\n============================================\n";
        cout << "Welcome to the shop!\n";
        cout << "--------------------------------------------\n";
        cout << "Your gold: " << inventory->getCurrency() << "\n\n";

        // Show current stock
        shop.displayItems();

        // New actions, including Refresh and Sell
        cout << "\nActions: [B]uy   [S]ell   [R]efresh stock   [E]xit\n";
        cout << "Enter action: ";

        std::string action;
        cin >> action;

        if (action.size() == 1) {
            char a = static_cast<char>(std::tolower(action[0]));
            if (a == 'b') {
                shop.buyItem(inventory);
                continue;
            }
            else if (a == 's') {
                shop.sellItem(inventory);     // <-- NEW
                continue;
            }
            else if (a == 'r') {
                shop.refreshStock();
                cout << "Shop stock refreshed!\n";
                continue;
            }
            else if (a == 'e') {
                keepShopping = false;
                break;
            }
        }

        cout << "Invalid action. Try again.\n";
    }

    cout << "Leaving the shop...\n";
    waitForEnter();
}

void Option::waitForEnter() const {
    cout << "\nPress ENTER to continue...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (cin.peek() == '\n') cin.get();
}

void Option::clearCin() const {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
