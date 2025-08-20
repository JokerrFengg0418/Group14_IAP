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
	case EnemyType::Monster:   return 'M';
	case EnemyType::Hellhound: return 'H';
	case EnemyType::Zombie:    return 'Z';
	case EnemyType::Goblin:    return 'G';
	case EnemyType::Bat:       return 'B';
	case EnemyType::Skeleton:  return 'S';
	case EnemyType::Gargoyle:  return 'G';
	case EnemyType::Boss:      return 'B';
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

void Enemy::attack(Player* player)
{
	if (player != nullptr) {
		player->takeDamage(getDamage());
		std::cout << "Enemy attacks the player, dealing " << getDamage() << " damage." << std::endl;
	}
}


void Enemy::move()
{
	// Example: Move randomly in one of the four directions
	int direction = rand() % 4; // 0: up, 1: down, 2: left, 3: right
	switch (direction) {
	case 0:
		setRow(getRow() - 1); // Move up
		break;
	case 1:
		setRow(getRow() + 1); // Move down
		break;
	case 2:
		setCol(getCol() - 1); // Move left
		break;
	case 3:
		setCol(getCol() + 1); // Move right
		break;
	}
}

// Debug
void Enemy::printStatus() const
{
	std::cout << "Enemy Type: " << getTypeName() << ", Health: " << getHealth()
		<< ", Damage: " << getDamage() << ", Location: ("
		<< getRow() << ", " << getCol() << ")" << std::endl;
}

