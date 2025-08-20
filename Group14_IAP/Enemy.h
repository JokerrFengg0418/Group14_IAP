#pragma once
#include "Entity.h"
#include "Player.h"
#include <string>

// define different types of Enemies with enum
enum class EnemyType {
	Monster, // M
	Hellhound, // H
	Zombie, // Z
	Goblin, // G
	Bat, // B
	Skeleton, // S
	Gargoyle, // G
	Boss // B
};

class Enemy : public Entity  // inheritance from Entity
{


private:
	EnemyType type; // type of enemy
	Position pos; // position of the enemy

public:

	// Default Constructor
	Enemy();

	// Overloaded Constructor
	Enemy(int r, int c, EnemyType type, int hp, int dmg);

	// Default Destructor
	~Enemy();

	// Getters
	EnemyType getType() const;
	char getTypeName() const; // get readable name

	// Enemy Behaviour
	void takeDamage(int amount);
	void attack(Player* player);
	void move();

	// Debug
	void printStatus() const;

};

