#include "Player.h"
#include "Enemy.h"
#include "Inventory.h"
#include <conio.h>
#include <iostream>
#include "Position.h"
#include "Shop.h"
#include "Dungeon.h"
#include <cctype>

Player::Player()
{
	Health = 100;
    actedThisTurn = false;
	//std::cout << "Player Created \n";
}

Player::Player(int r, int c, int hp, int dmg)
{
	setRow(r);
	setCol(c);
	setHealth(hp);   // 
	setDamage(dmg);  // 
	setEntityType('P');
    actedThisTurn = false;
	//std::cout << "Player Created at (" << r << ", " << c << ")" << ", health: " << getHealth() << ", damage: " << getDamage() << std::endl;
}

Player::~Player()
{
}


// Player Behaviours 
void Player::takeDamage(int amount)
{
	setHealth(getHealth() - amount);
	if (getHealth() <= 0) {
		setHealth(0);
		std::cout << " Player has been defeated!" << std::endl;

	}
}

void Player::move(Entity* list[20]) {
    std::cout << "Use WASD to move\n";

    // current position
    int row = getRow();
    int col = getCol();

    // target position starts as current
    int newRow = row;
    int newCol = col;

    const int ROWS = 20;  // adjust to your board size
    const int COLS = 20;

    const char input = _getch();
    switch (input) {
    case 'w': case 'W':
        newRow = row - 1;
        std::cout << "Move Up\n";
        break;
    case 's': case 'S':
        newRow = row + 1;
        std::cout << "Move Down\n";
        break;
    case 'a': case 'A':
        newCol = col - 1;
        std::cout << "Move Left\n";
        break;
    case 'd': case 'D':
        newCol = col + 1;
        std::cout << "Move Right\n";
        break;
    default:
        std::cout << "invalid input\n";
        return; // don't move on invalid input
    }

    // bounds check (both axes)
    if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS) {
        return; // out of bounds: ignore the move
    }

    // apply the move
    setRow(newRow);
    setCol(newCol);

    actedThisTurn = true;

    // collision: check player vs others (e.g., enemies 'E')
    for (int i = 0; i < 20; ++i) {
        Entity* e = list[i];
        if (!e || e == this) continue; // skip empty and yourself if present

        if (e->getPosition().getRow() == newRow &&
            e->getPosition().getCol() == newCol) {
            if (e->getEntityType() == 'E') {
                // handle player-enemy collision here
                // e.g., takeDamage(), start combat, or just return
                return;
            }
        }
    }
}


bool Player::moveDungeon()
{
    // current position
    int row = getRow();
    int col = getCol();

    // target position starts as current
    int newRow = row;
    int newCol = col;

    const int ROWS = 5;  // dungeon board size
    const int COLS = 5;

    const char input = _getch();
    switch (toupper(input)) {
    case 'W':
        newRow--;
        //std::cout << "Move Up\n";
        break;
    case 'S':
        newRow++;
        //std::cout << "Move Down\n";
        break;
    case 'A':
        newCol--;
        //std::cout << "Move Left\n";
        break;
    case 'D':
        newCol++;
        //std::cout << "Move Right\n";
        break;
    case 'E':
        return true; // signal exit
    default:
        //std::cout << "Invalid input\n";
        return false; // ignore bad key
    }

    // bounds check
    if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS) {
        return false; // out of bounds: ignore move
    }

    // apply the move
    setRow(newRow);
    setCol(newCol);


    return false; // no exit, just moved
}










