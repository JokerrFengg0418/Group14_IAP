#pragma once
#include "Player.h"
#include "Entity.h"

class Board
{
public:
    Board();
    ~Board(); // non-owning: does not delete entities

    void selectEnemy(Entity* e);

    // Safe cell accessor
    char GetBoard(int row, int col) const;

    // Main board (25x25)
    void drawBoard();

    // Dungeon (5x5) helpers + render
    void initializeDungeonXGrid();                 // clears 5x5 area
    void drawDungeon();
    void setCellContentDungeon(int row, int col, char content);
    char getCellContentDungeon(int row, int col) const;

    // No-op stubs unless you already implement elsewhere
    bool save(const char* path) const;
    bool load(const char* path);

    // Entity registration (non-owning)
    void addPlayer(Entity* p);
    void addEnemy(Entity* e);
    void removeEnemy(Entity* e);

    // Visual helper to highlight a cell
    void printBoardCellColor(int row, int col);

    // Clears only the board tiles and references (non-owning)
    void clearBoard();

private:
    static constexpr int ROWS = 25;
    static constexpr int COLS = 25;
    static constexpr int maxEnemies = 20;

    static bool inBounds(int r, int c) {
        return r >= 0 && r < ROWS && c >= 0 && c < COLS;
    }

    char    board[ROWS][COLS]{};
    Entity* Player;                 // non-owning
    Entity* selectedEnemy;          // non-owning
    Entity* enemies[maxEnemies];    // non-owning
    int     enemyCount;
};
