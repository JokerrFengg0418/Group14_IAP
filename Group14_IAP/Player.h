#pragma once
#include "Entity.h"
#include <string>

class Player : public Entity
{

public:



	Player();
	Player(int r, int c, int hp, int dmg);
	~Player();

	void takeDamage(int amount);

	void PlayerAttack(Enemy* enemy);

	void Move();


};

