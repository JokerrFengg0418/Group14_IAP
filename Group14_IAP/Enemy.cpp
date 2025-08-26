#include "Enemy.h"
#include <iostream>

// Default Constructor
Enemy::Enemy()
{
	std::cout << "Enemy Created." << std::endl;
}

// Overloaded Constructor to set initial position, type, health, and damage
Enemy::Enemy(int r, int c, EnemyType type, int hp, int dmg)
{
	setRow(r);
	setCol(c);
	this->type = type;
	setEntityType('E');
	setHealth(hp);   // 
	setDamage(dmg);  // 
	std::cout << "Enemy Created at (" << r << ", " << c << ") with type: "
		<< static_cast<int>(type) << ", health: " << getHealth()
		<< ", damage: " << getDamage() << std::endl;
}

 
// Destructor
Enemy::~Enemy()
{
	std::cout << "Enemy Destroyed." << std::endl;
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
	// Decide a direction: up/down/left/right
	int dr = 0, dc = 0;
	switch (rand() % 4) {
	case 0: dr = -1; break; // up
	case 1: dr = +1; break; // down
	case 2: dc = -1; break; // left
	case 3: dc = +1; break; // right
	}

	// Current and proposed positions
	const int r = getRow();
	const int c = getCol();
	const int tr = r + dr;
	const int tc = c + dc;

	// Adjust these to your board size (25x25 -> 0..24)
	constexpr int ROWS = 25;
	constexpr int COLS = 25;

	// Bounds check
	if (tr < 0 || tr >= ROWS || tc < 0 || tc >= COLS) {
		return; // blocked by wall
	}

	// Collision check: is target tile occupied by anyone else?
	for (int i = 0; i < 20; ++i) {
		Entity* e = List[i];
		if (!e || e == this) continue;
		if (e->getRow() == tr && e->getCol() == tc) {
			return; // tile occupied (player or another enemy)
		}
	}

	// Move
	setRow(tr);
	setCol(tc);
}

// Debug
void Enemy::printStatus() const
{
	std::cout << "Enemy Type: " << getTypeName() << ", Health: " << getHealth()
		<< ", Damage: " << getDamage() << ", Location: ("
		<< getRow() << ", " << getCol() << ")" << std::endl;
}

