#pragma once
#include "Board.h"
#include "Player.h"

class Dungeon {
public:
    Dungeon(Player* p, Inventory* inv) : player(p), PlayerInventory(inv) {}   // non-owning pointer
    void dungeonOption();

private:
    Board board;               
    Player* player; 
    Inventory* PlayerInventory;
    bool dungeonInited = false;
};
