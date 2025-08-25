#pragma once
#include "Board.h"
#include "Player.h"

class Dungeon {
public:
    explicit Dungeon(Player* p) : player(p) {}   // non-owning pointer
    void dungeonOption();

private:
    Board board;               // non-owning of entities
    Player* player;            // non-owning; must be valid for the session
    bool dungeonInited = false;
};
