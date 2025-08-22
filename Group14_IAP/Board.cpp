#include <iostream>
#include <iomanip>
#include "Board.h"
#include "Enemy.h"
#include <fstream>

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

void Board::initializeDungeonXGrid() {
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			board[i][j] = 'X';           // fill with X

	board[4][4] = ' '; // spawn cell blank (bottom-right)
}

void Board::setCellContentDungeon(int r, int c, char ch) { board[r][c] = ch; }
char Board::getCellContentDungeon(int r, int c) const { return board[r][c]; }

void Board::drawDungeon() {
	std::cout << "+---------+ \n";
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			char cell = board[i][j];
			std::cout << '|' << (cell ? cell : ' ');
		}
		std::cout << "|\n";
	}
	std::cout << "+---------+ \n";
}

bool Board::save(const char* path) const {
	std::ofstream out(path, std::ios::binary);
	if (!out) return false;
	out.write(reinterpret_cast<const char*>(board), sizeof(board));
	return true;
}

bool Board::load(const char* path) {
	std::ifstream in(path, std::ios::binary);
	if (!in) return false;
	in.read(reinterpret_cast<char*>(board), sizeof(board));
	return in.good();
}


char Board::GetBoard() const
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
