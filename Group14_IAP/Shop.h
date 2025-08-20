#pragma once
#include <vector>
#include "Item.h"
#include "Player.h"
#include "Inventory.h"

class Shop
{
private:
    std::vector<Item*> inventory;

public:
    // Constructor
    Shop();
    ~Shop();

    // Member functions
    void populateShop();
    void displayItems();
    void buyItem(Player* player);
};

