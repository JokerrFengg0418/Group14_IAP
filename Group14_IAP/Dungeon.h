#include "Board.h"
#include "Player.h"
#include "Entity.h"
#include <iostream>

class Dungeon {

public:
    Dungeon();
    void dungeonOption();
private:
    Board board; // The Dungeon class owns the Board object
    Player player;
    bool dungeonInited = false;
};