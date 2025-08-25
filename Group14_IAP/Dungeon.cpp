#include "Dungeon.h"
#include <iostream>
#include <conio.h>

#ifdef _WIN32
#endif

// Persist the dungeon grid for the whole process (resets only on app restart)
static Board sBoard;
static bool  sInited = false;

// Do NOT reinitialize anything here
Dungeon::Dungeon() {
    // leave empty; no board init here
}

void Dungeon::dungeonOption() {
    bool DungeonOpen = true;

    // One-time setup per program run
    if (!sInited) {
        sBoard.initializeDungeonXGrid();  // fill with 'X', blank at (4,4)
        sInited = true;
    }

    // spawn player (bottom-right)
    player.setRow(4);
    player.setCol(4);

    while (DungeonOpen) {
        system("cls");

        // --- draw with temporary 'P' ---
        int pr = player.getRow(), pc = player.getCol();
        char under = sBoard.getCellContentDungeon(pr, pc); // what's under player
        sBoard.setCellContentDungeon(pr, pc, 'P');
        sBoard.drawDungeon();
        std::cout << "\n=== DUNGEON ===\n";
        std::cout << "Move (W/A/S/D) or 'E' to Exit: ";
        sBoard.setCellContentDungeon(pr, pc, under);       // restore after draw

        // input & move (E exits)
        if (player.moveDungeon()) break;

        // delete X when stepped on it
        pr = player.getRow(); pc = player.getCol();
        if (sBoard.getCellContentDungeon(pr, pc) == 'X') {
            sBoard.setCellContentDungeon(pr, pc, ' ');
        }
    }
}
