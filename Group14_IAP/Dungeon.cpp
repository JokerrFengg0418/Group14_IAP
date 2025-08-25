#include "Dungeon.h"
#include "Logic.h"
#include "Option.h"
#include "Combat.h"
#include <iostream>
#include <conio.h>
#include <random>
#include <vector>
#include <ctime>
#include <algorithm>

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

static std::vector<std::pair<int, int>> sCombatTiles;
// -----------------------------------------------------------------------------

void Dungeon::dungeonOption() {

    Option GameOption;
    Combat CombatHandler;

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

        // Build candidate list for ruby/combat tiles (skip start)
        std::vector<std::pair<int, int>> candidates;
        candidates.reserve(25);
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (r == 4 && c == 4) continue; // skip start
                if (sBoard.getCellContentDungeon(r, c) == 'X')
                    candidates.emplace_back(r, c);
            }
        }

        // Pick ONE ruby tile
        if (!candidates.empty()) {
            std::uniform_int_distribution<int> dist(0, static_cast<int>(candidates.size()) - 1);
            auto pick = candidates[dist(sRng)];
            sRubyRow = pick.first;
            sRubyCol = pick.second;
            // Ensure ruby tile is not also a combat tile
            candidates.erase(std::remove(candidates.begin(), candidates.end(), pick), candidates.end());
        }

        // Pick up to 12 distinct combat tiles from remaining X’s
        std::shuffle(candidates.begin(), candidates.end(), sRng);
        const int WANT = 12;
        const int take = std::min(WANT, static_cast<int>(candidates.size()));
        sCombatTiles.assign(candidates.begin(), candidates.begin() + take);

        sInited = true;
    }

    // Spawn player at bottom-right for each entry
    player->setRow(4);
    player->setCol(4);

    // helper to render one frame (optionally with a status message)
    auto renderFrame = [&](const char* msg = nullptr) {
        CLEAR_SCREEN();

        int pr = player->getRow(), pc = player->getCol();
        char under = sBoard.getCellContentDungeon(pr, pc);
        sBoard.setCellContentDungeon(pr, pc, 'P');

        sBoard.drawDungeon();

        std::cout << "\n=== DUNGEON ===\n";
        if (msg) std::cout << msg << '\n';
        std::cout << "Move (W/A/S/D) or 'E' to Exit: ";

        // restore underlying tile after drawing
        sBoard.setCellContentDungeon(pr, pc, under);
    };

    bool running = true;
    while (running) {
        // draw current state first
        renderFrame();

        // Move (return true to exit)
        if (player->moveDungeon()) {
            break;
        }

        // After moving, update the tile the player stepped on
        int pr = player->getRow();
        int pc = player->getCol();

        // Break X tiles when stepped on
        if (sBoard.getCellContentDungeon(pr, pc) == 'X') {
            sBoard.setCellContentDungeon(pr, pc, ' ');
        }

        // Ruby pickup (only once) — pause so it stays visible
        if (!sRubyCollected && pr == sRubyRow && pc == sRubyCol) {
            sRubyCollected = true;

            CLEAR_SCREEN();
            char under2 = sBoard.getCellContentDungeon(pr, pc);
            sBoard.setCellContentDungeon(pr, pc, 'P');
            sBoard.drawDungeon();

            std::cout << "\n=== DUNGEON ===\nCollected red Ruby!\nPress any key to continue...";
            sBoard.setCellContentDungeon(pr, pc, under2);

            (void)_getch();
        }

        // Combat trigger tiles — show "Attack Combat" once per tile
        auto it = std::find(sCombatTiles.begin(), sCombatTiles.end(), std::make_pair(pr, pc));
        if (it != sCombatTiles.end()) {
            // consume so it won't re-trigger
            sCombatTiles.erase(it);

            CLEAR_SCREEN();
            char under3 = sBoard.getCellContentDungeon(pr, pc);
            sBoard.setCellContentDungeon(pr, pc, 'P');
            sBoard.drawDungeon();

            std::cout << "\n=== DUNGEON ===\nAttack Combat\nPress any key to continue...";
            sBoard.setCellContentDungeon(pr, pc, under3);

            (void)_getch();
            CombatHandler.startCombat('T');
            CombatHandler.TurnOrder(GameOption.getPlayerInventory());

        }
    }
}


