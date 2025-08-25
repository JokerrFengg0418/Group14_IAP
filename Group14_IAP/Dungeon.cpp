#include "Dungeon.h"
#include <iostream>
#include <conio.h>

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() (std::cout << "\x1b[2J\x1b[H")
#endif

// ---------- Persist for the whole process (resets when program exits) ----------
static Board sBoard;       // shared 5x5 dungeon grid for this process
static bool  sInited = false; // one-time init guard
// -----------------------------------------------------------------------------

void Dungeon::dungeonOption() {
    if (!player) {
        std::cout << "[Dungeon] No player provided.\n";
        return;
    }

    // One-time setup of the 5x5 dungeon grid (per program run)
    if (!sInited) {
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 5; ++c)
                sBoard.setCellContentDungeon(r, c, 'X'); // fill with breakables
        sBoard.setCellContentDungeon(4, 4, ' ');         // start cell empty
        sInited = true;
    }

    // Spawn player at bottom-right for each entry
    player->setRow(4);
    player->setCol(4);

    bool running = true;
    while (running) {
        CLEAR_SCREEN();

        // --- draw with a temporary 'P' ---
        int pr = player->getRow(), pc = player->getCol();
        char under = sBoard.getCellContentDungeon(pr, pc);
        sBoard.setCellContentDungeon(pr, pc, 'P');
        sBoard.drawDungeon();
        std::cout << "\n=== DUNGEON ===\n";
        std::cout << "Move (W/A/S/D) or 'E' to Exit: ";
        // restore underlying tile after drawing
        sBoard.setCellContentDungeon(pr, pc, under);

        // Move (ensure your moveDungeon() updates (row,col) only — no board reset)
        if (player->moveDungeon()) { // return true to exit
            running = false;
            break;
        }

        // After moving, if we stepped on an X, delete it permanently
        pr = player->getRow();
        pc = player->getCol();
        if (sBoard.getCellContentDungeon(pr, pc) == 'X') {
            sBoard.setCellContentDungeon(pr, pc, ' ');
        }
    }
}
