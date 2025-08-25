#include "Dungeon.h"
#include <iostream>
#include <conio.h>

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() (std::cout << "\x1b[2J\x1b[H")
#endif

void Dungeon::dungeonOption() {
    if (!player) {
        std::cout << "[Dungeon] No player provided.\n";
        return;
    }

    // One-time setup of the 5x5 dungeon grid
    if (!dungeonInited) {
        // Fill 5x5 with 'X' (breakable tiles), clear start cell (4,4)
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 5; ++c)
                board.setCellContentDungeon(r, c, 'X');
        board.setCellContentDungeon(4, 4, ' ');
        dungeonInited = true;
    }

    // Spawn player at bottom-right
    player->setRow(4);
    player->setCol(4);

    bool running = true;
    while (running) {
        CLEAR_SCREEN();

        // Draw with a temporary 'P' on top of whatever is in that cell
        int pr = player->getRow(), pc = player->getCol();
        char under = board.getCellContentDungeon(pr, pc);
        board.setCellContentDungeon(pr, pc, 'P');

        board.drawDungeon();

        std::cout << "\n=== DUNGEON ===\n";
        std::cout << "Move (W/A/S/D) or 'E' to Exit: ";

        // Restore the underlying tile after drawing
        board.setCellContentDungeon(pr, pc, under);

        // Read input and move (player->moveDungeon() should return true to exit)
        if (player->moveDungeon()) {
            running = false;
            break;
        }

        // After moving, if we stepped on an X, delete it
        pr = player->getRow();
        pc = player->getCol();
        if (board.getCellContentDungeon(pr, pc) == 'X') {
            board.setCellContentDungeon(pr, pc, ' ');
        }
    }
}
