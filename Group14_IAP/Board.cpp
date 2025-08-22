#include <iostream>
#include <iomanip>
#include "Board.h"
#include "Enemy.h"

// ANSI color (works on Win10+ if VT enabled)
static const char* const YELLOW = "\x1b[33m";
static const char* const RESET = "\x1b[0m";

static constexpr int ROWS = 25;
static constexpr int COLS = 25;

Board::Board()
    : Player(nullptr),
    enemyCount(0),
    selectedEnemy(nullptr)
{
    // Init enemy slots
    for (int i = 0; i < maxEnemies; ++i) {
        enemies[i] = nullptr;
    }
    // Init board cells
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            board[r][c] = ' ';
        }
    }
    std::cout << "Map created\n";
}

Board::~Board()
{
    // Non-owning: do NOT delete Player or enemies here
    // If Board is the owner in your design, re-enable deletes carefully.
}

void Board::selectEnemy(Entity* e) {
    selectedEnemy = e;
}

void Board::addPlayer(Entity* p)
{
    Player = p;
}

void Board::addEnemy(Entity* e)
{
    if (enemyCount < maxEnemies) {
        enemies[enemyCount++] = e;
    }
    else {
        std::cout << "Max enemies reached.\n";
    }
}

static inline bool inBounds(int r, int c) {
    return (r >= 0 && r < ROWS && c >= 0 && c < COLS);
}

void Board::printBoardCellColor(int row, int col)
{
    if (!inBounds(row, col)) return;

    bool isSelected = false;
    for (int k = 0; k < enemyCount; ++k) {
        if (enemies[k] &&
            enemies[k]->getRow() == row &&
            enemies[k]->getCol() == col) {
            isSelected = true;
            break;
        }
    }

    if (isSelected) {
        system("cls"); // Windows-only. Remove if not desired.
        std::cout << "+--------------------------------------------------+\n";
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                std::cout << '|';
                if (i == row && j == col) {
                    std::cout << YELLOW << board[i][j] << RESET;
                }
                else {
                    std::cout << board[i][j];
                }
            }
            std::cout << "|\n";
        }
        std::cout << "+--------------------------------------------------+\n";
    }
    else {
        std::cout << board[row][col];
    }
}

void Board::drawBoard()
{
    // Clear board
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            board[r][c] = ' ';
        }
    }

    // Place Player
    if (Player) {
        int pr = Player->getRow(), pc = Player->getCol();
        if (inBounds(pr, pc)) board[pr][pc] = 'P';
    }

    // Place Enemies
    for (int i = 0; i < enemyCount; ++i) {
        if (!enemies[i]) continue;
        int r = enemies[i]->getRow();
        int c = enemies[i]->getCol();
        if (!inBounds(r, c)) continue;

        // getTypeName() must return a single char marker for this to work.
        board[r][c] = static_cast<Enemy*>(enemies[i])->getTypeName();
    }

    // Render once into a string (less flicker)
    std::string out;
    out.reserve((ROWS + 2) * (COLS * 2 + 4));

    out += "+--------------------------------------------------+\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            out.push_back('|');
            out.push_back(board[i][j]);
        }
        out.push_back('|');
        out.push_back('\n');
    }
    out += "+--------------------------------------------------+\n";

    std::cout << out;
}

// Function to draw a 5x5 Dungeon layout (kept as-is, but bounded)
void Board::drawDungeon()
{
	std::cout << "+---------+ \n";
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			char cell = Board::board[i][j];
			if (cell == ' ' || cell == '\0') {
				std::cout << '|' << 'X'; // gap → X
			}
			else {
				std::cout << '|' << cell; // keep actual content
			}
		}
		std::cout << '|';
		std::cout << std::endl;
	}
	std::cout << "+---------+ \n";
}


void Board::setCellContentDungeon(int row, int col, char content)
{
	if (row >= 0 && row < 5 && col >= 0 && col < 5) {
		board[row][col] = content;
	}
}

// Prefer a safe accessor with indices.
// Update the header to match this signature:
char Board::GetBoard(int row, int col) const
{
    if (!inBounds(row, col)) return ' ';
    return board[row][col];
}

void Board::clearBoard()
{
    // Clear board cells
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            board[r][c] = ' ';
        }
    }

    // Non-owning: just drop references
    Player = nullptr;
    selectedEnemy = nullptr;

    for (int i = 0; i < enemyCount; ++i) {
        enemies[i] = nullptr;
    }
    enemyCount = 0;
}

void Board::removeEnemy(Entity* e)
{
    for (int i = 0; i < enemyCount; ++i) {
        if (enemies[i] == e) {
            // Move last into this slot (if not already last)
            enemies[i] = enemies[--enemyCount];
            enemies[enemyCount] = nullptr;
            return;
        }
    }
    std::cout << "Enemy not found.\n";
}
