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
	health = hp;
	damage = dmg;
	std::cout << "Enemy Created at (" << r << ", " << c << ") with type: "
		<< static_cast<int>(type) << ", health: " << health
		<< ", damage: " << damage << std::endl;
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

int Enemy::getHealth() const
{
	return health;
}

int Enemy::getDamage() const
{
	return damage;
}

// Setters

void Enemy::setHealth(int hp)
{
	health = hp;
}

void Enemy::setDamage(int dmg)
{
	damage = dmg;
}

// Behaviours 
void Enemy::takeDamage(int amount) 
{
	health -= amount;
	if (health < 0) {
		health = 0; // Ensure health doesn't go below 0
		std::cout << getTypeName() << " has been defeated!" << std::endl;
	}
}

void Enemy::attack() const
{
	std::cout << getTypeName() << " attacks for " << damage << " damage!" << std::endl;
}

void Enemy::moveEnemy()
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

void Enemy::printStatus() const
{
	std::cout << "Enemy Type: " << getTypeName() << ", Health: " << health
		<< ", Damage: " << damage << ", Location: ("
		<< getRow() << ", " << getCol() << ")" << std::endl;
}