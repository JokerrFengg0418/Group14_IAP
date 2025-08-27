#include "Enemy.h"
#include <iostream>

// Default Constructor
Enemy::Enemy()
{
	// Enemy Created
}

// Overloaded Constructor to set initial position, type, health, and damage
Enemy::Enemy(int r, int c, EnemyType type, int hp, int dmg)
{
	setRow(r);
	setCol(c);
	this->type = type;
	setEntityType('E');
	setHealth(hp);   
	setDamage(dmg); 
	//std::cout << "Enemy Created at (" << r << ", " << c << ") with type: "
	//	<< static_cast<int>(type) << ", health: " << getHealth()
	//	<< ", damage: " << getDamage() << std::endl;
}

 
// Destructor
Enemy::~Enemy()
{
	// Enemy Destroyed
}

// Getters
EnemyType Enemy::getType() const
{
	return type;
}

// Returns a character representation of the enemy type defined in Enemy.h as a enum
char Enemy::getTypeName() const 
{ 
	switch (type) {
	case EnemyType::Rat:	   return 'R';
	case EnemyType::Hellhound: return 'H';
	case EnemyType::Zombie:    return 'Z';
	case EnemyType::Goblin:    return 'G';
	case EnemyType::Bat:       return 'B';
	case EnemyType::Skeleton:  return 'S';
	case EnemyType::Witch:	   return 'W';
	case EnemyType::Boss:      return 'A';
	default:                   return 'U';
	}
}


// Enemy Behaviours 
void Enemy::takeDamage(int amount)
{
	setHealth(getHealth() - amount);
	if (getHealth() <= 0) {
		setHealth(0);
		std::cout << getTypeName() << " has been defeated!" << std::endl;
	}
}


void Enemy::move(Entity* List[20])
{
    // Randomise enemy movement
    int rowOffset = 0;
    int colOffset = 0;
    switch (rand() % 4) {
    case 0: 
        rowOffset = -1; 
        break; // up
    case 1: 
        rowOffset = +1; 
        break; // down
    case 2: 
        colOffset = -1; 
        break; // left
    case 3: 
        colOffset = +1; 
        break; // right
    }

    // Current and proposed positions
    const int currentRow = getRow();
    const int currentCol = getCol();
    const int newRow = currentRow + rowOffset;
    const int newCol = currentCol + colOffset;

    // 20x20 board size
    constexpr int ROWS = 20;
    constexpr int COLS = 20;

    // Boundary Check
    if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS) {
        return; // blocked by wall
    }

    // Collision check
    for (int i = 0; i < 20; ++i) {
        Entity* entity = List[i];
        if (!entity || entity == this) continue;
        if (entity->getRow() == newRow && entity->getCol() == newCol) {
            return; // tile occupied (player or another enemy)
        }
    }

    // Move
    setRow(newRow);
    setCol(newCol);
}


// Debug
void Enemy::printStatus() const
{
	std::cout << "Enemy Type: " << getTypeName() << ", Health: " << getHealth()
		<< ", Damage: " << getDamage() << ", Location: ("
		<< getRow() << ", " << getCol() << ")" << std::endl;
}

