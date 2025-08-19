#pragma once
#include "Entity.h"

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
	int health; // health of enemy
	int damage; // damage of enemy


public:

	// Default Constructor
	Enemy();

	// Overloaded Constructor
	Enemy(int r, int c, EnemyType type, int hp, int dmg);

	// Default Destructor
	~Enemy();

	// Getters
	EnemyType getType() const;
	std::string getTypeName() const; // get readable name
	int getHealth() const;
	int getDamage() const;


	// Setters
	void setHealth(int hp);
	void setDamage(int dmg);

	// Enemy Behaviour
	void takeDamage(int amount);
	void attack() const;
	void moveEnemy();

	// Debug
	void printStatus() const;

};

