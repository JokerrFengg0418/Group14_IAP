#pragma once
#include <vector>
#include "Item.h"
#include "Player.h"
#include "Inventory.h"


class Shop
{
private:
    std::vector<Item*> inventory;
    Inventory* gameInventory;
    void populateShop();

public:
    // Constructor
    Shop(Inventory* inventoryPtr);
    ~Shop();

    // Member functions
    void displayItems();
    void buyItem(Inventory* PlayerInventory);
    void refreshStock();

    // in class Shop
    void sellItem(Inventory* PlayerInventory);
};

