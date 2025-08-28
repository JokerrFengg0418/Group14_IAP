#pragma once
#include "Shop.h"
#include "Inventory.h"
#include <iostream>
#include <string>

class Option
{

private:
    Inventory PlayerInventory;
    Inventory* PlayerInventoryPointer; // pointer to the player's inventory
    bool inventoryOpen;         // track if inventory is open
    bool dungeonOpen;

    Player mainPlayer;

    // Small helpers
    void clearCin() const;

public:
    Option();     // constructor to set player inventory

    Inventory* getPlayerInventory() const;
    void runMainMenu();         // The single menu loop

    void openInventory();       // open and interact with inventory
    void closeInventory();      // close inventory
    void shopOption(Inventory* inventory);
    void waitForEnter() const;
    void gameStart() const;

    Player* getPlayerDungeon();
};