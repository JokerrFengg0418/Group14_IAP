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

    // Try to load; if no save exists yet, init fresh
    if (!dungeonInited) {
        if (!board.load("dungeon.dat")) {
            board.initializeDungeonXGrid();
        }
        dungeonInited = true;
    }

    // spawn player (bottom-right)
    player.setRow(4);
    player.setCol(4);

    while (DungeonOpen) {
        system("cls");

        // draw with temporary 'P'
        int pr = player.getRow(), pc = player.getCol();
        char under = board.getCellContentDungeon(pr, pc);
        board.setCellContentDungeon(pr, pc, 'P');

        board.drawDungeon();

        const int r = player->getRow();
        const int c = player->getCol();
        board.setCellContentDungeon(r, c, 'P');

        // restore underlying cell before moving
        board.setCellContentDungeon(pr, pc, under);

        if (player.moveDungeon()) {
            // Save board state and leave
            board.save("dungeon.dat");
            break;
        }

        // delete X when stepped on it
        pr = player.getRow(); pc = player.getCol();
        if (board.getCellContentDungeon(pr, pc) == 'X') {
            board.setCellContentDungeon(pr, pc, ' ');
        }
    }

    // Also save when loop ends normally
    board.save("dungeon.dat");
}


