#pragma once
#include "Entity.h"
#include "Player.h"
#include <string>

// Forward declaration of the Player class
class Player;

// define different types of Enemies with enum
enum class EnemyType {
	Rat, // R
	Hellhound, // H
	Zombie, // Z
	Goblin, // G
	Bat, // B
	Skeleton, // S
	Witch, // W
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
	void move(Entity* List[20]);

	// Debug
	void printStatus() const;

};

