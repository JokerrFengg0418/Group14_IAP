
#include "Dungeon.h"
#include <iostream>
#include <conio.h>
#ifdef _WIN32
#endif

void Dungeon::dungeonOption() {
    bool DungeonOpen = true;

    // Set the player's starting position to the bottom-right corner (row 4, col 4)
    player.setRow(4);
    player.setCol(4);

    while (DungeonOpen) {

        system("cls");

        board.clearBoard();

        board.setCellContentDungeon(player.getRow(), player.getCol(), 'P');

        this->board.drawDungeon();

        std::cout << "\n=== DUNGEON ===\n";
        std::cout << "Move (W/A/S/D) or 'E' to Exit: ";

        if (player.moveDungeon()) { // if true → exit
            DungeonOpen = false;
        }

    }
}