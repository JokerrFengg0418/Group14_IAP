#pragma once
#include "Shop.h"
#include "Inventory.h"

class Option
{
private: 

	Inventory* PlayerInventory;

public:
	Inventory* getPlayerInventory() const;

};

