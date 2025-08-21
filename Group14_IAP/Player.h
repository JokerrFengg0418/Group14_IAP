#pragma once
#include "Entity.h"
#include <string>
#include "Enemy.h"
#include "Inventory.h"

class Player : public Entity
{

public:


	Player();
	Player(int r, int c, int hp, int dmg);
	~Player();

	void takeDamage(int amount);

	void move();

};

