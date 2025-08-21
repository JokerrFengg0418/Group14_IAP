#pragma once
#include "Shop.h"
#include "Inventory.h"
#include <iostream>
#include <string>

class Option
{
private:
    Inventory PlayerInventory;
    Inventory* PlayerInventoryPointer; // pointer to the player’s inventory
    bool inventoryOpen;         // track if inventory is open

    void shopOption(Inventory* inventory);  // Opens the shop and keeps looping until the player chooses to exit

    void dungeonOption();       // Stub/entry to your dungeon flow

    // Small helpers
    void waitForEnter() const;
    void clearCin() const;

public:
    Option();     // constructor to set player inventory

    Inventory* getPlayerInventory() const;
    void runMainMenu();         // The single menu loop

    void handleInput();         // check input (E to toggle inventory)
    void openInventory();       // open and interact with inventory
    void closeInventory();      // close inventory

    void displayMenu();         // Prints the main menu 

    void handleChoice(int choice, Inventory* inventory);    // Handles the user’s menu choice
};
