#pragma once
#include "Entity.h"
#include <string>
#include "Enemy.h"
#include "Inventory.h"

class Player : public Entity
{

private:
	Inventory* playerInventory;
public:


	Player();
	Player(int r, int c, int hp, int dmg);
	~Player();

	void takeDamage(int amount);

	void attack(Enemy* enemy);

	void move();

	// Inventory Functions
	Inventory& getInventory();
	const Inventory& getInventory() const;


};

