#include "Dungeon.h"
#include <iostream>
#include <conio.h>

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() (std::cout << "\x1b[2J\x1b[H")
#endif

// Persist the dungeon grid for the whole program run
static Board sBoard;
static bool  sInited = false;

// Do NOT re-initialize here; constructor stays empty
Dungeon::Dungeon() {}

void Dungeon::dungeonOption() {
    bool running = true;

    // One-time setup per program run (no disk save/load)
    if (!sInited) {
        sBoard.initializeDungeonXGrid();  // fill with 'X', blank at (4,4)
        sInited = true;
    }

    // spawn player (bottom-right)
    player.setRow(4);
    player.setCol(4);

    while (DungeonOpen) {
        system("cls");

        // draw with temporary 'P'
        int pr = player.getRow(), pc = player.getCol();
        char under = sBoard.getCellContentDungeon(pr, pc);
        sBoard.setCellContentDungeon(pr, pc, 'P');

        sBoard.drawDungeon();

        const int r = player->getRow();
        const int c = player->getCol();
        board.setCellContentDungeon(r, c, 'P');

        // restore underlying cell before moving
        sBoard.setCellContentDungeon(pr, pc, under);

        // handle input/move; E exits dungeon (no saving)
        if (player.moveDungeon()) {
            break;
        }

        // delete X when stepped on it
        pr = player.getRow(); pc = player.getCol();
        if (sBoard.getCellContentDungeon(pr, pc) == 'X') {
            sBoard.setCellContentDungeon(pr, pc, ' ');
        }
    }
}
