#include "Dungeon.h"
#include <iostream>
#include <conio.h>

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() (std::cout << "\x1b[2J\x1b[H")
#endif

void Dungeon::dungeonOption() {
    bool running = true;

    // start in bottom-right of 5x5
    player->setRow(4);
    player->setCol(4);

    int prevR = -1, prevC = -1;

    auto render = [&]() {
        CLEAR_SCREEN();
        board.drawDungeon();
        std::cout << "\n=== DUNGEON ===\n"
            << "Move (W/A/S/D) or 'E' to Exit: ";
        };

    while (running) {
        // erase previous P
        if (prevR >= 0 && prevC >= 0)
            board.setCellContentDungeon(prevR, prevC, ' ');

        const int r = player->getRow();
        const int c = player->getCol();
        board.setCellContentDungeon(r, c, 'P');

        prevR = r; prevC = c;
        render();

        if (player->moveDungeon())  // returns true on 'E'
            running = false;
    }
}
