#include "Option.h"
#include <limits>

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
        cout << "=============================\n";
        cout << "Enter your choice: ";

        int choice = 0;
        cin >> choice;
        clearCin(); // clean trailing junk/newlines

        switch (choice) {
        case 1:
            openInventory();
            waitForEnter();
            break;
        case 2:
            shopOption(PlayerInventoryPointer);
            break;
        case 3:
            dungeonOption();
            break;
        case 4:
            cout << "Exiting...\n";
            running = false;
            break;
        default:
            cout << "Invalid choice, try again.\n";
            waitForEnter();
            break;
        }
    }
}

void Option::handleInput() {
    char input;
    std::cout << "Press 'E' to open/close inventory.\n";
    std::cin >> input;

    if (input == 'E' || input == 'e') {
        if (!inventoryOpen) {
            openInventory();
        }
        else {
            closeInventory();
        }
    }
}

void Option::openInventory() {
    inventoryOpen = true;

    constexpr int ROWS = 4;
    constexpr int COLS = 5;

    while (inventoryOpen) {
        std::cout << "\n=== INVENTORY ===\n";
        PlayerInventory.DrawInventory();
        std::cout << "Enter coordinates (row col) to equip item, or 'E' to close: ";

        std::string first;
        if (!(std::cin >> first)) {
            clearCin();
            std::cout << "Invalid input, try again.\n";
            continue;
        }

        if (first == "E" || first == "e") {
            closeInventory();
            break;
        }

        int row;
        try {
            row = std::stoi(first);
        }
        catch (...) {
            std::cout << "Invalid input, try again.\n";
            clearCin();
            continue;
        }

        int col;
        if (!(std::cin >> col)) {
            std::cout << "Invalid input, try again.\n";
            clearCin();
            continue;
        }

        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            std::cout << "Invalid coordinates, try again.\n";
            continue;
        }

        const int index = row * COLS + col;

        Item* item = PlayerInventory.PullInventoryIndex(index);
        if (item) {
            PlayerInventory.setEquippedItem(item);
            std::cout << "Equipped " << item->GetItemWord('N') << "!\n";
        }
        else {
            std::cout << "No item at those coordinates.\n";
        }
    }
}

void Option::closeInventory() {
    inventoryOpen = false;
    std::cout << "Inventory closed.\n";
}

// Shop Menu

void Option::displayMenu() {
    cout << "=============================\n";
    cout << "           MAIN MENU         \n";
    cout << "=============================\n";
    cout << "1. Inventory\n";
    cout << "2. Shop\n";
    cout << "3. Dungeon\n";
    cout << "4. Exit\n";
    cout << "=============================\n";
    cout << "Enter your choice: ";
}

void Option::handleChoice(int choice, Inventory* inventory) {
    switch (choice) {
    case 1:
        // Open/close & interact with inventory
        openInventory();
        break;
    case 2:
        // Enter the shop flow
        shopOption(inventory);
        break;
    case 3:
        // Enter dungeon (stub here; hook up to your dungeon system)
        dungeonOption();
        break;
    case 4:
        cout << "Exiting...\n";
        break;
    default:
        cout << "Invalid choice, try again.\n";
        break;
    }
}

void Option::shopOption(Inventory* inventory) {
    Shop shop(inventory);   // uses the player's DBs

    bool keepShopping = true;
    while (keepShopping) {
        cout << "\n============================================\n";
        cout << "Welcome to the shop!\n";
        cout << "--------------------------------------------\n";
        cout << "Your gold: " << inventory->getCurrency() << "\n\n";

        // Show current stock
        shop.displayItems();

        // New actions, including Refresh
        cout << "\nActions: [B]uy   [R]efresh stock   [E]xit\n";
        cout << "Enter action: ";

        std::string action;
        cin >> action;

        if (action.size() == 1) {
            char a = static_cast<char>(std::tolower(action[0]));
            if (a == 'b') {
                // Shop handles prompting for item index internally
                shop.buyItem(inventory);
                continue;
            }
            else if (a == 'r') {
                // NEW: refresh stock
                shop.refreshStock();      // requires Shop::refreshStock() (see below)
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

// --- NEW: simple dungeon entry stub (add if you don't already have one) ---
void Option::dungeonOption() {
    cout << "\n[Entering the dungeon...]\n";
    // TODO: hook up your dungeon run here
    cout << "[Dungeon WIP]\n";
    waitForEnter();
}
