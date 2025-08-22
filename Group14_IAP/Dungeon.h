#include "Board.h"
#include "Player.h"
#include "Entity.h"
#include <iostream>

class Dungeon {

public:
    explicit Dungeon(Player* p) : player(p) {}
    void dungeonOption();

private:
    Board board;        // board should also be non-owning (no deletes)
    Player* player;     // non-owning pointer
};