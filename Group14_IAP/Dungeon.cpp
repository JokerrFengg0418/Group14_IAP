#include "Dungeon.h"
#include <iostream>
#include <conio.h>
#ifdef _WIN32
#endif

// Persist the dungeon grid for the whole program run
static Board sBoard;
static bool  sInited = false;

// Do NOT re-initialize here; constructor stays empty
Dungeon::Dungeon() {}

void Dungeon::dungeonOption() {
    bool DungeonOpen = true;

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

        std::cout << "\n=== DUNGEON ===\n";
        std::cout << "Move (W/A/S/D) or 'E' to Exit: ";

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
