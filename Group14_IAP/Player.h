#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
	void move();

	Player(int r, int c, int hp, int dmg);
};

