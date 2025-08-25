#include "Dungeon.h"
#include <iostream>
#include <conio.h>
#include <random>
#include <vector>
#include <ctime>

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() (std::cout << "\x1b[2J\x1b[H")
#endif

// ---------- Persist for the whole process (resets when program exits) ----------
static Board sBoard;            // shared 5x5 dungeon grid for this process
static bool  sInited = false;   // one-time init guard

// Ruby placement & state
static int  sRubyRow = -1;
static int  sRubyCol = -1;
static bool sRubyCollected = false;

// RNG
static std::mt19937 sRng(static_cast<unsigned>(std::time(nullptr)));
// -----------------------------------------------------------------------------

void Dungeon::dungeonOption() {
    if (!player) {
        std::cout << "[Dungeon] No player provided.\n";
        return;
    }

    // One-time setup of the 5x5 dungeon grid (per program run)
    if (!sInited) {
        // Fill with breakables
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 5; ++c)
                sBoard.setCellContentDungeon(r, c, 'X');

        // Start cell empty
        sBoard.setCellContentDungeon(4, 4, ' ');
        sRubyCollected = false;

        // Pick ONE random X as the Ruby tile (not the start cell)
        std::vector<std::pair<int, int>> candidates;
        candidates.reserve(25);
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (r == 4 && c == 4) continue; // skip start
                if (sBoard.getCellContentDungeon(r, c) == 'X') {
                    candidates.emplace_back(r, c);
                }
            }
        }
        if (!candidates.empty()) {
            std::uniform_int_distribution<int> dist(0, static_cast<int>(candidates.size()) - 1);
            auto pick = candidates[dist(sRng)];
            sRubyRow = pick.first;
            sRubyCol = pick.second;
        }

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
        sBoard.setCellContentDungeon(pr, pc, under); // restore

        // Move (return true to exit)
        if (player->moveDungeon()) {
            running = false;
            break;
        }

        // After moving, if we stepped on an X, delete it
        pr = player->getRow();
        pc = player->getCol();
        if (sBoard.getCellContentDungeon(pr, pc) == 'X') {
            sBoard.setCellContentDungeon(pr, pc, ' ');
        }

        // Check Ruby pickup (only once)
        if (!sRubyCollected && pr == sRubyRow && pc == sRubyCol) {
            sRubyCollected = true;
            std::cout << "\nCollected red Ruby!\n";
            // (Optional) pause for a key:
            (void)_getch();
        }
    }
}

