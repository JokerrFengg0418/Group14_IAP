#include "Option.h"
#include <limits>

using std::cin;
using std::cout;
using std::endl;

// Inventory

Option::Option(Inventory* inv) : PlayerInventory(inv), inventoryOpen(false) {}

Inventory* Option::getPlayerInventory() const {
    return PlayerInventory;
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
    bool running = true;

    while (running) {
        std::cout << "\n=== INVENTORY ===\n";
        PlayerInventory->DrawInventory();
        std::cout << "Enter coordinates (row col) to equip item, or 'E' to close: ";

        std::string input;
        std::cin >> input;

        if (input == "E" || input == "e") {
            running = false;
            closeInventory();
            break;
        }

        int row, col;
        try {
            row = std::stoi(input);
            std::cin >> col;
        }
        catch (...) {
            std::cout << "Invalid input, try again.\n";
            continue;
        }

        if (row < 0 || row >= 4 || col < 0 || col >= 5) {
            std::cout << "Invalid coordinates, try again.\n";
            continue;
        }

        int index = row * 5 + col; // Flatten 2D → 1D

        Item* item = (*PlayerInventory).Inventory[index];
        if (item != nullptr) {
            PlayerInventory->setEquippedItem(item);
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
    cout << "1. Enter Shop\n";
    cout << "2. Exit\n";
    cout << "=============================\n";
    cout << "Enter your choice: ";
}

void Option::handleChoice(int choice, Inventory& inventory) {
    switch (choice) {
    case 1:
        shopOption(inventory);
        break;
    case 2:
        cout << "Exiting...\n";
        break;
    default:
        cout << "Invalid choice, try again.\n";
        break;
    }
}

void Option::shopOption(Inventory& inventory) {
    // Create a Shop that uses the player's inventory and databases
    Shop shop(&inventory);

    bool keepShopping = true;
    while (keepShopping) {
        // Clear screen (optional; comment out if you don't want it)
        // system("cls"); // Windows
        cout << "\n============================================\n";
        cout << "Welcome to the shop! What would you like to buy?\n";
        cout << "--------------------------------------------\n";

        // Show player's current gold before the list
        cout << "Your gold: " << inventory.getCurrency() << "\n\n";

        // Show items currently available in the shop
        shop.displayItems();
        cout << "Tip: Enter the item number shown above, or 0 to exit.\n";

        // Let the Shop handle the buy flow (it prompts for the item index itself)
        shop.buyItem(&inventory);

        // After buying (or cancel), ask if they want to continue
        cout << "\nContinue shopping? (Y/N): ";
        char yn;
        cin >> yn;

        if (yn == 'n' || yn == 'N' || yn == '0') {
            keepShopping = false;
        }
        else if (yn == 'y' || yn == 'Y') {
            keepShopping = true;
        }
        else {
            // If input is weird, default to leaving the shop
            keepShopping = false;
        }
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