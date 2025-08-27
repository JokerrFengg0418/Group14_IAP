#include <iostream>
#include "Board.h"
#include "Enemy.h"

// ANSI colors
static const char* const YELLOW = "\x1b[33m";
static const char* const RESET = "\x1b[0m";

Board::Board()
    : Player(nullptr), selectedEnemy(nullptr), enemyCount(0)
{
    static bool printed = false;
    if (!printed) {
        // Map Creation
        printed = true;
    }
    for (int i = 0; i < maxEnemies; ++i) enemies[i] = nullptr;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            board[r][c] = ' ';
}

Board::~Board()
{
    // NON-OWNING: do not delete Player or enemies here
}

void Board::selectEnemy(Entity* e) {
    selectedEnemy = e;
}

void Board::addTurret(Entity* t)
{
    this->Turret = Turret;
}

void Board::addPlayer(Entity* p)
{
    Player = p; // non-owning
}

void Board::addEnemy(Entity* e)
{
    if (enemyCount < maxEnemies) {
        enemies[enemyCount++] = e; // non-owning
    }
    else {
        std::cout << "Max enemies reached.\n";
    }
}

void Board::printBoardCellColor(int row, int col)
{
    if (!inBounds(row, col)) return;

    bool isSelected = false;
    for (int k = 0; k < enemyCount; ++k) {
        if (!enemies[k]) continue;
        if (enemies[k]->getRow() == row && enemies[k]->getCol() == col) {
            isSelected = true;
            break;
        }
    }

#ifdef _WIN32
    system("cls");
#else
    std::cout << "\x1b[2J\x1b[H";
#endif

    std::cout << "+--------------------------------------------------+\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << '|';
            if (isSelected && i == row && j == col)
                std::cout << YELLOW << board[i][j] << RESET;
            else
                std::cout << board[i][j];
        }
        std::cout << "|\n";
    }
    std::cout << "+--------------------------------------------------+\n";
}


void Board::selectTurretHighlight(int row, int col)
{
    system("cls");

    board[row][col] = 'T';
    std::cout << "+------------------------------------------------------+ \n";

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            std::cout << "|";
            if (i == row && j == col) {
                std::cout << YELLOW << board[row][col] << RESET;
            }
            else {
                std::cout << board[i][j];
            }
        }
        std::cout << '|';
        std::cout << '\n';
    }
    std::cout << "+------------------------------------------------------+ \n";
    board[row][col] = ' ';
}



void Board::drawBoard(Entity* List[], int size)
{
    // Clear board tiles
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            board[r][c] = ' ';

    // Place entities on the board using their coordinates
    if (Player) {
        board[Player->getRow()][Player->getCol()] = 'P'; // P = Player
    }

    for (int i = 0; i < 20; i++) {

        if (List[i] != nullptr && List[i]->getEntityType() == 'T') {
            Turret = List[i];
        }
    }

    if (Turret) {
        board[Turret->getRow()][Turret->getCol()] = 'T'; // T = Turret
    }

    // Place enemies
    for (int i = 0; i < enemyCount; ++i) {
        Entity* e = enemies[i];
        if (!e) continue;
        int r = e->getRow();
        int c = e->getCol();
        if (!inBounds(r, c)) continue;

        // getTypeName() must return a single-character code for this to print correctly
        board[r][c] = static_cast<Enemy*>(e)->getTypeName();
    }

    for (int i = 0; i < 20; i++) {
        // If the slot is not empty, get its position and character type
        if (List[i] != nullptr) {
            int row = List[i]->getRow();
            int col = List[i]->getCol();

            char type = List[i]->getEntityType();
        }
    }

    std::string Template;

    // Print out Board
    Template.append("+---------------------------------------+ \n");

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Template.append("|");
            Template.append(1, board[i][j]);
        }
        Template.append(1, '|');
        Template.append(1, '\n');
    }
    Template.append("+---------------------------------------+ \n");
    std::cout << Template;
}


// --- Dungeon 5x5 helpers over the same board region (0..4,0..4) ---

void Board::initializeDungeonXGrid()
{
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c)
            board[r][c] = ' ';
}

void Board::setCellContentDungeon(int row, int col, char content)
{
    if (row >= 0 && row < 5 && col >= 0 && col < 5)
        board[row][col] = content;
}

char Board::getCellContentDungeon(int row, int col) const
{
    if (row >= 0 && row < 5 && col >= 0 && col < 5)
        return board[row][col];
    return ' ';
}

void Board::drawDungeon()
{
    std::cout << ("+---------+ \n");
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            std::cout << '|' << board[i][j];
        }
        std::cout << "|\n";
    }
    std::cout << ("+---------+ \n");
}

// --- Accessor ---

char Board::GetBoard(int row, int col) const
{
    if (!inBounds(row, col)) return ' ';
    return board[row][col];
}

// --- Clear (non-owning): only tiles and references ---

void Board::clearBoard()
{
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            board[r][c] = ' ';

    Player = nullptr;
    selectedEnemy = nullptr;
    for (int i = 0; i < enemyCount; ++i) enemies[i] = nullptr;
    enemyCount = 0;
}

// --- Remove enemy reference ---

void Board::removeEnemy(Entity* e)
{
    for (int i = 0; i < enemyCount; ++i) {
        if (enemies[i] == e) {
            enemies[i] = enemies[--enemyCount];
            enemies[enemyCount] = nullptr;
            return;
        }
    }
    std::cout << "Enemy not found.\n";
}

// --- Save/Load stubs (implement if you need them) ---

bool Board::save(const char* /*path*/) const { return false; }
bool Board::load(const char* /*path*/) { return false; }
